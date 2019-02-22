package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.GenericStreamCipher;
import com.velopayments.blockchain.util.ByteUtil;

public class TestUtil {

    /**
     * The format of the outer envelope response is as follows:
     *   byte 0            message type
     *   bytes 1 - 4       size of payload big endian format
     *   bytes 5 - N+5     encrypted payload
     *   bytes N+6 - N+38  HMAC of envelope (all previous bytes)
     *
     * @param encryptionKey
     * @param apiMethod
     * @param requestId
     * @param status
     * @param payload
     * @return
     */
    public static byte[] createAgentdResponse(byte[] encryptionKey,ApiMethod apiMethod,
                                              long requestId, long status, byte[] payload) {

        // build inner envelope response
        byte inner[] = new byte[12 + payload.length];

        // first four bytes of inner envelope are the api method
        System.arraycopy(ByteUtil.longToBytes(apiMethod.getValue(),true),
                0, inner,0,4);

        // next four bytes of inner envelope are the request ID
        System.arraycopy(ByteUtil.longToBytes(requestId, true), 0, inner, 4, 4);

        // next four bytes of inner envelope are the response
        System.arraycopy(ByteUtil.longToBytes(status, true), 0, inner, 8, 4);

        // the remaining bytes of the inner envelope are the payload
        System.arraycopy(payload, 0, inner, 12, payload.length);

        // now to build the outer envelope
        byte[] encryptedPayload = GenericStreamCipher.encrypt(encryptionKey, new byte[8], inner);

        // create a buffer to hold the output
        byte response[] = new byte[1 + 4 + encryptedPayload.length + 32];

        // bytes 1 - 4 are the size of the encrypted payload
        System.arraycopy(ByteUtil.longToBytes(encryptedPayload.length,4, true), 0, response, 1, 4);

        // bytes 5 - N+5 are the encrypted payload
        System.arraycopy(encryptedPayload, 0, response, 5, encryptedPayload.length);

        // TODO: HMAC

        return response;
    }

}
