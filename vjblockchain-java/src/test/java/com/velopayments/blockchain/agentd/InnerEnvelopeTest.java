package com.velopayments.blockchain.agentd;

import org.junit.Test;

import java.util.Arrays;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class InnerEnvelopeTest {

    @Test
    public void wrap() {

        // given an API method ID, request ID, and a payload
        long requestId = 0xFFL;
        byte[] payload = "this is my payload".getBytes();

        // when the payload is wrapped
        byte[] wrapped = InnerEnvelope.wrap(ApiMethod.SUBMIT, requestId, payload);

        // then the wrapped payload should be the correct size
        assertThat(wrapped, notNullValue());
        assertThat(wrapped.length, is(4 + 4 + payload.length));

        // and the first 4 bytes should be the API Method ID in big endian
        assertThat(wrapped[0], is((byte)0));
        assertThat(wrapped[1], is((byte)0));
        assertThat(wrapped[2], is((byte)0));
        assertThat(wrapped[3], is((byte)1));

        // and the next 4 bytes should be the request ID in big endian
        assertThat(wrapped[4], is((byte)0x00));
        assertThat(wrapped[5], is((byte)0x00));
        assertThat(wrapped[6], is((byte)0x00));
        assertThat(wrapped[7], is((byte)0xFF));


        // and the remaining bytes should be the payload
        assertThat(
                Arrays.copyOfRange(wrapped, 8, wrapped.length),
                is(payload));
    }

    @Test
    public void unwrap() {

        // given an inner envelope
        byte[] inner = new byte[] {
            (byte)0xFF,(byte)0x00,(byte)0x00,(byte)0x01,  // API method ID
            (byte)0xEE,(byte)0x00,(byte)0x00,(byte)0x02,  // request ID
            (byte)0x01,(byte)0x02,(byte)0x03,(byte)0x04,  // status
            (byte)0x87, (byte)0xa0                        // payload
        };

        // when the envelope is unwrapped
        InnerEnvelopeResponse unwrapped = InnerEnvelope.unwrap(inner);

        // then the API method Id should be correct
        assertThat(unwrapped.getApiMethodId(), is(0xFF000001L));

        // and the request ID should be correct
        assertThat(unwrapped.getRequestId(), is(0xEE000002L));

        // and the status should be correct
        assertThat(unwrapped.getStatus(), is(0x1020304L));

        // and the payload should be correct
        assertThat(unwrapped.getPayload(),
                is(new byte[] {(byte)0x87, (byte)0xa0}));
    }

    @Test
    public void unwrap_noPayload() {

        // given an inner envelope with no payload
        byte[] inner = new byte[] {
                (byte)0x3F,(byte)0x00,(byte)0x00,(byte)0x01,  // API method ID
                (byte)0xEE,(byte)0x00,(byte)0x00,(byte)0x02,  // request ID
                (byte)0x01,(byte)0x02,(byte)0x03,(byte)0x04   // status
        };

        // when the envelope is unwrapped
        InnerEnvelopeResponse unwrapped = InnerEnvelope.unwrap(inner);

        // then the API method Id should be correct
        assertThat(unwrapped.getApiMethodId(), is(0x3F000001L));

        // and the request ID should be correct
        assertThat(unwrapped.getRequestId(), is(0xEE000002L));

        // and the status should be correct
        assertThat(unwrapped.getStatus(), is(0x1020304L));

        // and the payload should be empty
        assertThat(unwrapped.getPayload(), is(new byte[0]));
    }

}
