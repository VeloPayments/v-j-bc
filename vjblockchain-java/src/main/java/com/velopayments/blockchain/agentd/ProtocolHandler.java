package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.cert.Certificate;
import java.io.IOException;
import java.util.Optional;
import java.util.UUID;

/**
 * Handler for implementing the wire protocol
 */
public interface ProtocolHandler {

    /**
     * Perform a handshake operation.  A handshake has two stages:
     * 1) Initiation
     * 2) Acknowledgement
     *
     * @throws IOException
     */
    void handshake() throws IOException;

    /**
     * Submit a certificate to the blockchain agent.
     */
    void submit(Certificate transaction) throws IOException;

    /**
     * Get the latest block id from the blockchain agent.
     */
    UUID getLatestBlockId() throws IOException;

    /**
     * Get a block by UUID.
     *
     * @param blockId The block id to get.
     */
    Optional<Certificate> getBlockById(UUID blockId) throws IOException;

    /**
     * Given a block id, get the next block id if possible.
     *
     * @param blockId The block id.
     */
    Optional<UUID> getNextBlockId(UUID blockId) throws IOException;

    /**
     * Given a block id, get the previous block id if possible.
     *
     * @param blockId The block id.
     */
    Optional<UUID> getPrevBlockId(UUID blockId) throws IOException;

    /**
     * Get a transaction by UUID.
     *
     * @param txnId The transaction id.
     */
    Optional<Certificate> getTransactionById(UUID txnId) throws IOException;

    /**
     * Get the next transaction id given a transaction id.
     *
     * @param txnId The transaction id.
     */
    Optional<UUID> getTransactionNextId(UUID txnId) throws IOException;

    /**
     * Get the previous transaction id given a transaction id.
     *
     * @param txnId The transaction id.
     */
    Optional<UUID> getTransactionPreviousId(UUID txnId) throws IOException;

    /**
     * Get the block id of a transaction.
     *
     * @param txnId The transaction id.
     */
    Optional<UUID> getTransactionBlockId(UUID txnId) throws IOException;

    /**
     * Get the first transaction id of an artifact.
     *
     * @param artifactId The artifact id.
     */
    Optional<UUID> getArtifactFirstTxnId(UUID artifactId) throws IOException;

    /**
     * Get the last transaction id of an artifact.
     *
     * @param artifactId The artifact id.
     */
    Optional<UUID> getArtifactLastTxnId(UUID artifactId) throws IOException;

    /**
     * Given a block height, get the block id if possible.
     *
     * @param height The block height.
     */
    Optional<UUID> getBlockIdByBlockHeight(long height) throws IOException;
}
