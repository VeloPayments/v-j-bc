package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

public class OuterEnvelopeReader {

    private byte[] key;

    static {
        Initializer.init();
    }

    public OuterEnvelopeReader(byte[] key)
    {
        this.key = key;
    }


    public int decryptPayloadSize(byte[] header) {
        return decryptPayloadSize(key, header);
    }

    public byte[] decryptPayload(byte[] payload) {
        return decryptPayload(key, payload);
    }

    private native int decryptPayloadSize(byte[] key, byte[] header);

    private native byte[] decryptPayload(byte[] key, byte[] payload);
}
