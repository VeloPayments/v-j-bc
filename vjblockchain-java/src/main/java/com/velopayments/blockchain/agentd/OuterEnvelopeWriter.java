package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

public class OuterEnvelopeWriter {

    static {
        Initializer.init();
    }

    public OuterEnvelopeWriter() {
        this.iv = 0x0000000000000001L;
    }

    public byte[] encryptPayload(byte[] key, byte[] payload)
    {
        //encrypt the payload
        byte[] encryptedPayload = encryptPayloadNative(key, iv, payload);

        //increment iv after encrypting payload
        ++iv;

        return encryptedPayload;
    }

    private long iv;
    private native byte[] encryptPayloadNative(
            byte[] key, long iv, byte[] payload);

}
