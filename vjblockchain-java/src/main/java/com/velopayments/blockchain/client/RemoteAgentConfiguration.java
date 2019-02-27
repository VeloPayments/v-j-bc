package com.velopayments.blockchain.client;

import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.util.UUID;

public class RemoteAgentConfiguration {

    private String host;
    private Integer port;

    private UUID agentId;
    private EncryptionPublicKey agentPublicKey;

    public RemoteAgentConfiguration(String host, Integer port,
                                    UUID agentId, EncryptionPublicKey agentPublicKey) {
        this.host = host;
        this.port = port;
        this.agentId = agentId;
        this.agentPublicKey = agentPublicKey;
    }

    public String getHost() {
        return host;
    }

    public Integer getPort() {
        return port;
    }

    public UUID getAgentId() {
        return agentId;
    }

    public EncryptionPublicKey getAgentPublicKey() {
        return agentPublicKey;
    }
}
