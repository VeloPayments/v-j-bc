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

        try {
            protocolHandler.submit(transaction);
            /* sleep for 250 milliseconds before completing. */
            Thread.sleep(250);
            return
                CompletableFuture.completedFuture(
                    TransactionStatus.SUCCEEDED);
        } catch (InterruptedException e) {
            return
                CompletableFuture.completedFuture(
                    TransactionStatus.SUCCEEDED);
        } catch (Exception e) {
            return
                CompletableFuture.completedFuture(
                    TransactionStatus.FAILED);
        }
    }

    @Override
    public CompletableFuture<UUID>getLatestBlockId()
    throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getLatestBlockId());
    }

    @Override
    public CompletableFuture<Optional<UUID>> getNextBlockId(UUID blockId)
    throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getNextBlockId(blockId));
    }

    @Override
    public CompletableFuture<Optional<UUID>> getPrevBlockId(UUID blockId)
    throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getPrevBlockId(blockId));
    }

    @Override
    public CompletableFuture<Optional<UUID>> getTransactionBlockId(UUID txnId)
    throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getTransactionBlockId(txnId));
    }

    @Override
    public CompletableFuture<Optional<Certificate>> getBlockById(UUID blockId)
    throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getBlockById(blockId));
    }

    @Override
    public CompletableFuture<Optional<UUID>>
    getBlockIdByBlockHeight(long height) throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getBlockIdByBlockHeight(height));
    }

    @Override
    public CompletableFuture<Optional<Certificate>>
    getTransactionById(UUID txnId) throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getTransactionById(txnId));
    }

    @Override
    public CompletableFuture<Optional<UUID>>
    getFirstTransactionIdForArtifactById(UUID artifactId) throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getArtifactFirstTxnId(artifactId));
    }

    @Override
    public CompletableFuture<Optional<UUID>>
    getLastTransactionIdForArtifactById(UUID artifactId) throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getArtifactLastTxnId(artifactId));
    }

    @Override
    public CompletableFuture<Optional<UUID>>
    getPreviousTransactionIdForTransactionById(UUID txnId) throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getTransactionPreviousId(txnId));
    }

    @Override
    public CompletableFuture<Optional<UUID>>
    getNextTransactionIdForTransactionById(UUID txnId) throws IOException {

        return
            CompletableFuture.completedFuture(
                protocolHandler.getTransactionNextId(txnId));
    }

    @Override
    public CompletableFuture<Integer>
    getConnectionStatus() throws IOException {

        return
            CompletableFuture.completedFuture(
                Integer.valueOf(protocolHandler.getConnectionStatus()));
    }
}
