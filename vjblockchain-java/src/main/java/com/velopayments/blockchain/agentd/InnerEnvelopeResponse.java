package com.velopayments.blockchain.agentd;

/**
 * Structure representing the contents of an inner envelope of a response.
 */
public class InnerEnvelopeResponse {

    private long apiMethodId;

    private long requestId;

    private long status;

    private byte[] payload;

    /**
     * Constructor method
     *
     * @param apiMethodId the API Method sending the response
     * @param requestId   requestId passed in the initial request
     * @param status      0 for success, non-zero for failure
     * @param payload     payload of the response
     */
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
