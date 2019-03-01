package com.velopayments.blockchain.agentd;

public class AgentVerificationException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an AgentVerificationException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public AgentVerificationException(String msg) {
        super(msg);
    }
}
