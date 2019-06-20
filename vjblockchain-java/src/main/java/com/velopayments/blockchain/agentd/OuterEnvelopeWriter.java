package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

public class OuterEnvelopeWriter {

    static {
        Initializer.init();
    }

    public byte[] encryptPayload(byte[] key, byte[] payload)
    {
        return encryptPayloadNative(key, payload);
    }

    private native byte[] encryptPayloadNative(byte[] key, byte[] payload);

}
