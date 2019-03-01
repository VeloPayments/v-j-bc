package com.velopayments.blockchain.agentd;

public class InvalidResponseException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an InvalidResponseException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public InvalidResponseException(String msg) {
        super(msg);
    }
}
