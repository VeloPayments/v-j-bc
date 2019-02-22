package com.velopayments.blockchain.agentd;

public class InnerEnvelopeResponse {

    private long apiMethodId;

    private long requestId;

    private long status;

    private byte[] payload;


    public InnerEnvelopeResponse(long apiMethodId, long requestId,
                                 long status, byte[] payload) {
        this.apiMethodId = apiMethodId;
        this.requestId = requestId;
        this.status = status;
        this.payload = payload;
    }

    public long getApiMethodId() {
        return apiMethodId;
    }

    public long getRequestId() {
        return requestId;
    }

    public long getStatus() {
        return status;
    }

    public byte[] getPayload() {
        return payload;
    }
}
