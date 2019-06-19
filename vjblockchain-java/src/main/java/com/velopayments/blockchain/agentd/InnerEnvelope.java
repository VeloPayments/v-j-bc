package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.util.ByteUtil;

import java.util.Arrays;

/**
 * Utility methods to wrap and unwrap inner envelopes.
 */
public class InnerEnvelope {

    /**
     * Wrap the payload in the inner envelope:
     * <p>
     *   bytes 0-3      API method ID
     *   bytes 4-7      request ID
     *   bytes 8+       payload
     *
     * @param apiMethod      API method
     * @param requestId      request ID
     * @param payload        payload to wrap
     * @return               the inner envelope
     */
    public static byte[] wrap(ApiMethod apiMethod, final long requestId,
                              byte[] payload)
    {
        byte[] wrapped = new byte[8 + payload.length];

        // the first four bytes are the API Method ID
        System.arraycopy(
                ByteUtil.htonl(apiMethod.getValue()),
                0, wrapped, 0, 4);

        // the next four bytes are the request ID
        // TODO: check for issues with sign bit
        System.arraycopy(
                ByteUtil.htonl((int)requestId), 0, wrapped, 4, 4);

        // the remaining bytes are the payload
        System.arraycopy(payload, 0, wrapped, 8, payload.length);

        return wrapped;
    }

    /**
     * Unwrap the inner envelope.  The wrapped envelope should be structured as:
     * <p>
     *   bytes 0-3      API method ID
     *   bytes 4-7      request ID
     *   bytes 8-11     status (0 = success, other for fail)
     *   bytes 12+      payload
     *
     * @param wrapped     The inner envelope to be unwrapped
     *
     * @return            The unwrapped envelope
     */
    public static InnerEnvelopeResponse unwrap(byte[] wrapped) {

        long apiMethodId = ByteUtil.ntohl(
                Arrays.copyOfRange(wrapped, 0, 4));

        long requestId = ByteUtil.ntohl(
                Arrays.copyOfRange(wrapped, 4, 8));

        long status = ByteUtil.ntohl(
                Arrays.copyOfRange(wrapped, 8, 12));

        byte[] payload = Arrays.copyOfRange(wrapped, 12, wrapped.length);

        return new InnerEnvelopeResponse(apiMethodId, requestId, status, payload);
    }


}
