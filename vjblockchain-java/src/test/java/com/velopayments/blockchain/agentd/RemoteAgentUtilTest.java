package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import org.junit.Test;

import java.util.Arrays;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class RemoteAgentUtilTest {

    @Test
    public void wrapInner() {

        // given a request ID, request offset, and a payload

        long requestId = 924059689;
        long requestOffset = 0xFFFFFFFF; // max
        byte[] payload = "this is my payload".getBytes();

        // when the payload is wrapped

        byte[] wrapped = RemoteAgentUtil.wrapInner(requestId, requestOffset, payload);

        // then the wrapped payload should be the correct size
        assertThat(wrapped, notNullValue());
        assertThat(wrapped.length, is(4 + 4 + payload.length));

        // and the first 4 bytes should be the request ID
        // 00110111 00010100 00001000 00101001
        assertThat(wrapped[0], is((byte)41));
        assertThat(wrapped[1], is((byte)8));
        assertThat(wrapped[2], is((byte)20));
        assertThat(wrapped[3], is((byte)55));

        // and the next 4 bytes should be the request offset
        assertThat(wrapped[4], is((byte)0xFF));
        assertThat(wrapped[5], is((byte)0xFF));
        assertThat(wrapped[6], is((byte)0xFF));
        assertThat(wrapped[7], is((byte)0xFF));


        // and the remaining bytes should be the payload
        assertThat(Arrays.copyOfRange(wrapped, 8, wrapped.length), is(payload));
    }

    @Test
    public void unwrapInner() {

        // given an inner envelope

        byte[] inner = new byte[] {
            (byte)0x01,(byte)0x02,(byte)0x00,(byte)0x00,  // request ID
            (byte)0x02,(byte)0x03,(byte)0x00,(byte)0x00,  // request offset
            (byte)0x01,(byte)0x02,(byte)0x03,(byte)0x04,  // status
            (byte)0x87, (byte)0xa0 // payload
        };

        // when the envelope is unwrapped
        UnwrappedInnerEnvelope unwrapped = RemoteAgentUtil.unwrapInner(inner);

        // then the request Id should be correct
        assertThat(unwrapped.getRequestId(), is(0x201L));

        // and the request offset should be correct
        assertThat(unwrapped.getRequestOffset(), is(0x302L));

        // and the status should be correct
        assertThat(unwrapped.getStatus(), is(0x4030201L));

        // and the payload should be correct
        assertThat(unwrapped.getPayload(), is(new byte[] {(byte)0x87, (byte)0xa0}));
    }

    @Test
    public void wrapOuter() {

        // given an inner envelope and an encryption key

        byte[] inner = "this is my inner envelope.".getBytes();
        byte[] key = EncryptionKeyPair.generate().getPrivateKey().getRawBytes();

        // when the envelope is wrapped

        byte[] wrapped = RemoteAgentUtil.wrapOuter(MessageType.HANDSHAKE, inner, key);

        // then the first byte should be the message type


        // ... and the second 4 bytes should be the size of the inner payload
        // TODO: - minus 8 bytes?

        // ... and the next N-32 bytes should be the encrypted payload


        // ... and the last 32 bytes should be the encrypted payload HMAC'd
    }


}
