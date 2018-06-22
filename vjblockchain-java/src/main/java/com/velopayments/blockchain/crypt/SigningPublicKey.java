package com.velopayments.blockchain.crypt;

public class SigningPublicKey {

    public SigningPublicKey(byte[] rawBytes) {
        this.rawBytes = rawBytes;
    }

    public byte[] getRawBytes() {
        return rawBytes;
    }

    private byte[] rawBytes;
}
