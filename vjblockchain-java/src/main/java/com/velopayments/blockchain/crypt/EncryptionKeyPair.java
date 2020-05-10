package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.init.Initializer;

public class EncryptionKeyPair {

    /**
     * Generate a keypair.
     */
    public static EncryptionKeyPair generate() {
        return generateNative(Initializer.getInstance());
    }

    /**
     * Constructor.  Create an EncryptionKeyPair from a public key and a private
     * key.
     *
     * @param publicKey The public key for this keypair.
     * @param privateKey The private key for this keypair.
     */
    public EncryptionKeyPair(
            EncryptionPublicKey publicKey, EncryptionPrivateKey privateKey) {
        this.publicKey = publicKey;
        this.privateKey = privateKey;
    }

    /**
     * Get the public key for this keypair.
     */
    public EncryptionPublicKey getPublicKey() {
        return publicKey;
    }

    /**
     * Get the private key for this keypair.
     */
    public EncryptionPrivateKey getPrivateKey() {
        return privateKey;
    }

    private static native EncryptionKeyPair generateNative(long nativeInst);

    private EncryptionPublicKey publicKey;
    private EncryptionPrivateKey privateKey;
}
