package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.cert.Certificate;
import com.velopayments.blockchain.cert.CertificateParser;
import com.velopayments.blockchain.cert.CertificateReader;
import com.velopayments.blockchain.cert.Field;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.util.ByteUtil;
import com.velopayments.blockchain.util.EqualsUtil;
import com.velopayments.blockchain.util.UuidUtil;

import java.io.IOException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.Optional;
import java.util.UUID;

public class ProtocolHandlerImpl implements ProtocolHandler {

    private DataChannel dataChannel;
    private UUID agentId;
    private EncryptionPublicKey agentPublicEncKey;
    private UUID entityId;
    private EncryptionPrivateKey entityPrivateEncKey;

    private SecureRandom random;
    private long requestId; // TODO: details on this

    public ProtocolHandlerImpl(DataChannel dataChannel,
                               UUID agentId,
                               EncryptionPublicKey agentPublicEncKey,
                               UUID entityId,
                               EncryptionPrivateKey entityPrivateEncKey) {

        this.dataChannel = dataChannel;
        this.agentId = agentId;
        this.agentPublicEncKey = agentPublicEncKey;
        this.entityId = entityId;
        this.entityPrivateEncKey = entityPrivateEncKey;
        this.random = new SecureRandom();
    }

    @Override
    public void handshake() throws IOException {
        byte[] serverChallengeNonce = initiateHandshake();
        acknowledgeHandshake(serverChallengeNonce);
    }

    @Override
    public Optional<Certificate> getBlockById(UUID blockId) throws IOException {

        byte[] payload = sendAndReceive(
                ApiMethod.GET_BLOCK_BY_ID, UuidUtil.getBytesFromUUID(blockId));

        // block UUID (16 bytes)
        // prev block UUID (16 bytes)
        // next block UUID (16 bytes)
        // first transaction UUID (16 bytes)
        // block height (64 bit)

        // block certificate (remaining bytes)
        if (payload.length > 72) {
            return Optional.of(
                    Certificate.fromByteArray(
                            Arrays.copyOfRange(payload, 72, payload.length)
                    ));
        }

        return Optional.empty();
    }

    @Override
    public Optional<UUID> getBlockIdByBlockHeight(long height) throws IOException {

        return UuidUtil.getOptUUIDFromBytes(
                sendAndReceive(ApiMethod.GET_BLOCK_ID_BY_BLOCK_HEIGHT,
                    ByteUtil.htonll(height)));
    }

    @Override
    public UUID getLatestBlockId() throws IOException {
        return UuidUtil.getUUIDFromBytes(
            sendAndReceive(ApiMethod.GET_LATEST_BLOCK_ID, new byte[0])
        );
    }

    @Override
    public Optional<Certificate> getTransactionById(UUID transactionId) throws IOException {

        byte[] payload = sendAndReceive(
                ApiMethod.GET_TXN_BY_ID, UuidUtil.getBytesFromUUID(transactionId));

        // transaction uuid (16 bytes)
        // prev transaction uuid (16 bytes)
        // next transaction uuid (16 bytes)
        // artifact uuid (16 bytes)

        // transaction certificate (remaining bytes)
        if (payload.length > 64) {
            return Optional.of(
                    Certificate.fromByteArray(
                            Arrays.copyOfRange(payload, 64, payload.length)
                    ));
        }


        return Optional.empty();
    }


    @Override
    public Optional<UUID> sendAndReceiveUUID(ApiMethod apiMethod, UUID uuid)
    throws IOException {

        return UuidUtil.getOptUUIDFromBytes(
            sendAndReceive(apiMethod, UuidUtil.getBytesFromUUID(uuid))
        );
    }

    @Override
    public void submit(Certificate transaction) throws IOException {

        byte[] certficateBytes = transaction.toByteArray();

        byte[] payload = new byte[16 + 16 + certficateBytes.length];

        // transaction id (16 bytes)
        CertificateReader reader = new CertificateReader(new CertificateParser(transaction));
        UUID transactionId = reader.getFirst(Field.CERTIFICATE_ID).asUUID();
        System.arraycopy(UuidUtil.getBytesFromUUID(transactionId), 0, payload, 0, 16);

        // artifact id (16 bytes)
        UUID artifactId = reader.getFirst(Field.ARTIFACT_ID).asUUID();
        System.arraycopy(UuidUtil.getBytesFromUUID(artifactId), 0, payload, 16, 16);

        // transaction certificate (remaining bytes)
        System.arraycopy(certficateBytes, 0, payload, 32, certficateBytes.length);

        sendAndReceive(ApiMethod.SUBMIT, payload);
    }


