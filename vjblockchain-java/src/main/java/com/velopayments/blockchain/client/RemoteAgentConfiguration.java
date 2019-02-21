package com.velopayments.blockchain.client;

import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.util.UUID;

public class RemoteAgentConfiguration {

    private String host;
    private Integer port;
    private UUID entityId;
    private UUID agentId;
    private EncryptionPublicKey agentPublicKey;

    public RemoteAgentConfiguration(String host, Integer port, UUID entityId,
                                    UUID agentId, EncryptionPublicKey agentPublicKey) {
        this.host = host;
        this.port = port;
        this.entityId = entityId;
        this.agentId = agentId;
        this.agentPublicKey = agentPublicKey;
    }

    public String getHost() {
        return host;
    }

    public Integer getPort() {
        return port;
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
