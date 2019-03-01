package com.velopayments.blockchain.client;

import com.velopayments.blockchain.agentd.*;
import com.velopayments.blockchain.cert.Certificate;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;

import javax.net.SocketFactory;
import java.io.IOException;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.CompletableFuture;

/**
 * Connect to a remote agent (agentd) and perform basic client functions.
 */
public class RemoteAgentConnection implements VelochainConnection {

    private RemoteAgentChannel remoteAgentChannel;
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

    /**
     * Submit a transaction to the remote agent (agentd).
     *
     * Implementation note: this is currently a synchronous operation,
     * with agentd blocking submission on success.  Therefore, we immediately
     * return a success / failure status.  Future implementations will
     * process asynchronously and batch submissions.
     *
     * @param transaction       The transaction to be added.
     *
     * @return a Future which evaluates into the status of the transaction.
     * @throws IOException
     */
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

    /**
     * Get the latest block UUID in the blockchain, according to this agent.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @return the latest block UUID.
     * @throws IOException
     */
    @Override
    public UUID getLatestBlockId()
    throws IOException {

        return protocolHandler.getLatestBlockId();
    }

    /**
     * Given a block UUID, return the next block UUID if available.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @param blockId       The block UUID.
     *
     * @return the next block UUID.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getNextBlockId(UUID blockId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_NEXT_BLOCK_ID, blockId);
    }

    /**
     * Given a block UUID, return the previous block UUID if available.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @param blockId       The block UUID.
     *
     * @return the previous block UUID.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getPrevBlockId(UUID blockId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_PREV_BLOCK_ID, blockId);
    }

    /**
     * Given a transaction UUID, return the block UUID associated with this
     * transaction UUID if available.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @param txnId         The transaction UUID.
     *
     * @return the block UUID associated with the transaction UUID.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getTransactionBlockId(UUID txnId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(ApiMethod.GET_TXN_BLOCK_ID, txnId);
    }

    /**
     * Get the block transaction for a given UUID.
     *
     * Note - this method blocks until the given block is available.
     *
     * @param blockId   The transaction ID to look up.
     *
     * @return the raw bytes for a given block, or empty if not found.
     * @throws IOException
     */
    @Override
    public Optional<Certificate> getBlockById(UUID blockId)
    throws IOException {

        return protocolHandler.getBlockById(blockId);
    }

    /**
     * Get the block id for a given block height.
     *
     * @param height    The block height of this block ID.
     *
     * @return the block UUID at the given height, or empty if not found.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getBlockIdByBlockHeight(long height)
    throws IOException {

        return protocolHandler.getBlockIdByBlockHeight(height);
    }

    /**
     * Get a transaction for a given UUID.
     *
     * Note - this method blocks until the given transaction is available.
     *
     * @param txnId     The transaction ID to look up.
     *
     * @return the raw bytes for a given transaction, or empty() if not found.
     * @throws IOException
     */
    @Override
    public Optional<Certificate> getTransactionById(UUID txnId)
    throws IOException {

        return protocolHandler.getTransactionById(txnId);
    }

    /**
     * Get the first transaction UUID for a given artifact UUID.
     *
     * @param artifactId The artifact ID to look up.
     *
     * @return the first transaction id for a given artifact id.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getFirstTransactionIdForArtifactById(UUID artifactId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_FIRST_TXN_ID_FOR_ARTIFACT_BY_ID,
                artifactId);
    }

    /**
     * Get the last transaction UUID for a given artifact UUID.
     *
     * @param artifactId The artifact ID to look up.
     *
     * @return the last transaction id for a given artifact id.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getLastTransactionIdForArtifactById(UUID artifactId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_LAST_TXN_ID_FOR_ARTIFACT_BY_ID,
                artifactId);
    }

    /**
     * Get the last block UUID containing a transaction for a given artifact
     * UUID.
     *
     * @param artifactId The artifact ID to look up.
     *
     * @return the last block id containing a transaction for a given artifact
     * id.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getLastBlockIdForArtifactById(UUID artifactId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_LAST_BLOCK_ID_FOR_ARTIFACT_BY_ID,
                artifactId);
    }

    /**
     * Get the previous transaction ID associated with the given transaction ID.
     *
     * @param txnId The transaction ID to look up.
     *
     * @return the previous transaction ID associated with this transaction ID.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getPreviousTransactionIdForTransactionById(UUID txnId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_PREV_TXN_ID_FOR_TXN_BY_ID,
                txnId);
    }

    /**
     * Get the next transaction ID associated with the given transaction ID.
     *
     * @param txnId The transaction ID to look up.
     *
     * @return the next transaction ID associated with this transaction ID.
     * @throws IOException
     */
    @Override
    public Optional<UUID> getNextTransactionIdForTransactionById(UUID txnId)
    throws IOException {

        return protocolHandler.sendAndReceiveUUID(
                ApiMethod.GET_NEXT_TXN_ID_FOR_TXN_BY_ID,
                txnId);
    }
}
