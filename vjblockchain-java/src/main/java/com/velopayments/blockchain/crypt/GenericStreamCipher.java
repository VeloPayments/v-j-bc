package com.velopayments.blockchain.crypt;

/**
 * Generic stream cipher.  This class exposes the suite provided stream
 * cipher as defined in the Velochain specification to encrypt and MAC
 * a field.
 */
public class GenericStreamCipher {

    /**
     * Encrypt data in the input buffer, returning an encrypted buffer.
     *
     * @param key   The key to use to encrypt this data.
     * @param iv    The initialization vector to use.
     * @param input The input to encrypt.
     *
     * @return the encrypted data with iv / HMAC prepended / appended.
     * FIXME: not currently appending HMAC
     */
    public static byte[] encrypt(byte[] key, byte[] iv, byte[] input) {
        return encryptNative(key, iv, input);
    }

    private static native byte[] encryptNative(
            byte[] key, byte[] iv, byte[] input);


    /**
     * Decrypt the input value using the provided secret key.
     *
     * @param secretKey     The secret key to use to decrypt this value.
     * @param input         The input value to decrypt.
     *
     * @return the decrypted value.
     */
    public static byte[] decrypt(byte[] secretKey, byte[] input) {
        return decryptNative(secretKey, input);
    }

    private static native byte[] decryptNative(byte[] secretKey, byte[] input);


}
