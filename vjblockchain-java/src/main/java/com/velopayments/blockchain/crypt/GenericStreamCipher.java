package com.velopayments.blockchain.crypt;

public class GenericStreamCipher {

    /**
     * Encrypt data in the input buffer, returning an encrypted buffer.
     *
     * @param key   The key to use to encrypt this data.
     * @param iv    The initialization vector to use.
     * @param input The input to encrypt.
     *
     * @return the encrypted data with iv / HMAC prepended / appended.
     */
    public static byte[] encryptData(byte[] key, byte[] iv, byte[] input) {
        return input;  // TODO
    }

    //private static native byte[] encryptData(
    //        byte[] key, byte[] iv, byte[] input);


    /**
     * Decrypt the input value using the provided secret key.
     *
     * @param secretKey     The secret key to use to decrypt this value.
     * @param input         The input value to decrypt.
     *
     * @return the decrypted value.
     */
    public static byte[] decryptData(byte[] secretKey, byte[] input) {
        return input; // TODO
    }

    //private static native byte[] decryptNative(byte[] secretKey, byte[] input);


}
