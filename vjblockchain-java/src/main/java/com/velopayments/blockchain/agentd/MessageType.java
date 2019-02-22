package com.velopayments.blockchain.agentd;

public enum MessageType {

    // TODO: the values are not correct
    UNAUTHENTICATED(1),
    AUTHENTICATED(2);

    private int value;

    MessageType(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }
}
