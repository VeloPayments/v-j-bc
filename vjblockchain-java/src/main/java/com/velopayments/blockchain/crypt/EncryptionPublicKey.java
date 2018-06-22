package com.velopayments.blockchain.crypt;

public class EncryptionPublicKey {

    public EncryptionPublicKey(byte[] rawBytes) {
        this.rawBytes = rawBytes;
    }

    public byte[] getRawBytes() {
        return rawBytes;
    }

    private byte[] rawBytes;
}
