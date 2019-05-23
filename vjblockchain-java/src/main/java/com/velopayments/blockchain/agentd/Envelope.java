package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.GenericStreamCipher;
import com.velopayments.blockchain.crypt.HMAC;
import com.velopayments.blockchain.util.ByteUtil;
import com.velopayments.blockchain.util.EqualsUtil;

import java.util.Arrays;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Utility methods to wrap and unwrap inner and outer envelopes.
 */
public class Envelope {

    private static AtomicLong iv = new AtomicLong(0);

    /**
     * Wraps the inner envelope by encrypting and HMAC'ing the
     * message.  The returned value represents the outer envelope.
     * <p>
     * bytes   0 - N      encrypted payload
     * bytes N+1 - N+32   HMAC of encrypted payload
     * </p>
     * The IV is incremented after this call.
     *
     * @param key            key to use to encrypt the payload
     * @param input          unencrypted payload (inner envelope)
     *
     * @return               the outer envelope
     */
    public static byte[] wrapOuter(byte[] key, byte[] input) {

        // encrypt the payload
        byte[] ivBytes = ByteUtil.longToBytes(iv.getAndIncrement());
        byte[] encryptedPayload = GenericStreamCipher.encrypt(
                key, ivBytes, input);

        // create a buffer to hold the output
        byte outer[] = new byte[encryptedPayload.length + 32];

        // add the encrypted payload to the output buffer
        System.arraycopy(
                encryptedPayload, 0, outer, 0, encryptedPayload.length);

        // add the HMAC of the encrypted payload to the output buffer
        HMAC hmac = new HMAC(key);
        System.arraycopy(
                hmac.createHMACShort(encryptedPayload),
                0, outer, outer.length - 32, 32);

        return outer;
    }

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
    public static byte[] unwrapOuter(byte[] key, byte[] outer) {

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
    }


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
