package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.crypt.HMAC;
import com.velopayments.blockchain.util.ByteUtil;
import com.velopayments.blockchain.util.UuidUtil;

import java.security.SecureRandom;
import java.util.Arrays;
import java.util.UUID;

public class TestUtil {

    static SecureRandom random = new SecureRandom();

    public static byte[] createInitiateHandshakeResponseHeader()
    {
        byte[] responseHeader = new byte[5];

        // response type
        responseHeader[0] = 0x20; // IPC_DATA_TYPE_DATA_PACKET

        // response size
        System.arraycopy(ByteUtil.htonl(164), 0, responseHeader, 1, 4);

        return responseHeader;
    }

    /* | Handshake request response packet.                            | */
    /* | ---------------------------------------------- | ------------ | */
    /* | DATA                                           | SIZE         | */
    /* | ---------------------------------------------- | ------------ | */
    /* | UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE      |   4 bytes    | */
    /* | offset                                         |   4 bytes    | */
    /* | status                                         |   4 bytes    | */
    /* | record:                                        | 152 bytes    | */
    /* |    protocol_version                            |   4 bytes    | */
    /* |    crypto_suite                                |   4 bytes    | */
    /* |    agent_id                                    |  16 bytes    | */
    /* |    server public key                           |  32 bytes    | */
    /* |    server key nonce                            |  32 bytes    | */
    /* |    server challenge nonce                      |  32 bytes    | */
    /* |    server_cr_hmac                              |  32 bytes    | */
    /* | ---------------------------------------------- | ------------ | */

    public static byte[] createInitiateHandshakeResponse(
            UUID agentdId, EncryptionPrivateKey entityPrivateKey)
    {
        byte[] response = new byte[164];

        // protocol version
        System.arraycopy(ByteUtil.htonl(1), 0, response, 12,4);

        // crypto version suite
        System.arraycopy(ByteUtil.htonl(1), 0, response, 16, 4);

        // agentd ID
        System.arraycopy(UuidUtil.getBytesFromUUID(agentdId), 0, response,
                20, 16);

        byte[] serverPublicKey = new byte[32];
        random.nextBytes(serverPublicKey);
        System.arraycopy(serverPublicKey, 0, response, 36,32);

        byte[] serverKeyNonce = new byte[32];
        random.nextBytes(serverKeyNonce);
        System.arraycopy(serverKeyNonce, 0, response, 68,32);

        byte[] serverChallengeNonce = new byte[32];
        random.nextBytes(serverChallengeNonce);
        System.arraycopy(serverChallengeNonce, 0, response, 100,32);

        byte[] sharedSecret = ProtocolHandlerImpl.computeSharedSecret(entityPrivateKey,
                new EncryptionPublicKey(serverPublicKey),
                serverKeyNonce,
                new byte[32]); // client key nonce generated using a mocked SecureRandom

        HMAC hmac = new HMAC(sharedSecret);
        byte[][] hmacParts = new byte[2][];
        hmacParts[0] = Arrays.copyOfRange(response, 0, 132);
        hmacParts[1] = new byte[32];  // client challenge nonce
        byte[] computedHMAC = hmac.createHMACShort(hmacParts);
        System.arraycopy(computedHMAC, 0, response, 132,32);


        return response;
    }

    public static byte[] createAckHandshakeResponseHeader()
    {
        byte[] responseHeader = new byte[5];

        // response type
        responseHeader[0] = 0x30; // IPC_DATA_TYPE_AUTHED_PACKET

        // response size
        System.arraycopy(ByteUtil.htonl(12), 0, responseHeader, 1, 4);

        return responseHeader;
    }

}
