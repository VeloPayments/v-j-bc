package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.init.Initializer;

public class SigningPrivateKey {

    static {
        Initializer.init();
    }

    /**
     * Create a private signing key from a raw byte array.
     *
     * @param rawBytes The raw bytes of the private key.
     */
    public SigningPrivateKey(byte[] rawBytes) {
        this.rawBytes = rawBytes;
    }

    /**
     * Get the raw bytes of a private signing key.
     */
    public byte[] getRawBytes() {
        return rawBytes;
    }

    /**
     * Sign a byte array using the private key.
     *
     * @param message The byte array to sign.
     */
    public Signature sign(Message message) {
        return signNative(message);
    }

    private native Signature signNative(Message message);

    private byte[] rawBytes;
}
