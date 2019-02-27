package com.velopayments.blockchain.agentd;

public class InvalidResponseException extends RuntimeException {

    public InvalidResponseException(String msg) {
        super(msg);
    }
}
