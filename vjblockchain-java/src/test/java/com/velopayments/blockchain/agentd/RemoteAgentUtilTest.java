package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.GenericStreamCipher;
import com.velopayments.blockchain.util.ByteUtil;
import org.junit.Test;

import java.util.Arrays;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class RemoteAgentUtilTest {

    @Test
    public void wrapInner() {

        // given a request ID, request offset, and a payload

        long requestId = 0x01L;
        long requestOffset = 0xFFL;
        byte[] payload = "this is my payload".getBytes();

        // when the payload is wrapped

        byte[] wrapped = RemoteAgentUtil.wrapInner(requestId, requestOffset, payload);

        // then the wrapped payload should be the correct size
        assertThat(wrapped, notNullValue());
        assertThat(wrapped.length, is(4 + 4 + payload.length));

        // and the first 4 bytes should be the request ID in big endian
        assertThat(wrapped[0], is((byte)0));
        assertThat(wrapped[1], is((byte)0));
        assertThat(wrapped[2], is((byte)0));
        assertThat(wrapped[3], is((byte)1));

        // and the next 4 bytes should be the request offset in big endian
        assertThat(wrapped[4], is((byte)0x00));
        assertThat(wrapped[5], is((byte)0x00));
        assertThat(wrapped[6], is((byte)0x00));
        assertThat(wrapped[7], is((byte)0xFF));


        // and the remaining bytes should be the payload
        assertThat(Arrays.copyOfRange(wrapped, 8, wrapped.length), is(payload));
    }

    @Test
    public void unwrapInner() {

        // given an inner envelope

        byte[] inner = new byte[] {
            (byte)0xFF,(byte)0x00,(byte)0x00,(byte)0x01,  // request ID
            (byte)0xEE,(byte)0x00,(byte)0x00,(byte)0x02,  // request offset
            (byte)0x01,(byte)0x02,(byte)0x03,(byte)0x04,  // status
            (byte)0x87, (byte)0xa0 // payload
        };

        // when the envelope is unwrapped
        UnwrappedInnerEnvelope unwrapped = RemoteAgentUtil.unwrapInner(inner);

        // then the request Id should be correct
        assertThat(unwrapped.getRequestId(), is(0xFF000001L));

        // and the request offset should be correct
        assertThat(unwrapped.getRequestOffset(), is(0xEE000002L));

        // and the status should be correct
        assertThat(unwrapped.getStatus(), is(0x1020304L));

        // and the payload should be correct
        assertThat(unwrapped.getPayload(), is(new byte[] {(byte)0x87, (byte)0xa0}));
    }

    @Test
    public void wrapOuter() {

        // given an inner envelope, an encryption key, and an IV
        byte[] inner = "this is my inner envelope.".getBytes();
        byte[] key = EncryptionKeyPair.generate().getPrivateKey().getRawBytes();

        // when the envelope is wrapped
        byte[] wrapped = RemoteAgentUtil.wrapOuter(MessageType.HANDSHAKE, key, inner);

        // then the first byte should be the message type
        assertThat(wrapped[0], is((byte)MessageType.HANDSHAKE.getValue()));

        // ... and the next 4 bytes should be the size of the encrypted payload
        long payloadSize = ByteUtil.bytesToLong(Arrays.copyOfRange(wrapped, 1, 5), true);

        // ... and the next N bytes should be the encrypted payload
        byte[] encrypted = Arrays.copyOfRange(wrapped, 5, (int)payloadSize+5);
        assertThat(GenericStreamCipher.decrypt(key, encrypted), is(inner));

        // ... and the last 32 bytes should be the HMAC of the previous bytes
        // TODO
    }

    @Test
    public void unwrapOuter() {

        // given an outer envelope

        byte[] inner = "this is my inner envelope.".getBytes();
        byte[] key = EncryptionKeyPair.generate().getPrivateKey().getRawBytes();
        byte[] wrapped = RemoteAgentUtil.wrapOuter(MessageType.HANDSHAKE, key, inner);


        // when the envelope is unwrapped

        byte[] unwrapped = RemoteAgentUtil.unwrapOuter(key, wrapped);

        // the decrypted message is correct

        assertThat(unwrapped, is(inner));
    }

}
