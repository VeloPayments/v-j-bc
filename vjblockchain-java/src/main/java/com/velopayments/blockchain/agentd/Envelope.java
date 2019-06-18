package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.util.ByteUtil;

import java.util.Arrays;

/**
 * Utility methods to wrap and unwrap inner and outer envelopes.
 */
public class Envelope {

    /**
     * Unwraps the outer envelope by HMAC'ing and then decrypting the
     * payload.  The returned value represents the inner envelope.
     * <p>
     * The format of the outer envelope is as follows:
     *
     *   bytes      0 - len-33     encrypted payload
     *   bytes len-32 - len        HMAC of encrypted payload
     *
     * @param key           The secret key to use for decryption
     * @param outer         The outer envelope
     *
     * @return              The decrypted payload
     */
    /*public static byte[] unwrapOuter(byte[] key, byte[] outer) {

        // the first len-32 bytes are the encrypted payload
        byte[] encryptedPayload = Arrays.copyOfRange(
                outer, 0, outer.length-32);

        // the last 32 bytes are the HMAC
        byte[] hmacBytes = Arrays.copyOfRange(
                outer, outer.length - 32, outer.length);

        // verify HMAC
        HMAC hmac = new HMAC(key);
        if (!EqualsUtil.constantTimeEqual(
                hmac.createHMACShort(encryptedPayload), hmacBytes))
        {
            throw new MessageVerificationException("Invalid HMAC!");
        }


        return GenericStreamCipher.decrypt(key, encryptedPayload);
    }*/


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
    public static byte[] wrapInner(ApiMethod apiMethod, final long requestId,
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
    public static InnerEnvelopeResponse unwrapInner(byte[] wrapped) {

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
