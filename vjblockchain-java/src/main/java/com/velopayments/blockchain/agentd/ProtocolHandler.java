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
     * Get the block transaction for a given UUID.
     *
     * @param blockId  the block to retrieve
     *
     * @return the raw bytes for the given block, or empty if not found
     *
     * @throws IOException
     */
    Optional<Certificate> getBlockById(UUID blockId)
        throws IOException;

    /**
     * Get the block id for a given block height.
     *
     * @param height    The block height of this block ID.
     *
     * @return the block UUID at the given height, or empty if not found.
     *
     * @throws IOException
     */
    Optional<UUID> getBlockIdByBlockHeight(long height)
        throws IOException;

    /**
     * Get the latest block UUID in the blockchain, according to this agent.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @return the latest block UUID.
     *
     * @throws IOException
     */
    UUID getLatestBlockId()
        throws IOException;

    /**
     * Get a transaction for a given UUID.
     *
     * Note - this method blocks until the given transaction is available.
     *
     * @param transactionId     The transaction ID to look up.
     *
     * @return the raw bytes for a given transaction, or empty() if not found.
     *
     * @throws IOException
     */
    Optional<Certificate> getTransactionById(UUID transactionId)
        throws IOException;

    /**
     * Generic method for sending a UUID to an agent, and receiving one in
     * return.
     *
     * @param apiMethod the method to invoke on the remote agent
     * @param uuid a single UUID argument to the method being invoked
     *
     * @return {@code Optional} wrapper around any {@code UUID} returned
     *   from the remote method.
     *
     * @throws IOException
     */
    Optional<UUID> sendAndReceiveUUID(ApiMethod apiMethod, UUID uuid)
        throws IOException;

    /**
     * Submit a transaction to the agent.
     *
     * @param transaction the transaction to submit to the remote agent
     *
     * @throws IOException if an error occurs communicating with the agent
     * @throws InvalidResponseException if the response from the agent is invalid
     * @throws OperationFailureException if the transaction could not be submitted
     */
    void submit(Certificate transaction)
        throws IOException;

}
