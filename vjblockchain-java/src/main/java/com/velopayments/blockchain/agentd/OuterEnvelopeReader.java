package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

public class OuterEnvelopeReader {

    public OuterEnvelopeReader() {
        this.iv = 0x8000000000000001L;
    }

    public int decryptHeader(byte[] key, byte[] header)
    {
        return
            decryptHeaderNative(
                Initializer.getInstance(), key, iv, header);
    }

    public byte[] decryptPayload(byte[] key, byte[] header, byte[] payload)
        throws MessageVerificationException
    {
        byte[] decryptedPayload =
            decryptPayloadNative(
                Initializer.getInstance(), key, iv, header, payload);

        //increment iv after decrypting payload.
        ++iv;

        return decryptedPayload;
    }

    private long iv;

    private native int decryptHeaderNative(
            long nativeInst, byte[] key, long iv, byte[] header);

    private native byte[] decryptPayloadNative(
            long nativeInst, byte[] key, long iv, byte[] header, byte[] payload)
        throws MessageVerificationException;
}
