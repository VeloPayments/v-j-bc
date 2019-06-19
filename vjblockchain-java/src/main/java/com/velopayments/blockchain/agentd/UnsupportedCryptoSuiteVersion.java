package com.velopayments.blockchain.agentd;

public class UnsupportedCryptoSuiteVersion extends RuntimeException
{
    static final long serialVersionUID = 1;

    /**
     * Create an UnsupportedCryptoSuiteVersion from an error String.
     *
     * @param msg The error message for this exception.
     */
    public UnsupportedCryptoSuiteVersion(String msg) {
        super(msg);
    }

}
