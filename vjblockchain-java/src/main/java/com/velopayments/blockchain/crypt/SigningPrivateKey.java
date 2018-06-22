package com.velopayments.blockchain.crypt;

public class SigningPrivateKey {

    public SigningPrivateKey(byte[] rawBytes) {
        this.rawBytes = rawBytes;
    }

    public byte[] getRawBytes() {
        return rawBytes;
    }

    private byte[] rawBytes;
}
