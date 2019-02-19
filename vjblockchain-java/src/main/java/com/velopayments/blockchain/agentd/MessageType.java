package com.velopayments.blockchain.agentd;

public enum MessageType {

    HANDSHAKE(1);

    private int value;

    MessageType(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }
}
