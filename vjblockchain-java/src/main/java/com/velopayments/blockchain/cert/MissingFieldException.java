package com.velopayments.blockchain.cert;

public class MissingFieldException extends Exception {

    static final long serialVersionUID = 1;

    /**
     * Create a MissingFieldException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public MissingFieldException(String msg) {
        super(msg);
    }

    /**
     * Create a MissingFieldException from an error String and a Throwable.
     *
     * @param msg   The error message for this exception.
     * @param error The downstream error to wrap.
     */
    public MissingFieldException(String msg, Throwable error) {
        super(msg, error);
    }
}
