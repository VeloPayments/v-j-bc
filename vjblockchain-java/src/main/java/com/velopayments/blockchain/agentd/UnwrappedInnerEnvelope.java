package com.velopayments.blockchain.agentd;

public class UnwrappedInnerEnvelope {

    private long requestId;

    private long requestOffset;

    private long status;

    private byte[] payload;


    public UnwrappedInnerEnvelope(long requestId, long requestOffset,
                                  long status, byte[] payload) {
        this.requestId = requestId;
        this.requestOffset = requestOffset;
        this.status = status;
        this.payload = payload;
    }

    public long getRequestId() {
        return requestId;
    }

    public long getRequestOffset() {
        return requestOffset;
    }

    public long getStatus() {
        return status;
    }

    public byte[] getPayload() {
        return payload;
    }
}
