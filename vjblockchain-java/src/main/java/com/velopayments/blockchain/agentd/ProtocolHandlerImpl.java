package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.cert.*;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.crypt.HMAC;
import com.velopayments.blockchain.init.Initializer;
import com.velopayments.blockchain.util.ByteUtil;
import com.velopayments.blockchain.util.EqualsUtil;
import com.velopayments.blockchain.util.UuidUtil;

import java.io.IOException;
import java.net.ConnectException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.Optional;
import java.util.UUID;

public class ProtocolHandlerImpl implements ProtocolHandler {

    public static final long PROTOCOL_VERSION    = 1L;
    public static final long CRYPTO_SUITE_VERSION = 1L;

    public static final byte IPC_DATA_TYPE_DATA_PACKET   = 0x20;

    public static final long UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE    =  0L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_ACKNOWLEDGE =  1L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_LATEST_BLOCK_ID_GET   =  2L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_SUBMIT    =  3L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_BLOCK_BY_ID_GET       =  4L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_NEXT     =  5L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_PREV     =  6L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_BY_HEIGHT_GET=  7L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_BY_ID_GET = 16L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_NEXT
                                                                          = 17L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_PREV
                                                                          = 18L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_BLOCK_ID
                                                                          = 19L;
    public static final long
        UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_FIRST_TXN_BY_ID_GET               = 32L;
    public static final long
        UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_LAST_TXN_BY_ID_GET                = 33L;
    public static final long UNAUTH_PROTOCOL_REQ_ID_STATUS_GET         = 40960L;

    public static final int PROTOCOL_ERROR_MALFORMED_REQUEST       = 0x080A000E;
    public static final int PROTOCOL_ERROR_UNAUTHORIZED            = 0x080A0010;

    private DataChannel dataChannel;
    private UUID agentId;
    private UUID entityId;
    private EncryptionPublicKey agentPublicEncKey;
    private EncryptionPrivateKey entityPrivateEncKey;
    private OuterEnvelopeReader outerEnvelopeReader;
    private OuterEnvelopeWriter outerEnvelopeWriter;
    private SecureRandom random;

    private byte[] sharedSecret;

    public ProtocolHandlerImpl(
            DataChannel dataChannel, UUID agentId, UUID entityId,
            EncryptionPublicKey agentPublicEncKey,
            EncryptionPrivateKey entityPrivateEncKey,
            OuterEnvelopeReader outerEnvelopeReader,
            OuterEnvelopeWriter outerEnvelopeWriter,
            SecureRandom random)
    {
        this.dataChannel = dataChannel;
        this.agentId = agentId;
        this.entityId = entityId;
        this.agentPublicEncKey = agentPublicEncKey;
        this.entityPrivateEncKey = entityPrivateEncKey;
        this.outerEnvelopeReader = outerEnvelopeReader;
        this.outerEnvelopeWriter = outerEnvelopeWriter;
        this.random = random;
    }

    @Override
    public void handshake() throws IOException {
        byte[] serverChallengeNonce = initiateHandshake();
        acknowledgeHandshake(serverChallengeNonce);
    }

    @Override
    public void submit(Certificate transaction) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        CertificateReader reader =
            new CertificateReader(new CertificateParser(transaction));
        UUID transactionId = null;
        UUID artifactId = null;

        //get the transaction id.
        try {
            transactionId = reader.getFirst(Field.CERTIFICATE_ID).asUUID();
        } catch (MissingFieldException e) {
            //TODO - we should bubble up this exception instead of masking it.
            throw new IOException("Missing transaction id.");
        } catch (FieldConversionException e) {
            //TODO - we should bubble up this exception instead of masking it.
            throw new IOException("Invalid transaction id.");
        }

