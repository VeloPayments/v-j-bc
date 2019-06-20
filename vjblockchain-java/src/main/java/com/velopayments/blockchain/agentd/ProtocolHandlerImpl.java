package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.crypt.HMAC;
import com.velopayments.blockchain.init.Initializer;
import com.velopayments.blockchain.util.ByteUtil;
import com.velopayments.blockchain.util.EqualsUtil;
import com.velopayments.blockchain.util.UuidUtil;

import java.io.IOException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.UUID;

public class ProtocolHandlerImpl implements ProtocolHandler {

    public static final long PROTOCOL_VERSION    = 1L;
    public static final long CRYPTO_SUITE_VERSION = 1L;

    public static final byte IPC_DATA_TYPE_DATA_PACKET   = 0x20;

    public static final long UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE = 0L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_ACKNOWLEDGE = 1L;

    static {
        Initializer.init();
    }

    private DataChannel dataChannel;
    private UUID agentId;
    private UUID entityId;
    private EncryptionPrivateKey entityPrivateEncKey;
    private OuterEnvelopeReader outerEnvelopeReader;
    private SecureRandom random;

    private byte[] sharedSecret;

    public ProtocolHandlerImpl(
            DataChannel dataChannel, UUID agentId, UUID entityId,
            EncryptionPrivateKey entityPrivateEncKey,
            OuterEnvelopeReader outerEnvelopeReader,
            SecureRandom random)
    {
        this.dataChannel = dataChannel;
        this.agentId = agentId;
        this.entityId = entityId;
        this.entityPrivateEncKey = entityPrivateEncKey;
        this.outerEnvelopeReader = outerEnvelopeReader;
        this.random = random;
    }

    @Override
    public void handshake() throws IOException {
        byte[] serverChallengeNonce = initiateHandshake();
        acknowledgeHandshake(serverChallengeNonce);
    }

