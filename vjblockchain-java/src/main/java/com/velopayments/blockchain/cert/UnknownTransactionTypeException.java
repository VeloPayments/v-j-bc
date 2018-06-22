package com.velopayments.blockchain.cert;

import java.util.UUID;

public class UnknownTransactionTypeException extends Exception {

    static final long serialVersionUID = 1;

    /**
     * Create an UnknownTransactionTypeException from an error String.
     *
     * @param msg                   The error message for this exception.
     */
    public UnknownTransactionTypeException(String msg) {
        super(msg);
    }

    /**
     * Create an UnknownTransactionTypeException from an transaction type UUID.
     *
     * @param transactionTypeId        The transaction typeUUID.
     */
    public UnknownTransactionTypeException(UUID transactionTypeId) {
        super("Unknown Transaction Type " + transactionTypeId.toString());
    }
}
