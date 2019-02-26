package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.cert.Certificate;

import java.io.IOException;
import java.util.Optional;
import java.util.UUID;

public interface ProtocolHandler {

    void handshake() throws IOException;

    Optional<Certificate> getBlockById(UUID blockId)
        throws IOException;

    Optional<UUID> getBlockIdByBlockHeight(long height)
        throws IOException;

    UUID getLatestBlockId()
        throws IOException;

    Optional<Certificate> getTransactionById(UUID transactionId)
        throws IOException;

    Optional<UUID> sendAndReceiveUUID(ApiMethod apiMethod, UUID uuid)
        throws IOException;

}
