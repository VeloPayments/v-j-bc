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
     * byte 0            message type
     * bytes 1 - 4       size of payload
     * bytes 5 - N+5     encrypted payload
     * bytes N+6 - N+38  HMAC of envelope (all previous bytes)
     * </p>
     * The IV is incremented after this call.
     *
     * @param messageType    message type
     * @param key            key to use to encrypt the payload
     * @param input          unencrypted payload (inner envelope)
     * @return               the outer envelope
     */
    public static byte[] wrapOuter(MessageType messageType, byte[] key,
                                   byte[] input) {

        // encrypt the payload
        byte[] ivBytes = ByteUtil.longToBytes(iv.getAndIncrement());
        byte[] encryptedPayload = GenericStreamCipher.encrypt(key, ivBytes, input);

        // create a buffer to hold the output
        byte outer[] = new byte[1 + 4 + encryptedPayload.length + 32];

        // the first byte is the message type
        outer[0] = (byte)messageType.getValue();

        // bytes 1 - 4 are the size of the encrypted payload
        byte[] payloadSize = ByteUtil.htonl(encryptedPayload.length);
        System.arraycopy(payloadSize, 0, outer, 1, 4);

        // bytes 5 - N+5 are the encrypted payload
        System.arraycopy(
                encryptedPayload, 0, outer, 5,
                encryptedPayload.length);

        // bytes N+6 - N+38 are the HMAC'd value
        HMAC hmac = new HMAC(key);
        System.arraycopy(
                hmac.createHMACShort(
                        Arrays.copyOfRange(
                                outer, 0, outer.length - 32)),
                0, outer, outer.length - 32, 32);

        return outer;
    }

    /**
     * Unwraps the outer envelope by HMAC'ing and then decrypting the
     * payload.  The returned value represents the decrypted inner envelope.
     * <p>
     * The format of the outer envelope is as follows:
     *   byte 0            message type
     *   bytes 1 - 4       size of payload big endian format
     *   bytes 5 - N+5     encrypted payload
     *   bytes N+6 - N+38  HMAC of envelope (all previous bytes)
     *
     * @param key           The secret key to use for decryption
     * @param outer         The outer envelope
     *
     * @return              The decrypted payload
     */
    public static byte[] unwrapOuter(byte[] key, byte[] outer) {

        // the last 32 bytes are the HMAC of the previous bytes
        byte[] returnedHmac = Arrays.copyOfRange(outer, outer.length - 32, outer.length);

        // verify HMAC
        HMAC hmac = new HMAC(key);
        if (!EqualsUtil.constantTimeEqual(
                hmac.createHMACShort(Arrays.copyOfRange(outer, 0, outer.length-32)),
                returnedHmac))
        {
            throw new MessageVerificationException("Invalid HMAC!");
        }

        // the first byte is the type, which should be "authed data"
        byte messageType = outer[0];

        // the next four bytes are the size of the payload
        int payloadSize = (int)ByteUtil.ntohl(
                Arrays.copyOfRange(outer, 1, 5));

        // bytes 5 - payloadSize+5 are the payload
        byte[] encryptedPayload = Arrays.copyOfRange(
                outer, 5, payloadSize+5);

        return GenericStreamCipher.decrypt(key, encryptedPayload);
    }


    /**
     * Wrap the payload in the inner envelope:
     * <p>
     *   bits  0 - 31 - API method ID in big endian
     *   bits 32 - 63 - request ID in big endian
     *   bits 64+     - payload
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
     *   bytes 0 -  3 - API method ID
     *   bytes 4 -  7 - request ID
     *   bytes 8 - 11 - status (0 = success, other for fail)
     *   bytes 12+     - payload
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
