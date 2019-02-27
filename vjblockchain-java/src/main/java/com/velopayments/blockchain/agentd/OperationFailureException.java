package com.velopayments.blockchain.agentd;

public class OperationFailureException extends RuntimeException {

    public OperationFailureException(String msg) {
        super(msg);
    }
}
