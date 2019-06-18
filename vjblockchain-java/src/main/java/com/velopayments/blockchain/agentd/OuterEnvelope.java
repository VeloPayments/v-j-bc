package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

// TODO: OuterEnvelopeWrapper?
public class OuterEnvelope {

    static {
        Initializer.init();
    }

    private byte[] key;
    private byte[] payload;

    private byte[] wrapped;

    public OuterEnvelope(byte[] key, byte[] payload)
    {
        this.key = key;
        this.payload = payload;

        wrapped = wrapPayload(key, payload);
    }

    private native byte[] wrapPayload(byte[] key, byte[] payload);

    public byte[] getWrapped() {
        return wrapped;
    }
}
