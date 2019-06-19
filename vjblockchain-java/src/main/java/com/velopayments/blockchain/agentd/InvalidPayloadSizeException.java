package com.velopayments.blockchain.agentd;

public class InvalidPayloadSizeException extends RuntimeException
{
    static final long serialVersionUID = 1;

    /**
     * Create an InvalidPayloadSizeException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public InvalidPayloadSizeException(String msg) {
        super(msg);
    }

}
