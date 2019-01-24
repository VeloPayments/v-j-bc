package com.velopayments.blockchain.crypt;

public class MessageAuthenticationException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an MessageAuthenticationException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public MessageAuthenticationException(String msg) {
        super(msg);
    }

    /**
     * Create an MessageAuthenticationException from an error String and a
     * Throwable.
     *
     * @param msg   The error message for this exception.
     * @param error The downstream error to wrap.
     */
    public MessageAuthenticationException(String msg, Throwable error) {
        super(msg, error);
    }
}
