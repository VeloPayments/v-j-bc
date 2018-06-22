package com.velopayments.blockchain.cert;

public class FieldConversionException extends Exception {

    static final long serialVersionUID = 1;

    /**
     * Create a FieldConversionException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public FieldConversionException(String msg) {
        super(msg);
    }

    /**
     * Create a FieldConversionException from an error String and a Throwable.
     *
     * @param msg   The error message for this exception.
     * @param error The downstream error to wrap.
     */
    public FieldConversionException(String msg, Throwable error) {
        super(msg, error);
    }
}
