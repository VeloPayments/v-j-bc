package com.velopayments.blockchain.crypt;

public class InvalidKeySizeException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an InvalidKeySizeException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public InvalidKeySizeException(String msg) {
        super(msg);
    }

    /**
     * Create an InvalidKeySizeException from an error String and a Throwable.
     *
     * @param msg   The error message for this exception.
     * @param error The downstream error to wrap.
     */
    public InvalidKeySizeException(String msg, Throwable error) {
        super(msg, error);
    }
}
