package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.init.Initializer;

/**
 * Simple Stream Cipher.  This stream cipher uses the suite provided stream
 * cipher as defined in the VeloChain specification to encrypt and MAC a field.
 * The constructor takes a key as input.  The encrypt method takes a byte array
 * as input and provides a byte array as output.  The output byte array is a
 * blob representing an encrypted initialization vector, the encrypted data, and
 * a message authentication code.  The message authentication code is keyed
 * using the input key, and the data is encrypted using a secret key that is
 * derived from the input key and the IV.  This key extension mechanism ensures
 * that if this stream cipher is used incorrectly, it protects the secret key
 * more than using CTR mode or similar.  The cost is that each time the
 * SimpleStreamCipher is used, it will generate a unique key.
 */
public class SimpleStreamCipher {

    /**
     * Generate a SimpleStreamCipher from a random Key.
     */
    public static SimpleStreamCipher createRandom() {
        return new SimpleStreamCipher(Key.createRandom());
    }

    /**
     * Construct a SimpleStreamCipher from a Key.
     * @param key The key for this stream cipher.
     */
    public SimpleStreamCipher(Key key) {
        this.key = key;
    }

    /**
     * Get the key for this SimpleStreamCipher.
     */
    public Key getKey() {
        return key;
    }

    /**
     * Encrypt an input byte array, returning an encrypted output byte array.
     *
     * @param input The input byte array to encrypt.
     */
    public byte[] encrypt(byte[] input) {
        return encryptNative(Initializer.getInstance(), input);
    }

    /**
     * Decrypt an input byte array, returning a plaintext output byte array or
     * throwing a MessageAuthenticationException if this decryption fails.
     *
     * @param input The input byte array to decrypt.
     */
    public byte[] decrypt(byte[] input) {
        return decryptNative(Initializer.getInstance(), input);
    }

    private native byte[] encryptNative(long nativeInst, byte[] input);
    private native byte[] decryptNative(long nativeInst, byte[] input);

    private Key key;
}