    /**
     * Initiate a handshake with the remote server
     *
     * @return response payload
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
        byte[] protocolVersionBytes = ByteUtil.htonl(1);
        System.arraycopy(protocolVersionBytes, 0, request,  8, 4);

        // bytes 12-15: crypto suite
        byte[] cryptoSuiteBytes = ByteUtil.htonl(1);
        System.arraycopy(cryptoSuiteBytes, 0, request, 12, 4);

        // bytes 16-31: entity ID
        byte[] entityIdBytes = UuidUtil.getBytesFromUUID(entityId);
        System.arraycopy(entityIdBytes, 0, request, 16, 16);

        // bytes 32-63: client key nonce
        byte clientKeyNonce[] = new byte[32];
        random.nextBytes(clientKeyNonce);
        System.arraycopy(clientKeyNonce, 0, request, 32, 32);

        // bytes 64-95: client challenge nonce
        byte clientChallengeNonce[] = new byte[32];
        random.nextBytes(clientChallengeNonce);
        System.arraycopy(clientChallengeNonce, 0, request, 64, 32);


        // send handshake request
        dataChannel.send(request);

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

        byte[] response = dataChannel.recv();

        // TODO: verify protocol version (should be 1)

        // TODO: verify the crypto suite version (should be 1)

        // verify agent UUID
        if (!EqualsUtil.constantTimeEqual(
                UuidUtil.getBytesFromUUID(agentId),
                Arrays.copyOfRange(response, 20, 36)))
        {
            throw new AgentVerificationException("Invalid agentId.");
        }

        // TODO: get server public key

        byte[] serverKeyNonce = Arrays.copyOfRange(response, 68,100);

        byte[] serverChallengeNonce = Arrays.copyOfRange(response, 100, 132);

        byte[] serverChallengeResponseHMAC = Arrays.copyOfRange(
                response, 132, 164);

        // TODO: compute shared secret

        // TODO: compute HMAC from payload + client challenge nonce

        // TODO: verify HMAC matches

        return serverChallengeNonce;
    }

    private void acknowledgeHandshake(byte[] serverChallengeNonce) {

        // request
        //   HMAC: server challenge nonce / session key (32 bytes)

        byte[] request = new byte[32];
        // TODO: construct request


        // TODO: send and receive


        // TODO: verify response
    }

    private byte[] sendAndReceive(ApiMethod apiMethod, byte[] reqPayload)
    throws IOException {

        // wrap in inner envelope
        byte[] reqInner = Envelope.wrapInner(apiMethod, requestId, reqPayload);

        // wrap in outer envelope
        byte[] reqOuter = Envelope.wrapOuter(
                agentPublicEncKey.getRawBytes(), reqInner);

        // send down channel
        dataChannel.send(reqOuter);

        // receive response
        byte[] response = dataChannel.recv();

        // unwrap outer envelope
        byte[] respInnerBytes = Envelope.unwrapOuter(
                entityPrivateEncKey.getRawBytes(), response);
        // TODO: maybe there should be an OuterEnvelopeResponse, with HMAC verification
        // done here

        // unwrap inner envelope
        InnerEnvelopeResponse respInner = Envelope.unwrapInner(respInnerBytes);

        // verify the request ID and apiMethod
        if (respInner.getApiMethodId() != apiMethod.getValue()) {
            throw new InvalidResponseException("Invalid ApiMethod - expected: "
                    + apiMethod.getValue()
                    + ", actual: " + respInner.getApiMethodId());
        }

        // TODO: request ID
        /*if (respInner.getRequestId() != requestId) {
            throw new InvalidResponseException("Invalid requestId - expected: " + requestId
                    + ", actual: " + respInner.getRequestId());
        }*/

        // the status code should be 0 for success
        if (respInner.getStatus() != 0) {
            throw new OperationFailureException(
                    "The remote operation failed.  ApiMethod: " + apiMethod);
        }

        return respInner.getPayload();
    }

}
