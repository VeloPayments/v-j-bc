package com.velopayments.blockchain.client;

import com.velopayments.blockchain.agentd.*;
import com.velopayments.blockchain.cert.Certificate;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;

import javax.net.SocketFactory;
import java.io.IOException;
import java.security.SecureRandom;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.CompletableFuture;

public class RemoteAgentConnection implements VelochainConnection {

    private DataChannel dataChannel;
    private ProtocolHandler protocolHandler;

    /**
     * Note this constructor does NOT implicitly open a remote connection.
     * Use {@code RemoteAgentConnection#connect} to open a connection and
     * perform a handshake.
     * Use {@code RemoteAgentConnection#} to close the connection.
     *
     * @param config configuration data containing connection information
     * @param socketFactory factory which should produce a socket suitable
     *                      for establishing the remote connection
     * @param entityId the entity UUID for authentication
     * @param entityPrivateEncKey the private encryption key for this entity.
     */
    public RemoteAgentConnection(RemoteAgentConfiguration config,
                                 SocketFactory socketFactory,
                                 UUID entityId,
                                 EncryptionPrivateKey entityPrivateEncKey) {

        this.dataChannel = new SocketDataChannelImpl(
                config.getHost(), config.getPort(), socketFactory);
        this.protocolHandler = new ProtocolHandlerImpl(
                dataChannel, config.getAgentId(), entityId,
                entityPrivateEncKey, new OuterEnvelopeReader(),
                new OuterEnvelopeWriter(), new SecureRandom());
    }

    /**
     * Open a client connection.
     */
    public void connect() throws IOException {
        dataChannel.connect();

        protocolHandler.handshake();
    }

    @Override
    public void close() throws IOException {

        dataChannel.close();
    }

    /**
     * {@inheritDoc}
     *
     * Implementation note: this is currently a synchronous operation,
     * with agentd blocking submission on success.  Therefore, we immediately
     * return a success / failure status.  Future implementations will
     * process asynchronously and batch submissions.
     */
    @Override
    public CompletableFuture<TransactionStatus> submit(Certificate transaction)
    throws IOException {

        // TODO: BLOC-174
        /*try {
            protocolHandler.submit(transaction);
            return CompletableFuture.completedFuture(TransactionStatus.SUCCEEDED);
        } catch (OperationFailureException e) {
            return CompletableFuture.completedFuture(TransactionStatus.FAILED);
        }*/
        return null;

    }

    @Override
    public UUID getLatestBlockId()
    throws IOException {

        //return protocolHandler.getLatestBlockId();
        return null;
    }

    @Override
    public Optional<UUID> getNextBlockId(UUID blockId)
    throws IOException {

        //return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_NEXT_BLOCK_ID, blockId);
        return null;
    }

    @Override
    public Optional<UUID> getPrevBlockId(UUID blockId)
    throws IOException {

        //return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_PREV_BLOCK_ID, blockId);
        return null;
    }

    @Override
    public Optional<UUID> getTransactionBlockId(UUID txnId)
    throws IOException {

        //return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_TXN_BLOCK_ID, txnId);
        return null;
    }

    @Override
    public Optional<Certificate> getBlockById(UUID blockId)
    throws IOException {

        // TODO: BLOC-173
        //return protocolHandler.getBlockById(blockId);
        return null;
    }

    @Override
    public Optional<UUID> getBlockIdByBlockHeight(long height)
    throws IOException {

        //return protocolHandler.getBlockIdByBlockHeight(height);
        return null;
    }

    @Override
    public Optional<Certificate> getTransactionById(UUID txnId)
    throws IOException {

        //return protocolHandler.getTransactionById(txnId);
        return null;
    }

    @Override
    public Optional<UUID> getFirstTransactionIdForArtifactById(UUID artifactId)
    throws IOException {

        /*return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_FIRST_TXN_ID_FOR_ARTIFACT_BY_ID,
                artifactId);*/
        return null;
    }

    @Override
    public Optional<UUID> getLastTransactionIdForArtifactById(UUID artifactId)
    throws IOException {

        /*return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_LAST_TXN_ID_FOR_ARTIFACT_BY_ID,
                artifactId);*/
        return null;
    }

    @Override
    public Optional<UUID> getLastBlockIdForArtifactById(UUID artifactId)
    throws IOException {

        /*return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_LAST_BLOCK_ID_FOR_ARTIFACT_BY_ID,
                artifactId);*/
        return null;
    }

    @Override
    public Optional<UUID> getPreviousTransactionIdForTransactionById(UUID txnId)
    throws IOException {

        /*return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_PREV_TXN_ID_FOR_TXN_BY_ID,
                txnId);*/
        return null;
    }

    @Override
    public Optional<UUID> getNextTransactionIdForTransactionById(UUID txnId)
    throws IOException {

        /*return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_NEXT_TXN_ID_FOR_TXN_BY_ID,
                txnId);*/
        return null;
    }


}
