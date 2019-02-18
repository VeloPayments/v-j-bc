package com.velopayments.blockchain.agentd;

public class AgentdIOException extends RuntimeException {

    public AgentdIOException(String message) {
        super(message);
    }

    public AgentdIOException(String message, Throwable t) {
        super(message, t);
    }
}
