package com.velopayments.blockchain.agentd;

import java.io.IOException;
import java.util.Optional;
import java.util.UUID;

public interface ProtocolHandler {

    void handshake() throws IOException;

    Optional<UUID> sendAndReceiveUUID(ApiMethod apiMethod, UUID uuid)
        throws IOException;

}
