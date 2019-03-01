package com.velopayments.blockchain.agentd;

public class MessageVerificationException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an MessageVerificationException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public MessageVerificationException(String msg) {
        super(msg);
    }

}
