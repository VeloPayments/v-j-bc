package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

public class OuterEnvelopeReader {

    private int payloadSize;

    static {
        Initializer.init();
    }

    public OuterEnvelopeReader(byte[] key, byte[] header)
    {
        payloadSize = getPayloadSize(key, header);
    }

    private native int getPayloadSize(byte[] key, byte[] header);

    public int getPayloadSize() {
        return payloadSize;
    }

    public byte[] decryptPayload(byte[] payload) {
        return payload;
    }
}
