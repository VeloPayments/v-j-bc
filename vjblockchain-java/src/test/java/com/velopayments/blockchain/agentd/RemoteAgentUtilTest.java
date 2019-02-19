package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class RemoteAgentUtilTest {

    RemoteAgentUtil remoteAgentUtil;

    @Before
    public void setup() {
        remoteAgentUtil = new RemoteAgentUtil();
    }

    @Test
    public void wrapInner() {

        // given a request ID, request offset, and a payload

        long requestId = 924059689;
        long requestOffset = 0xFFFFFFFF; // max
        byte[] payload = "this is my payload".getBytes();

        // when the payload is wrapped

        byte[] wrapped = remoteAgentUtil.wrapInner(requestId, requestOffset, payload);

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
    public void wrapOuter() {

        // given an inner envelope and an encryption key

        byte[] inner = "this is my inner envelope.".getBytes();
        byte[] key = EncryptionKeyPair.generate().getPrivateKey().getRawBytes();

        // when the envelope is wrapped

        byte[] wrapped = remoteAgentUtil.wrapOuter(MessageType.HANDSHAKE,
                inner, key);

        // then the first byte should be the message type


        // ... and the second 4 bytes should be the size of the inner payload
        // TODO: - minus 8 bytes?

        // ... and the next N-32 bytes should be the encrypted payload


        // ... and the last 32 bytes should be the encrypted payload HMAC'd
    }


}
