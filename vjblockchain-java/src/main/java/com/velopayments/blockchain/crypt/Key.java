package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.init.Initializer;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

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

        char[] chars = password.toCharArray();
        PBEKeySpec spec = new PBEKeySpec(chars, salt, iterations, 256);
        try {
            SecretKeyFactory skf =
                SecretKeyFactory.getInstance("PBKDF2WithHmacSHA256");
            byte[] keyHash = skf.generateSecret(spec).getEncoded();

            return new Key(keyHash);
        } catch (Exception e) {
            throw new IllegalStateException("Wrongness.", e);
        }
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
