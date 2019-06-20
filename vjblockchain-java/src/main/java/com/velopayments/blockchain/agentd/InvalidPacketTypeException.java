package com.velopayments.blockchain.agentd;

public class InvalidPacketTypeException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an InvalidPacketTypeException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public InvalidPacketTypeException(String msg) {
        super(msg);
    }

}
