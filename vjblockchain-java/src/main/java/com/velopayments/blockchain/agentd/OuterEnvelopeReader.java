package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

public class OuterEnvelopeReader {

    static {
        Initializer.init();
    }

    public int decryptHeader(byte[] key, byte[] header)
    {
        return decryptHeaderNative(key, header);
    }

    public byte[] decryptPayload(byte[] key, byte[] payload)
    {
        return decryptPayloadNative(key, payload);
    }

    private native int decryptHeaderNative(
            byte[] key, byte[] header);

    private native byte[] decryptPayloadNative(
            byte[] key, byte[] payload);
}
