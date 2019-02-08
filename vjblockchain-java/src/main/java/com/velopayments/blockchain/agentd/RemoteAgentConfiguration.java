package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.util.UUID;

public class RemoteAgentConfiguration {
    private UUID entityId;
    private UUID agentId;
    private EncryptionPublicKey agentPublicKey;

    public RemoteAgentConfiguration(UUID entityId, UUID agentId, EncryptionPublicKey agentPublicKey) {
        this.entityId = entityId;
        this.agentId = agentId;
        this.agentPublicKey = agentPublicKey;
    }

    public UUID getEntityId() {
        return entityId;
    }

    public UUID getAgentId() {
        return agentId;
    }

    public EncryptionPublicKey getAgentPublicKey() {
        return agentPublicKey;
    }
}
