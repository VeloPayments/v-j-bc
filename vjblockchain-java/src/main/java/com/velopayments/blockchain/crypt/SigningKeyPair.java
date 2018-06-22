package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.init.Initializer;

public class SigningKeyPair {

    static {
        Initializer.init();
    }

    /**
     * Generate a keypair.
     */
    public static SigningKeyPair generate() {
        return generateNative();
    }

    /**
     * Constructor.  Create an SigningKeyPair from a public key and a private
     * key.
     *
     * @param publicKey The public key for this keypair.
     * @param privateKey The private key for this keypair.
     */
    public SigningKeyPair(
            SigningPublicKey publicKey, SigningPrivateKey privateKey) {
        this.publicKey = publicKey;
        this.privateKey = privateKey;
    }

    /**
     * Get the public key for this keypair.
     */
    public SigningPublicKey getPublicKey() {
        return publicKey;
    }

    /**
     * Get the private key for this keypair.
     */
    public SigningPrivateKey getPrivateKey() {
        return privateKey;
    }

    private static native SigningKeyPair generateNative();

    private SigningPublicKey publicKey;
    private SigningPrivateKey privateKey;
}
