package com.velopayments.blockchain.agentd;

public class RemoteAgentUtil {

    /**
     * Wrap the inner envelope by encrypting and HMAC'ing the
     * message.  The returned value represents the outer envelope.
     *
     * @param inner
     * @return
     */
    public byte[] wrapOuter(MessageType messageType, byte[] inner, byte[] key) {

        return null;
    }

    /**
     * Unwrap the outer envelope by HMAC'ing and then decrypting the
     * payload.  The returned value represents the "inner envelope."
     *
     * @param outer
     * @return
     */
    public byte[] unwrapOuter(byte[] outer) {

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
    public byte[] wrapInner(final long requestId, final long requestOffset, byte[] payload) {

        byte[] wrapped = new byte[8 + payload.length];

        // the first four bytes are the request ID
        long r = requestId;
        for (int i=0; i<4 ;i++) {
            wrapped[i] = (byte)(r & 0xFF);
            r >>= 8;
        }

        // the next four bytes are the request offset
        long ro = requestOffset;
        for (int i=0; i<4; i++) {
            wrapped[i+4] = (byte)(ro & 0xFF);
            ro >>= 8;
        }

        // the remaining bytes are the payload
        System.arraycopy(payload, 0, wrapped, 8, payload.length);

        return wrapped;
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
