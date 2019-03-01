package com.velopayments.blockchain.client;

import com.velopayments.blockchain.cert.Certificate;

import java.io.IOException;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.CompletableFuture;

public interface VelochainConnection {

    /**
     * Close the client connection.
     */
    void close() throws IOException;


    /**
     * Submit a transaction to the blockchain / agent.
     *
     * This method returns a future that indicates the status of the
     * transaction.
     *
     * @param transaction       The transaction to be added.
     *
     * @return a Future which evaluates into the status of the transaction.
     */
    CompletableFuture<TransactionStatus> submit(Certificate transaction)
            throws IOException;


    /**
     * Get the latest block UUID in the blockchain, according to this agent.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @return the latest block UUID.
     */
    UUID getLatestBlockId() throws IOException;

    /**
     * Given a block UUID, return the next block UUID if available.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @param blockId       The block UUID.
     *
     * @return the next block UUID.
     */
    Optional<UUID> getNextBlockId(UUID blockId) throws IOException;

    /**
     * Given a block UUID, return the previous block UUID if available.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @param blockId       The block UUID.
     *
     * @return the previous block UUID.
     */
    Optional<UUID> getPrevBlockId(UUID blockId) throws IOException;


    /**
     * Given a transaction UUID, return the block UUID associated with this
     * transaction UUID if available.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @param txnId         The transaction UUID.
     *
     * @return the block UUID associated with the transaction UUID.
     */
    Optional<UUID> getTransactionBlockId(UUID txnId) throws IOException;


    /**
     * Get the block transaction for a given UUID.
     *
     * Note - this method blocks until the given block is available.
     *
     * @param blockId   The transaction ID to look up.
     *
     * @return the raw bytes for a given block, or empty if not found.
     */
    Optional<Certificate> getBlockById(UUID blockId) throws IOException;

    /**
     * Get the block id for a given block height.
     *
     * @param height    The block height of this block ID.
     *
     * @return the block UUID at the given height, or empty if not found.
     */
    Optional<UUID> getBlockIdByBlockHeight(long height) throws IOException;

    /**
     * Get a transaction for a given UUID.
     *
     * Note - this method blocks until the given transaction is available.
     *
     * @param txnId     The transaction ID to look up.
     *
     * @return the raw bytes for a given transaction, or empty() if not found.
     */
    Optional<Certificate> getTransactionById(UUID txnId) throws IOException;


    /**
     * Get the first transaction UUID for a given artifact UUID.
     *
     * @param artifactId The artifact ID to look up.
     *
     * @return the first transaction id for a given artifact id.
     */
    Optional<UUID> getFirstTransactionIdForArtifactById(UUID artifactId)
            throws IOException;


    /**
     * Get the last transaction UUID for a given artifact UUID.
     *
     * @param artifactId The artifact ID to look up.
     *
     * @return the last transaction id for a given artifact id.
     */
    Optional<UUID> getLastTransactionIdForArtifactById(UUID artifactId)
            throws IOException;


    /**
     * Get the last block UUID containing a transaction for a given artifact
     * UUID.
     *
     * @param artifactId The artifact ID to look up.
     *
     * @return the last block id containing a transaction for a given artifact
     * id.
     */
    Optional<UUID> getLastBlockIdForArtifactById(UUID artifactId)
            throws IOException;


    /**
     * Get the previous transaction ID associated with the given transaction ID.
     *
     * @param txnId The transaction ID to look up.
     *
     * @return the previous transaction ID associated with this transaction ID.
     */
    Optional<UUID> getPreviousTransactionIdForTransactionById(UUID txnId)
            throws IOException;


    /**
     * Get the next transaction ID associated with the given transaction ID.
     *
     * @param txnId The transaction ID to look up.
     *
     * @return the next transaction ID associated with this transaction ID.
     */
    Optional<UUID> getNextTransactionIdForTransactionById(UUID txnId)
            throws IOException;

}
