package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

public class OuterEnvelopeReader {

    static {
        Initializer.init();
    }

    public OuterEnvelopeReader() {
        this.iv = 0x8000000000000001L;
    }

    public int decryptHeader(byte[] key, byte[] header)
    {
        return decryptHeaderNative(key, iv, header);
    }

    public byte[] decryptPayload(byte[] key, byte[] header, byte[] payload)
        throws MessageVerificationException
    {
        byte[] decryptedPayload =
            decryptPayloadNative(key, iv, header, payload);

        //increment iv after decrypting payload.
        ++iv;

        return decryptedPayload;
    }

    private long iv;

    private native int decryptHeaderNative(
            byte[] key, long iv, byte[] header);

    private native byte[] decryptPayloadNative(
            byte[] key, long iv, byte[] header, byte[] payload)
        throws MessageVerificationException;
}
