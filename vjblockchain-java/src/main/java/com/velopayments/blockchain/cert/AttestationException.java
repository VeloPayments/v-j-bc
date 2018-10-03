package com.velopayments.blockchain.cert;

public class AttestationException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an AttestationException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public AttestationException(String msg) {
        super(msg);
    }

    /**
     * Create an AttestationException from an error String and a Throwable.
     *
     * @param msg   The error message for this exception.
     * @param error The downstream error to wrap.
     */
    public AttestationException(String msg, Throwable error) {
        super(msg, error);
    }
}
