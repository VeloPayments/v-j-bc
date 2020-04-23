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
}
