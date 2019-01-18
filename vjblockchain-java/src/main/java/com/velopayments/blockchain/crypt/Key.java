package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.init.Initializer;

/**
 * Symmetric Cipher Key.  This key works with the VeloChain SDK symmetric
 * cipher.  It includes a generate method that generates a random key from a
 * cryptographically random source and a method that derives a key from a
 * password.  When the key is constructed, it verifies that the size is
 * appropriate for the symmetric cipher used by the SDK.
 */
public class Key {

    static {
        Initializer.init();
    }

    /**
     * Generate a random Key.
     */
    public static Key createRandom() {
        return createRandomNative();
    }

    /**
     * Construct a Key from a byte array.  This method verifies that this key is
     * the correct size for the VeloChain SDK.
     *
     * @param key The byte array used for this key.
     */
    public Key(byte[] key) {
        this.key = key;
        verifyKeySizeNative();
    }

    /**
     * Get the raw bytes of this key.
     */
    public byte[] getRawBytes() {
        return key;
    }

    /**
     * Verify that this key is the correct size, or throw an
     * InvalidKeySizeException.
     */
    private native void verifyKeySizeNative();

    /**
     * Create a Key from a cryptographically random source.
     */
    private static native Key createRandomNative();

    private byte[] key;
}
