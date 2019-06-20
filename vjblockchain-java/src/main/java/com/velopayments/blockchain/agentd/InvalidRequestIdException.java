package com.velopayments.blockchain.agentd;

public class InvalidRequestIdException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an InvalidRequestIdException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public InvalidRequestIdException(String msg) {
        super(msg);
    }

}
