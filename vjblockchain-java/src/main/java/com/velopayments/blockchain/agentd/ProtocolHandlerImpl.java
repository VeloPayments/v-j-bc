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

    public static final int HANDSHAKE_INITIATE_RESPONSE_SIZE = 112;

    private RemoteAgentChannel remoteAgentChannel;
    private UUID agentId;
    private EncryptionPublicKey agentPublicEncKey;
    private UUID entityId;
    private EncryptionPrivateKey entityPrivateEncKey;

    private SecureRandom random;
    private long requestId; // TODO: details on this

    public ProtocolHandlerImpl(RemoteAgentChannel remoteAgentChannel,
                               UUID agentId,
                               EncryptionPublicKey agentPublicEncKey,
                               UUID entityId,
                               EncryptionPrivateKey entityPrivateEncKey) {

        this.remoteAgentChannel = remoteAgentChannel;
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
                    ByteUtil.longToBytes(height, true)));
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


    private byte[] initiateHandshake() throws IOException {

        // request
        //   entity ID (16 bytes)
        //   client key nonce (32 bytes)
        //   client challenge nonce (32 bytes)

        byte[] request = new byte[80];

        byte[] entityIdBytes = UuidUtil.getBytesFromUUID(entityId);
        System.arraycopy(entityIdBytes, 0, request, 0, 16);

        byte clientKeyNonce[] = new byte[32];
        random.nextBytes(clientKeyNonce);
        System.arraycopy(clientKeyNonce, 0, request, 16, 32);

        byte clientChallengeNonce[] = new byte[32];
        random.nextBytes(clientChallengeNonce);
        System.arraycopy(clientChallengeNonce, 0, request, 48, 32);


        // send handshake request
        remoteAgentChannel.send(request);

        // receive response
        //   agentId (16 bytes)
        //   server key nonce (32 bytes)
        //   server challenge nonce (32 bytes)
        //   server challenge / response (32 bytes)
        byte[] response = remoteAgentChannel.recv(HANDSHAKE_INITIATE_RESPONSE_SIZE);

        // verify agent UUID
        if (!EqualsUtil.constantTimeEqual(
                UuidUtil.getBytesFromUUID(agentId),
                Arrays.copyOfRange(response, 0, 16)))
        {
            throw new AgentVerificationException("Invalid agentId.");
        }

        byte[] serverKeyNonce = Arrays.copyOfRange(response, 16,48);

        byte[] serverChallengeNonce = Arrays.copyOfRange(response, 48, 80);

        byte[] serverChallengeResponse = Arrays.copyOfRange(response, 80, 112);

        // TODO - verify challenge / response

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
        byte[] reqInner = Envelope.wrapInner(
                apiMethod, requestId, reqPayload);

        // wrap in outer envelope
        byte[] reqOuter = Envelope.wrapOuter(
                MessageType.AUTHENTICATED, agentPublicEncKey.getRawBytes(), reqInner);

        // send down channel
        remoteAgentChannel.send(reqOuter);

        // receive the response.  This needs to be done in two steps as we don't
        // always know the size of the response in advance.

        // receive message type and payload size
        byte[] msgTypeAndSize = remoteAgentChannel.recv(5);
        int payloadSize = (int) ByteUtil.bytesToLong(
                Arrays.copyOfRange(msgTypeAndSize, 1, 5),true);

        // receive payload and HMAC
        byte[] payloadAndHmac = remoteAgentChannel.recv(payloadSize + 32);

        // unwrap outer envelope
        byte[] respInnerBytes = Envelope.unwrapOuter(
                entityPrivateEncKey.getRawBytes(),
                ByteUtil.merge(msgTypeAndSize, payloadAndHmac));

        // unwrap inner envelope
        InnerEnvelopeResponse respInner = Envelope.unwrapInner(respInnerBytes);

        // verify the request ID and apiMethod
        if (respInner.getApiMethodId() != apiMethod.getValue()) {
            throw new InvalidResponseException("Invalid ApiMethod - expected: "
                    + apiMethod.getValue() + ", actual: " + respInner.getApiMethodId());
        }

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