        //get the artifact id.
        try {
            artifactId = reader.getFirst(Field.ARTIFACT_ID).asUUID();
        } catch (MissingFieldException e) {
            //TODO - we should bubble up this exception instead of masking it.
            throw new IOException("Missing artifact id.");
        } catch (FieldConversionException e) {
            //TODO - we should bubble up this exception instead of masking it.
            throw new IOException("Invalid artifact id.");
        }

        // write the submit request
        writeSubmitRequest(transactionId, artifactId, transaction);

        // read the submit response
        long status = readSubmitResponse();

        // TODO - send the status back to the caller
    }

    @Override
    public UUID getLatestBlockId() throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetLatestBlockIdRequest();

        return readGetLatestBlockIdResponse();
    }

    @Override
    public Optional<Certificate> getBlockById(UUID blockId) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetBlockByIdRequest(blockId);

        return readGetBlockByIdResponse();
    }

    @Override
    public Optional<UUID> getNextBlockId(UUID blockId) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetNextBlockIdRequest(blockId);

        return readGetNextBlockIdResponse();
    }

    @Override
    public Optional<UUID> getPrevBlockId(UUID blockId) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetPrevBlockIdRequest(blockId);

        return readGetPrevBlockIdResponse();
    }

    @Override
    public Optional<Certificate>
    getTransactionById(UUID txnId) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetTransactionByIdRequest(txnId);

        return readGetTransactionByIdResponse();
    }

    @Override
    public Optional<UUID> getTransactionNextId(UUID txnId) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetTransactionNextIdRequest(txnId);

        return readGetTransactionNextIdResponse();
    }

    @Override
    public Optional<UUID> getTransactionPreviousId(UUID txnId)
    throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetTransactionPreviousIdRequest(txnId);

        return readGetTransactionPreviousIdResponse();
    }

    @Override
    public Optional<UUID> getTransactionBlockId(UUID txnId) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetTransactionBlockIdRequest(txnId);

        return readGetTransactionBlockIdResponse();
    }

    @Override
    public Optional<UUID>
    getArtifactFirstTxnId(UUID artifactId) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetArtifactFirstTxnIdRequest(artifactId);

        return readGetArtifactGetFirstTxnIdRequest();
    }

    @Override
    public Optional<UUID>
    getArtifactLastTxnId(UUID artifactId) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetArtifactLastTxnIdRequest(artifactId);

        return readGetArtifactGetLastTxnIdRequest();
    }

    @Override
    public Optional<UUID>
    getBlockIdByBlockHeight(long height) throws IOException {

        /* sharedSecret is populated when connect() successfully completes. */
        if (sharedSecret == null) {
            throw new ConnectException("Not connected. Call connect() first.");
        }

        writeGetBlockIdByBlockHeight(height);

        return readGetBlockIdByBlockHeight();
    }

    @Override
    public int
    getConnectionStatus() throws IOException {

        writeGetConnectionStatus();

        return readGetConnectionStatus();
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
            handleHandshakeInitiateError(responseLength);
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
            UUID badId = UuidUtil.getUUIDFromBytes(
                Arrays.copyOfRange(response, 20, 36));

            throw new AgentVerificationException(
                "Invalid agentId (expected " + agentId + " and got " + badId
                + ")");
        }

        byte[] serverPublicKey = Arrays.copyOfRange(response, 36, 68);

        // verify agent public encryption key
        if (!EqualsUtil.constantTimeEqual(
                agentPublicEncKey.getRawBytes(),
                serverPublicKey))
        {
            throw new AgentVerificationException(
                "Server public key does not match expected public key. " +
                "Possible MITM attack!");
        }

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

    private void handleHandshakeInitiateError(int responseLength)
            throws IOException {
        if (responseLength < 12) {
            throw new InvalidPayloadSizeException("Invalid response size ("
                    + responseLength + ").  Expected 164 bytes.");
        } else {
            byte[] response = dataChannel.recv(responseLength);

            //get the status code.
            long status = ByteUtil.ntohl(
                Arrays.copyOfRange(response, 8, 12));

            //decode it
            switch ((int)status)
            {
                case PROTOCOL_ERROR_MALFORMED_REQUEST:
                    throw new AgentVerificationException(
                        "Malformed client request.");
                case PROTOCOL_ERROR_UNAUTHORIZED:
                    throw new AgentVerificationException(
                        "Client not authorized.");
                default:
                    throw new AgentVerificationException(
                        "Client not authorized.");
            }
        }
    }

    protected static byte[] computeSharedSecret(EncryptionPrivateKey clientPrivateKey,
          EncryptionPublicKey serverPublicKey, byte[] serverNonce,
          byte[] clientNonce)
    {
        return computeSharedSecretNative(
                Initializer.getInstance(), clientPrivateKey.getRawBytes(),
                serverPublicKey.getRawBytes(), serverNonce, clientNonce);
    }

    private static native byte[] computeSharedSecretNative(
        long nativeInst, byte[] clientPrivateKey, byte[] serverPublicKey,
        byte[] serverNonce, byte[] clientNonce);

    private void acknowledgeHandshake(byte[] serverChallengeNonce)
            throws IOException
    {
        HMAC hmac = new HMAC(sharedSecret);

        // send the data packet to the server
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

        byte[] encryptedPayloadTail = dataChannel.recv(payloadSize);

        //the payload is the MAC and the payload bytes.
        byte[] encryptedPayload =
            new byte[responseHMAC.length + encryptedPayloadTail.length];
        System.arraycopy(
            responseHMAC, 0, encryptedPayload, 0, responseHMAC.length);
        System.arraycopy(
            encryptedPayloadTail, 0, encryptedPayload, responseHMAC.length,
            encryptedPayloadTail.length);

        // we have a valid encrypted payload, now to decrypt it
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
                sharedSecret, header, encryptedPayload);

        // verify request ID
        long requestId = ByteUtil.ntohl(
                Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_ACKNOWLEDGE)
        {
            throw new InvalidRequestIdException(
                    "Invalid request ID in second round of handshake: " +
                            requestId);
        }

        // validate status
        long status = ByteUtil.ntohl(
                Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0)
        {
            throw new ErrorStatusException(
                    "Bad status code in second round of handshake: " + status);
        }

    }

    private void writeSubmitRequest(
                        UUID transactionId, UUID artifactId,
                        Certificate transaction) throws IOException {

        /* | Transaction submit request packet.                            | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_SUBMIT      |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | transaction_id                                 |  16 bytes    | */
        /* | artifact_id                                    |  16 bytes    | */
        /* | transaction certificate                        |   N bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] certificateBytes = transaction.toByteArray();
        byte[] request = new byte[certificateBytes.length + 40];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_SUBMIT
        byte[] reqBytes =
            ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_SUBMIT);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: transaction id
        byte[] transactionIdBytes = UuidUtil.getBytesFromUUID(transactionId);
        System.arraycopy(transactionIdBytes, 0, request, 8, 16);

        // bytes 24-39: artifact id
        byte[] artifactIdBytes = UuidUtil.getBytesFromUUID(artifactId);
        System.arraycopy(artifactIdBytes, 0, request, 24, 16);

        // transaction certificate
        System.arraycopy(
            certificateBytes, 0, request, 40, certificateBytes.length);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private long readSubmitResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
                sharedSecret, header);

        if (payloadSize != 12) // 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected 12 bytes.");
        }

        byte[] responseHMAC = dataChannel.recv(32);

        byte[] encryptedPayloadTail = dataChannel.recv(payloadSize);

        //the payload is the MAC and the payload bytes.
        byte[] encryptedPayload =
            new byte[responseHMAC.length + encryptedPayloadTail.length];
        System.arraycopy(
            responseHMAC, 0, encryptedPayload, 0, responseHMAC.length);
        System.arraycopy(
            encryptedPayloadTail, 0, encryptedPayload, responseHMAC.length,
            encryptedPayloadTail.length);

        // we have a valid encrypted payload, now to decrypt it
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
                sharedSecret, header, encryptedPayload);

        /* | Transaction submit response packet.                           | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_SUBMIT      |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
                Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_SUBMIT)
        {
            throw new InvalidRequestIdException(
                    "Invalid request ID: " + requestId);
        }

        // get status.
        long status = ByteUtil.ntohl(
                Arrays.copyOfRange(decryptedPayload, 4, 8));

        return status;
    }

    private void writeGetLatestBlockIdRequest() throws IOException {

        /* | Get Latest Block ID request packet.                           | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_LATEST_BLOCK_ID_GET     |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[8];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_LATEST_BLOCK_ID_GET
        byte[] reqBytes =
            ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_LATEST_BLOCK_ID_GET);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private UUID readGetLatestBlockIdResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
                sharedSecret, header);

        if (payloadSize != 12 && payloadSize != 28) // 3 x 4 bytes + 16 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected either 12 or 28 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
                sharedSecret, header, encryptedPayload);

        /* | Get Latest Block ID response packet.                          | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_LATEST_BLOCK_ID_GET     |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | UUID                                           |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
                Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_LATEST_BLOCK_ID_GET) {
            throw new InvalidRequestIdException(
                    "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
                Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            // TODO - replace with better exception.
            throw new IOException("bad status code in call.");
        }

        // check size
        if (payloadSize != 28) {
            throw new InvalidPayloadSizeException("Expected UUID in payload.");
        }

        // return the UUID.
        byte[] uuidBytes = new byte[16];
        System.arraycopy(decryptedPayload, 12, uuidBytes, 0, 16);
        return UuidUtil.getUUIDFromBytes(uuidBytes);
    }


    private void writeGetBlockByIdRequest(UUID blockId) throws IOException {

        /* | Get Block by ID request packet.                               | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_BLOCK_BY_ID_GET         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | block_id                                       |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_BLOCK_BY_ID_GET
        byte[] reqBytes =
            ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_BLOCK_BY_ID_GET);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: block id
        byte[] blockIdBytes = UuidUtil.getBytesFromUUID(blockId);
        System.arraycopy(blockIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<Certificate>
    readGetBlockByIdResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Block by ID response packet.                              | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_BLOCK_BY_ID_GET         |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | BLOCK_UUID                                     |  16 bytes    | */
        /* | PREVIOUS_BLOCK_UUID                            |  16 bytes    | */
        /* | NEXT_BLOCK_UUID                                |  16 bytes    | */
        /* | FIRST_TRANSACTION_ID                           |  16 bytes    | */
        /* | NET_BLOCK_HEIGHT                               |   8 bytes    | */
        /* | NET_CERT_SIZE                                  |   8 bytes    | */
        /* | block certificate                              |   N bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_BLOCK_BY_ID_GET) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 4 + 8 * 2;
        if (payloadSize <= payloadHeaderSize) {
            return Optional.empty();
        }

        // get certificate
        byte[] certificate = Arrays.copyOfRange(
            decryptedPayload, payloadHeaderSize,
            payloadSize);
        return Optional.of(Certificate.fromByteArray(certificate));
    }

    private void writeGetNextBlockIdRequest(UUID blockId) throws IOException {

        /* | Get Next Block by ID request packet.                          | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_NEXT       |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | block_id                                       |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_NEXT
        byte[] reqBytes =
            ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_NEXT);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: block id
        byte[] blockIdBytes = UuidUtil.getBytesFromUUID(blockId);
        System.arraycopy(blockIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<UUID>
    readGetNextBlockIdResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Block by ID response packet.                              | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_NEXT       |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | BLOCK_UUID                                     |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_NEXT) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 1;
        if (payloadSize < payloadHeaderSize) {
            return Optional.empty();
        }

        // read next block id.
        byte[] nextBlockIdBytes = Arrays.copyOfRange(
            decryptedPayload, 3 * 4, 3 * 4 + 16);
        return Optional.of(UuidUtil.getUUIDFromBytes(nextBlockIdBytes));
    }

    private void writeGetPrevBlockIdRequest(UUID blockId) throws IOException {

        /* | Get Prev Block by ID request packet.                          | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_PREV       |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | block_id                                       |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_PREV
        byte[] reqBytes =
            ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_PREV);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: block id
        byte[] blockIdBytes = UuidUtil.getBytesFromUUID(blockId);
        System.arraycopy(blockIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<UUID>
    readGetPrevBlockIdResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Block by ID response packet.                              | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_PREV       |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | BLOCK_UUID                                     |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_GET_PREV) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 1;
        if (payloadSize < payloadHeaderSize) {
            return Optional.empty();
        }

        // read prev block id.
        byte[] prevBlockIdBytes = Arrays.copyOfRange(
            decryptedPayload, 3 * 4, 3 * 4 + 16);
        return Optional.of(UuidUtil.getUUIDFromBytes(prevBlockIdBytes));
    }

    private void writeGetTransactionByIdRequest(UUID txnId)
    throws IOException {

        /* | Get Transaction by ID request packet.                         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_BY_ID_GET   |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | txn_id                                         |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_BY_ID_GET
        byte[] reqBytes =
            ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_BY_ID_GET);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: txn id
        byte[] txnIdBytes = UuidUtil.getBytesFromUUID(txnId);
        System.arraycopy(txnIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<Certificate>
    readGetTransactionByIdResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Transaction by ID response packet.                        | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_BY_ID_GET   |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | TXN_UUID                                       |  16 bytes    | */
        /* | PREVIOUS_TXN_UUID                              |  16 bytes    | */
        /* | NEXT_TXN_UUID                                  |  16 bytes    | */
        /* | ARTIFACT_ID                                    |  16 bytes    | */
        /* | BLOCK_ID                                       |  16 bytes    | */
        /* | NET_CERT_SIZE                                  |   8 bytes    | */
        /* | NET_TXN_STATE                                  |   4 bytes    | */
        /* | txn certificate                                |   N bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_BY_ID_GET) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 5 + 8 * 1 + 4 * 1;
        if (payloadSize <= payloadHeaderSize) {
            return Optional.empty();
        }

        // get certificate
        byte[] certificate = Arrays.copyOfRange(
            decryptedPayload, payloadHeaderSize,
            payloadSize);
        return Optional.of(Certificate.fromByteArray(certificate));
    }

    private void writeGetTransactionNextIdRequest(UUID txnId)
    throws IOException {

        /* | Get Transaction next ID request packet.                       | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_NEXT |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | txn_id                                         |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_NEXT
        byte[] reqBytes =
            ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_NEXT);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: txn id
        byte[] txnIdBytes = UuidUtil.getBytesFromUUID(txnId);
        System.arraycopy(txnIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<UUID>
    readGetTransactionNextIdResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Transaction Next ID response packet.                      | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_NEXT |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | NEXT_TXN_UUID                                  |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_NEXT) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 1;
        if (payloadSize < payloadHeaderSize) {
            return Optional.empty();
        }

        // get uuid
        byte[] nextTxnIdBytes = Arrays.copyOfRange(
            decryptedPayload, 3 * 4, 3 * 4 + 16);
        return Optional.of(UuidUtil.getUUIDFromBytes(nextTxnIdBytes));
    }

    private void writeGetTransactionPreviousIdRequest(UUID txnId)
    throws IOException {

        /* | Get Transaction prev ID request packet.                       | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_PREV |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | txn_id                                         |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_PREV
        byte[] reqBytes =
            ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_PREV);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: txn id
        byte[] txnIdBytes = UuidUtil.getBytesFromUUID(txnId);
        System.arraycopy(txnIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<UUID>
    readGetTransactionPreviousIdResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Transaction Prev ID response packet.                      | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_PREV |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | PREV_TXN_UUID                                  |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_PREV) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 1;
        if (payloadSize < payloadHeaderSize) {
            return Optional.empty();
        }

        // get uuid
        byte[] prevTxnIdBytes = Arrays.copyOfRange(
            decryptedPayload, 3 * 4, 3 * 4 + 16);
        return Optional.of(UuidUtil.getUUIDFromBytes(prevTxnIdBytes));
    }

    private void writeGetTransactionBlockIdRequest(UUID txnId)
    throws IOException {

        /* | Get Transaction block ID request packet.                      | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_BLOCK_ID | 4 bytes  | */
        /* | offset                                         |   4 bytes    | */
        /* | txn_id                                         |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_BLOCK_ID
        byte[] reqBytes =
            ByteUtil.htonl(
                (int)UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_BLOCK_ID);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: txn id
        byte[] txnIdBytes = UuidUtil.getBytesFromUUID(txnId);
        System.arraycopy(txnIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<UUID>
    readGetTransactionBlockIdResponse() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Transaction Block ID response packet.                     | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_BLOCK_ID | 4 bytes  | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | PREV_TXN_UUID                                  |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_ID_GET_BLOCK_ID) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 1;
        if (payloadSize < payloadHeaderSize) {
            return Optional.empty();
        }

        // get uuid
        byte[] blockTxnIdBytes = Arrays.copyOfRange(
            decryptedPayload, 3 * 4, 3 * 4 + 16);
        return Optional.of(UuidUtil.getUUIDFromBytes(blockTxnIdBytes));
    }

    private void writeGetArtifactFirstTxnIdRequest(UUID artifactId)
    throws IOException {

        /* | Get Artifact first transaction ID request packet.             | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_FIRST_TXN_BY_ID_GET | 4 bytes | */
        /* | offset                                         |   4 bytes    | */
        /* | artifact_id                                    |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_FIRST_TXN_BY_ID_GET
        byte[] reqBytes =
            ByteUtil.htonl(
                (int)UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_FIRST_TXN_BY_ID_GET);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: artifact id
        byte[] artifactIdBytes = UuidUtil.getBytesFromUUID(artifactId);
        System.arraycopy(artifactIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<UUID>
    readGetArtifactGetFirstTxnIdRequest() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Artifact First Transaction ID response packet.            | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_FIRST_TXN_BY_ID_GET | 4 bytes | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | FIRST_TXN_ID                                   |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_FIRST_TXN_BY_ID_GET) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 1;
        if (payloadSize < payloadHeaderSize) {
            return Optional.empty();
        }

        // get uuid
        byte[] firstTxnIdBytes = Arrays.copyOfRange(
            decryptedPayload, 3 * 4, 3 * 4 + 16);
        return Optional.of(UuidUtil.getUUIDFromBytes(firstTxnIdBytes));
    }

    private void writeGetArtifactLastTxnIdRequest(UUID artifactId)
    throws IOException {

        /* | Get Artifact first transaction ID request packet.             | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_LAST_TXN_BY_ID_GET | 4 bytes  | */
        /* | offset                                         |   4 bytes    | */
        /* | artifact_id                                    |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[24];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_LAST_TXN_BY_ID_GET
        byte[] reqBytes =
            ByteUtil.htonl(
                (int)UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_LAST_TXN_BY_ID_GET);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-23: artifact id
        byte[] artifactIdBytes = UuidUtil.getBytesFromUUID(artifactId);
        System.arraycopy(artifactIdBytes, 0, request, 8, 16);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<UUID>
    readGetArtifactGetLastTxnIdRequest() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Artifact Last Transaction ID response packet.             | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_LAST_TXN_BY_ID_GET | 4 bytes  | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | LAST_TXN_ID                                    |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_ARTIFACT_LAST_TXN_BY_ID_GET) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 1;
        if (payloadSize < payloadHeaderSize) {
            return Optional.empty();
        }

        // get uuid
        byte[] lastTxnIdBytes = Arrays.copyOfRange(
            decryptedPayload, 3 * 4, 3 * 4 + 16);
        return Optional.of(UuidUtil.getUUIDFromBytes(lastTxnIdBytes));
    }

    private void writeGetBlockIdByBlockHeight(long height) throws IOException {

        /* | Get Block ID by Block Height request packet.                  | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_BY_HEIGHT_GET  |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | block_height                                   |   8 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[16];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_BY_HEIGHT_GET
        byte[] reqBytes =
            ByteUtil.htonl(
                (int)UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_BY_HEIGHT_GET);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // bytes 8-15: block height
        byte[] blockHeightBytes = ByteUtil.htonll(height);
        System.arraycopy(blockHeightBytes, 0, request, 8, 8);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private Optional<UUID>
    readGetBlockIdByBlockHeight() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Block ID by Block Height response packet.                 | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_BY_HEIGHT_GET  |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | BLOCK_ID                                       |  16 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_BLOCK_ID_BY_HEIGHT_GET) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* TODO - there might be a different error than not found. */
            return Optional.empty();
        }

        // check size
        int payloadHeaderSize =
            3 * 4 + 16 * 1;
        if (payloadSize < payloadHeaderSize) {
            return Optional.empty();
        }

        // get uuid
        byte[] blockIdBytes = Arrays.copyOfRange(
            decryptedPayload, 3 * 4, 3 * 4 + 16);
        return Optional.of(UuidUtil.getUUIDFromBytes(blockIdBytes));
    }

    private void writeGetConnectionStatus() throws IOException {

        /* | Get Connection Status request packet.                         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_STATUS_GET              |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        byte[] request = new byte[8];

        // bytes 0-3: UNAUTH_PROTOCOL_REQ_ID_STATUS_GET
        byte[] reqBytes =
            ByteUtil.htonl(
                (int)UNAUTH_PROTOCOL_REQ_ID_STATUS_GET);
        System.arraycopy(reqBytes, 0, request, 0, 4);

        // bytes 4-7: offset
        byte[] offsetBytes = ByteUtil.htonl(0);
        System.arraycopy(offsetBytes, 0, request, 4, 4);

        // send the request to the server
        dataChannel.send(outerEnvelopeWriter.encryptPayload(
            sharedSecret, request));
    }

    private int
    readGetConnectionStatus() throws IOException {

        // receive the header: type, size
        byte[] header = dataChannel.recv(5);
        int payloadSize = outerEnvelopeReader.decryptHeader(
            sharedSecret, header);

        if (payloadSize < 12) // at least 3 x 4 bytes
        {
            throw new InvalidPayloadSizeException("Invalid payload size ("
                + payloadSize + "). Expected at least 12 bytes.");
        }

        // read the HMAC and the payload.
        byte[] encryptedPayload = dataChannel.recv(payloadSize + 32);

        // decrypt the payload
        byte[] decryptedPayload = outerEnvelopeReader.decryptPayload(
            sharedSecret, header, encryptedPayload);

        /* | Get Connection Status response packet.                        | */
        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_STATUS_GET              |   4 bytes    | */
        /* | status                                         |   4 bytes    | */
        /* | offset                                         |   4 bytes    | */
        /* | ---------------------------------------------- | ------------ | */

        // verify request ID
        long requestId = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 0, 4));
        if (requestId != UNAUTH_PROTOCOL_REQ_ID_STATUS_GET) {
            throw new InvalidRequestIdException(
                "Invalid request ID: " + requestId);
        }

        // get status
        long status = ByteUtil.ntohl(
            Arrays.copyOfRange(decryptedPayload, 4, 8));
        if (status != 0) {
            /* doesn't matter, but make it non-zero. */
            return 1;
        }

        /* success. */
        return 0;
    }
}
