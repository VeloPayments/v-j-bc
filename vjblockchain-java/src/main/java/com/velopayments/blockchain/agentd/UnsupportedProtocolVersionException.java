package com.velopayments.blockchain.agentd;

public class UnsupportedProtocolVersionException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an UnsupportedProtocolVersionException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public UnsupportedProtocolVersionException(String msg) {
        super(msg);
    }

}
