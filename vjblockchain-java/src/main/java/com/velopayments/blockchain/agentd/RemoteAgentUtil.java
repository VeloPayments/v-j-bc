package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.util.ByteUtil;

import java.util.Arrays;

public class RemoteAgentUtil {

    /**
     * Wrap the inner envelope by encrypting and HMAC'ing the
     * message.  The returned value represents the outer envelope.
     *
     * @param inner
     * @return
     */
    public static byte[] wrapOuter(MessageType messageType, byte[] inner, byte[] key) {

        return null;
    }

    /**
     * Unwrap the outer envelope by HMAC'ing and then decrypting the
     * payload.  The returned value represents the "inner envelope."
     *
     * @param outer
     * @return
     */
    public static byte[] unwrapOuter(byte[] outer) {

        return null;
    }


    /**
     * Wrap the payload in the inner envelope:
     *
     *   bits  0 - 31 - request ID in big endian
     *   bits 32 - 63 - request offset in big endian
     *   bits 64+     - payload
     *
     * @param requestId      request ID.  should be in range 0 - 2^31-1
     * @param requestOffset  request offset.  should be in range 0 - 2^31-1
     * @param payload        payload to wrap
     * @return
     */
    public static byte[] wrapInner(final long requestId, final long requestOffset, byte[] payload) {

        byte[] wrapped = new byte[8 + payload.length];

        // the first four bytes are the request ID
        System.arraycopy(ByteUtil.longToBytes(requestId, 4),
            0, wrapped, 0, 4);

        // the next four bytes are the request offset
        System.arraycopy(ByteUtil.longToBytes(requestOffset, 4),
                0, wrapped, 4, 4);

        // the remaining bytes are the payload
        System.arraycopy(payload, 0, wrapped, 8, payload.length);

        return wrapped;
    }

    /**
     * Unwrap the inner envelope.  The wrapped envelope should be structured as:
     *
     *   bits  0 - 31 - request ID in big endian
     *   bits 32 - 63 - request offset in big endian
     *   bits 64 - 95 - status
     *   bits 96+     - payload
     *
     * @param wrapped     The inner envelope to be unwrapped
     *
     * @return            The unwrapped envelope
     */
    public static UnwrappedInnerEnvelope unwrapInner(byte[] wrapped) {

        long requestId = ByteUtil.bytesToLong(
                Arrays.copyOfRange(wrapped, 0, 4));

        long requestOffset = ByteUtil.bytesToLong(
                Arrays.copyOfRange(wrapped, 4, 8));

        long status = ByteUtil.bytesToLong(
                Arrays.copyOfRange(wrapped, 8, 12));

        byte[] payload = Arrays.copyOfRange(wrapped, 12, wrapped.length);


        return new UnwrappedInnerEnvelope(requestId, requestOffset, status, payload);
    }

    /**
     * Encrypt data in the input buffer, returning an encrypted buffer.
     *
     * @param key   The key to use to encrypt this data.
     * @param iv    The initialization vector to use.
     * @param input The input to encrypt.
     *
     * @return the encrypted data with iv / HMAC prepended / appended.
     */
    //private static native byte[] encryptData(
    //        byte[] key, byte[] iv, byte[] input);

}
