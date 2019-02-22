package com.velopayments.blockchain.agentd;

public enum ApiMethod {

    // TODO: these values are not correct
    SUBMIT(1),
    GET_LATEST_BLOCK_ID(2),
    GET_NEXT_BLOCK_ID(3),
    GET_PREV_BLOCK_ID(4),
    GET_TXN_BLOCK_ID(5),
    GET_BLOCK_BY_ID(6),
    GET_BLOCK_ID_BY_BLOCK_HEIGHT(7),
    GET_TXN_BY_ID(8),
    GET_FIRST_TXN_ID_FOR_ARTIFACT_BY_ID(9),
    GET_LAST_TXN_ID_FOR_ARTIFACT_BY_ID(10),
    GET_LAST_BLOCK_ID_FOR_ARTIFACT_BY_ID(11),
    GET_PREV_TXN_ID_FOR_TXN_BY_ID(12),
    GET_NEXT_TXN_ID_FOR_TXN_BY_ID(13);

    private int value;

    ApiMethod(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

}
