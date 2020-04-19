package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.cert.Certificate;
import java.io.IOException;

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
}
