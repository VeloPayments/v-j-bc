package com.velopayments.blockchain.agentd;

public class ErrorStatusException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an ErrorStatusException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public ErrorStatusException(String msg) {
        super(msg);
    }

}
