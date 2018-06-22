package com.velopayments.blockchain.crypt;

public class EncryptionPrivateKey {

    public EncryptionPrivateKey(byte[] rawBytes) {
        this.rawBytes = rawBytes;
    }

    public byte[] getRawBytes() {
        return rawBytes;
    }

    private byte[] rawBytes;
}
