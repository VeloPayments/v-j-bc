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
     * Generate a key from a password.
     *
     * @param salt The random salt to use for this password.  Must be unique per
     *             password and should be stored as password metadata.
     * @param iterations The number of iterations (e.g. 10000).
     * @param password The password from which this key is derived.
     *
     * @return a Key from this password and metadata.
     */
    public static Key createFromPassword(
                            byte[] salt, int iterations, String password) {

        return new Key(createFromPasswordAsBytes(
                salt, iterations, password, false));
    }

    /**
     * Generate a key from a password.
     *
     * @param salt The random salt to use for this password.  Must be unique per
     *             password and should be stored as password metadata.
     * @param iterations The number of iterations (e.g. 10000).
     * @param password The password from which this key is derived.
     * @param sha512 If true use HMAC-SHA-512 as the PRF, otherwise use
     *               HMAC-SHA-512-256
     *
     * @return a byte array from this password and metadata.
     */
    public static byte[] createFromPasswordAsBytes(
            byte[] salt, int iterations, String password, boolean sha512)
    {
        return createFromPasswordNative(
                password.getBytes(), salt, iterations, sha512);
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


    /**
     * Generate a key from a password.
     *
     * @param salt The random salt to use for this password.  Must be unique per
     *             password and should be stored as password metadata.
     * @param iterations The number of iterations (e.g. 10000).
     * @param password The password from which this key is derived.
     * @param sha512 If true use HMAC-SHA-512 as the PRF, otherwise use
     *               HMAC-SHA-512-256
     *
     * @return a byte array from this password and metadata.
     */
    private static native byte[] createFromPasswordNative(
            byte[] password, byte[] salt, int iterations, boolean sha512);

    private byte[] key;


}
