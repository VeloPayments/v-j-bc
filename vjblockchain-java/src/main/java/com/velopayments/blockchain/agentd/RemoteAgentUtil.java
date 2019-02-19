package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.GenericStreamCipher;
import com.velopayments.blockchain.util.ByteUtil;

import java.util.Arrays;
import java.util.concurrent.atomic.AtomicLong;

public class RemoteAgentUtil {

    private static AtomicLong iv = new AtomicLong(0);

    /**
     * Wrap the inner envelope by encrypting and HMAC'ing the
     * message.  The returned value represents the outer envelope.
     *
     * byte 0            message type
     * bytes 1 - 4       size of payload big endian format
     * bytes 5 - N+5     encrypted payload
     * bytes N+6 - N+38  HMAC of envelope (all previous bytes)

     * @param messageType    message type
     * @param inner          unencrypted payload (inner envelope)
     * @param key            key to use to encrypt the payload
     * @return
     */
    public static byte[] wrapOuter(MessageType messageType, byte[] inner,
                                   byte[] key) {

        byte outer[] = new byte[1 + 4 + inner.length + 4];

        // the first byte is the message type

        outer[0] = (byte)messageType.getValue();

        // bytes 1 - 4 are the size of the encrypted payload

        byte[] encryptedPayload = GenericStreamCipher.encryptData(key,
                ByteUtil.longToBytes(iv.getAndIncrement(),false), inner);

        System.arraycopy(
                ByteUtil.longToBytes(encryptedPayload.length,4, true),
                0,
                outer,
                1,
                4);

        // bytes 5 - N+5 are the encrypted payload

        System.arraycopy(encryptedPayload, 0, outer, 5, encryptedPayload.length);

        // bytes N+6 - N+38 are the HMAC'd value

        HMAC hmac = new HMAC(key);
        //hmac.createHMACLong()   // TODO: - should be short HMAC of the entire outer envelope


        return outer;
    }

    /**
     * Get the current value of the IV
     *
     * @return
     */
    public static long getCurrentIV() {
        return iv.get();
    }

    /**
     * Unwrap the outer envelope by HMAC'ing and then decrypting the
     * payload.  The returned value represents the "inner envelope."
     *
     * @param outer
     * @return
     */
    public static byte[] unwrapOuter(byte[] outer, byte[] key) {

        // the last 32 bytes are the HMAC of the previous bytes
        byte[] hmac = Arrays.copyOfRange(outer, outer.length - 32, outer.length);
        // TODO: verify HMAC

        // the first byte is the type, which should be "authed data"
        byte messageType = outer[0];

        // the next four bytes are the size of the payload
        int payloadSize = (int)ByteUtil.bytesToLong(
                Arrays.copyOfRange(outer, 1, 5),true);

        // bytes 5 - payloadSize+5 are the payload
        byte[] encryptedPayload = Arrays.copyOfRange(outer, 5, payloadSize+5);

        return GenericStreamCipher.decryptData(key, encryptedPayload);
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
        System.arraycopy(ByteUtil.longToBytes(requestId, 4,true),
            0, wrapped, 0, 4);

        // the next four bytes are the request offset
        System.arraycopy(ByteUtil.longToBytes(requestOffset, 4, true),
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
                Arrays.copyOfRange(wrapped, 0, 4),true);

        long requestOffset = ByteUtil.bytesToLong(
                Arrays.copyOfRange(wrapped, 4, 8),true);

        long status = ByteUtil.bytesToLong(
                Arrays.copyOfRange(wrapped, 8, 12),true);

        byte[] payload = Arrays.copyOfRange(wrapped, 12, wrapped.length);


        return new UnwrappedInnerEnvelope(requestId, requestOffset, status, payload);
    }


}
