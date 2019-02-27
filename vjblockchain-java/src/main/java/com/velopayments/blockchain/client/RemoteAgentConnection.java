package com.velopayments.blockchain.client;

import com.velopayments.blockchain.agentd.*;
import com.velopayments.blockchain.cert.Certificate;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;

import javax.net.SocketFactory;
import java.io.IOException;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.CompletableFuture;

public class RemoteAgentConnection implements VelochainConnection {

    private RemoteAgentChannel remoteAgentChannel;
    private ProtocolHandler protocolHandler;

    public RemoteAgentConnection(RemoteAgentConfiguration config,
                                 SocketFactory socketFactory,
                                 UUID entityId,
                                 EncryptionPrivateKey entityPrivateEncKey) {

        this.remoteAgentChannel = new RemoteAgentChannelImpl(
                config.getHost(), config.getPort(), socketFactory);
        this.protocolHandler = new ProtocolHandlerImpl(
                remoteAgentChannel, config.getAgentId(), config.getAgentPublicKey(),
                entityId, entityPrivateEncKey);
    }

    /**
     * Open a client connection.
     */
    public void connect() throws IOException {
        remoteAgentChannel.connect();

        protocolHandler.handshake();
    }

    /**
     * Close the client connection.
     */
    @Override
    public void close() throws IOException {

        remoteAgentChannel.close();
    }

    @Override
    public CompletableFuture<TransactionStatus> submit(Certificate transaction)
    throws IOException {

        try {
            protocolHandler.submit(transaction);
            return CompletableFuture.completedFuture(TransactionStatus.SUCCEEDED);
        } catch (OperationFailureException e) {
            return CompletableFuture.completedFuture(TransactionStatus.FAILED);
        }

    }

    @Override
    public UUID getLatestBlockId()
    throws IOException {

        return protocolHandler.getLatestBlockId();
    }

    @Override
    public Optional<UUID> getNextBlockId(UUID blockId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_NEXT_BLOCK_ID, blockId);
    }

    @Override
    public Optional<UUID> getPrevBlockId(UUID blockId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_PREV_BLOCK_ID, blockId);
    }

    @Override
    public Optional<UUID> getTransactionBlockId(UUID txnId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_TXN_BLOCK_ID, txnId);
    }

    @Override
    public Optional<Certificate> getBlockById(UUID blockId)
    throws IOException {

        return protocolHandler.getBlockById(blockId);
    }

    @Override
    public Optional<UUID> getBlockIdByBlockHeight(long height)
    throws IOException {

        return protocolHandler.getBlockIdByBlockHeight(height);
    }

    @Override
    public Optional<Certificate> getTransactionById(UUID txnId)
    throws IOException {

        return protocolHandler.getTransactionById(txnId);
    }

    @Override
    public Optional<UUID> getFirstTransactionIdForArtifactById(UUID artifactId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_FIRST_TXN_ID_FOR_ARTIFACT_BY_ID,
                artifactId);
    }

    @Override
    public Optional<UUID> getLastTransactionIdForArtifactById(UUID artifactId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_LAST_TXN_ID_FOR_ARTIFACT_BY_ID,
                artifactId);
    }

    @Override
    public Optional<UUID> getLastBlockIdForArtifactById(UUID artifactId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_LAST_BLOCK_ID_FOR_ARTIFACT_BY_ID,
                artifactId);
    }

    @Override
    public Optional<UUID> getPreviousTransactionIdForTransactionById(UUID txnId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_PREV_TXN_ID_FOR_TXN_BY_ID,
                txnId);
    }

    @Override
    public Optional<UUID> getNextTransactionIdForTransactionById(UUID txnId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_NEXT_TXN_ID_FOR_TXN_BY_ID,
                txnId);
    }
}
