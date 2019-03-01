package com.velopayments.blockchain.agentd;

public class OperationFailureException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an OperationFailureException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public OperationFailureException(String msg) {
        super(msg);
    }
}
