package com.velopayments.blockchain.client;

import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.util.UUID;

public class RemoteAgentConfiguration {

    private String host;
    private Integer port;

    private UUID entityId;
    private EncryptionPrivateKey entityPrivateKey;

    private UUID agentId;
    private EncryptionPublicKey agentPublicKey;

    public RemoteAgentConfiguration(String host, Integer port,
                                    UUID entityId, EncryptionPrivateKey entityPrivateKey,
                                    UUID agentId, EncryptionPublicKey agentPublicKey) {
        this.host = host;
        this.port = port;
        this.entityId = entityId;
        this.entityPrivateKey = entityPrivateKey;
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

    public EncryptionPrivateKey getEntityPrivateKey() {
        return entityPrivateKey;
    }

    public UUID getAgentId() {
        return agentId;
    }

    public EncryptionPublicKey getAgentPublicKey() {
        return agentPublicKey;
    }
}