    /**
     * Initiate a handshake with the remote server
     *
     * @return server challenge nonce
     *
     * @throws IOException
     */
    private byte[] initiateHandshake() throws IOException {

        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE      |  4 bytes     | */
        /* | offset                                         |  4 bytes     | */
        /* | record:                                        | 88 bytes     | */
        /* |    protocol_version                            |  4 bytes     | */
        /* |    crypto_suite                                |  4 bytes     | */
        /* |    entity_id                                   | 16 bytes     | */
        /* |    client key nonce                            | 32 bytes     | */
        /* |    client challenge nonce                      | 32 bytes     | */
        /* | ---------------------------------------------- | ------------ | */


        byte[] request = new byte[96];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE - already
        // initialized as zero

        // bytes 4-7: offset - already initialized as zero

        // bytes 8-11: protocol version
        byte[] protocolVersionBytes = ByteUtil.htonl((int)PROTOCOL_VERSION);
        System.arraycopy(protocolVersionBytes, 0, request,  8, 4);

        // bytes 12-15: crypto suite
        byte[] cryptoSuiteBytes = ByteUtil.htonl((int)CRYPTO_SUITE_VERSION);
        System.arraycopy(cryptoSuiteBytes, 0, request, 12, 4);

        // bytes 16-31: entity ID
        byte[] entityIdBytes = UuidUtil.getBytesFromUUID(entityId);
        System.arraycopy(entityIdBytes, 0, request, 16, 16);

        // bytes 32-63: client key nonce
        byte[] clientKeyNonce = new byte[32];
        random.nextBytes(clientKeyNonce);
        System.arraycopy(clientKeyNonce, 0, request, 32, 32);

        // bytes 64-95: client challenge nonce
        byte[] clientChallengeNonce = new byte[32];
        random.nextBytes(clientChallengeNonce);
        System.arraycopy(clientChallengeNonce, 0, request, 64, 32);

        // create the request header - 1 byte type + 4 size
        byte[] requestHeader = new byte[5];

        // set the type
        requestHeader[0] = IPC_DATA_TYPE_DATA_PACKET;

        // set the message length
        System.arraycopy(ByteUtil.htonl(request.length), 0,
                requestHeader, 1, 4);

        // send handshake request
        dataChannel.send(ByteUtil.merge(requestHeader, request));

        // get enough of the response to validate the type and get the size
        byte[] responseHeader = dataChannel.recv(5);

        // validate the message type
        if (responseHeader[0] != IPC_DATA_TYPE_DATA_PACKET)
        {
            throw new InvalidPacketTypeException("Incorrect message type");
        }

        // get the length of the rest of the response
        int responseLength = (int)ByteUtil.ntohl(
                Arrays.copyOfRange(responseHeader, 1, 5));
        if (responseLength != 164)
        {
            throw new InvalidPayloadSizeException("Invalid response size ("
                    + responseLength + ").  Expected 164 bytes.");
        }

        // read the rest of the message
        byte[] response = dataChannel.recv(responseLength);

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

        // verify request ID
        long requestId = ByteUtil.ntohl(
                Arrays.copyOfRange(response, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE)
        {
            throw new InvalidRequestIdException(
                    "Invalid request ID in first round of handshake: " +
                            requestId);
        }

        // validate status
        long status = ByteUtil.ntohl(
                Arrays.copyOfRange(response, 8, 12));
        if (status != 0)
        {
            throw new ErrorStatusException(
                    "Bad status code in first round of handshake: " + status);
        }


        // verify protocol version
        long protocolVersion = ByteUtil.ntohl(
                Arrays.copyOfRange(response, 12, 16));
        if (protocolVersion != PROTOCOL_VERSION)
        {
            throw new UnsupportedProtocolVersionException(
                    "Unsupported protocol version: " + protocolVersion);
        }

        // verify the crypto suite version
        long cryptoSuiteVersion = ByteUtil.ntohl(
                Arrays.copyOfRange(response, 16, 20));
        if (cryptoSuiteVersion != CRYPTO_SUITE_VERSION)
        {
            throw new UnsupportedCryptoSuiteVersion(
                    "Unsupported crypto suite version: " + cryptoSuiteVersion);
        }

        // verify agent UUID
        if (!EqualsUtil.constantTimeEqual(
                UuidUtil.getBytesFromUUID(agentId),
                Arrays.copyOfRange(response, 20, 36)))
        {
            throw new AgentVerificationException("Invalid agentId.");
        }

        byte[] serverPublicKey = Arrays.copyOfRange(response, 36, 68);

        byte[] serverKeyNonce = Arrays.copyOfRange(response, 68,100);

        byte[] serverChallengeNonce = Arrays.copyOfRange(response, 100, 132);

        byte[] serverChallengeResponseHMAC = Arrays.copyOfRange(
                response, 132, 164);

        // compute shared secret
        sharedSecret = computeSharedSecret(
                entityPrivateEncKey, new EncryptionPublicKey(serverPublicKey),
                serverKeyNonce, clientKeyNonce);

        // compute HMAC from payload + client challenge nonce
        HMAC hmac = new HMAC(sharedSecret);
        byte[][] hmacParts = new byte[2][];
        hmacParts[0] = Arrays.copyOfRange(response, 0, 132);
        hmacParts[1] = clientChallengeNonce;
        byte[] computedHMAC = hmac.createHMACShort(hmacParts);

        // verify HMAC matches
        if (!EqualsUtil.constantTimeEqual(
                computedHMAC, serverChallengeResponseHMAC))
        {
            throw new MessageVerificationException(
                    "Invalid HMAC in first round of handshake.");
        }

        // success - we have a shared secret

        return serverChallengeNonce;
    }

    protected static byte[] computeSharedSecret(EncryptionPrivateKey clientPrivateKey,
          EncryptionPublicKey serverPublicKey, byte[] serverNonce,
          byte[] clientNonce)
    {
        return computeSharedSecretNative(clientPrivateKey.getRawBytes(),
                serverPublicKey.getRawBytes(), serverNonce, clientNonce);
    }


    private static native byte[] computeSharedSecretNative(byte[] clientPrivateKey,
        byte[] serverPublicKey, byte[] serverNonce, byte[] clientNonce);


    private void acknowledgeHandshake(byte[] serverChallengeNonce)
            throws IOException
    {
        HMAC hmac = new HMAC(sharedSecret);

        // send the data packet to the server
        OuterEnvelopeWriter outerEnvelopeWriter = new OuterEnvelopeWriter();
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
                sharedSecret, hmac.createHMACShort(serverChallengeNonce)));


        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
                sharedSecret, header);

        if (payloadSize != 12) // 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + ").  Expected 12 bytes.");
        }

        byte[] responseHMAC = dataChannel.recv(32);

        byte[] encryptedPayload = dataChannel.recv(payloadSize);

        // verify HMAC
        byte[][] hmacParts = new byte[2][];
        hmacParts[0] = header;
        hmacParts[1] = encryptedPayload;
        byte[] computedHMAC = hmac.createHMACShort(hmacParts);

        if (!EqualsUtil.constantTimeEqual(responseHMAC, computedHMAC))
        {
            throw new MessageVerificationException(
                    "Invalid HMAC in second round of handshake.");
        }

        // we have a valid encrypted payload, now to decrypt it
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
                sharedSecret, encryptedPayload);

        // verify request ID
        long requestId = ByteUtil.ntohl(
                Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_ACKNOWLEDGE)
        {
            throw new MessageVerificationException(
                    "Invalid request ID in second round of handshake: " +
                            requestId);
        }

        // validate status
        long status = ByteUtil.ntohl(
                Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0)
        {
            throw new MessageVerificationException(
                    "Bad status code in second round of handshake: " + status);
        }

    }


}
