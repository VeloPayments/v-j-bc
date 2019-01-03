package com.velopayments.blockchain.document;


import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import org.apache.commons.io.IOUtils;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

public class EncryptedDocumentReader {

    private EncryptionPrivateKey localPrivateKey;
    private EncryptionPublicKey peerPublicKey;
    private byte[] secretKey;
    private InputStream encryptedDocStream;

    public EncryptedDocumentReader(EncryptionPrivateKey localPrivateKey, EncryptionPublicKey peerPublicKey,
                                   InputStream encryptedDocStream) {
        this.localPrivateKey = localPrivateKey;
        this.peerPublicKey = peerPublicKey;
        this.encryptedDocStream = encryptedDocStream;
    }

    // TODO - maybe pass the audience public key in here ?
    public void loadSecretKey(byte[] sharedSecret) {
        secretKey = decryptSecretNative(localPrivateKey, peerPublicKey, sharedSecret);
    }

    /**
     * Get the encrypted document as an InputStream.
     *
     * @return an InputStream representing the encrypted document
     * @throws IllegalStateException if the secret key has not been loaded
     */
    public InputStream getEncrypted() throws IOException {

        if (null == secretKey) {
            throw new IllegalStateException("Secret key must be loaded.");
        }

        return new ByteArrayInputStream(decryptNative(secretKey, IOUtils.toByteArray(encryptedDocStream)));
    }


    /**
     * Recover the secret key from the given local private key, peer public key,
     * and encrypted key.
     *
     * @param localPrivateKey   The local private key.
     * @param peerPublicKey     The peer public key.
     * @param encryptedKey      The encrypted key to decrypt.
     *
     * @return the decrypted key.
     */
    private static native byte[] decryptSecretNative(
            EncryptionPrivateKey localPrivateKey, EncryptionPublicKey peerPublicKey,
            byte[] encryptedKey);

    /**
     * Decrypt the input value using the provided secret key.
     *
     * @param secretKey     The secret key to use to decrypt this value.
     * @param input         The input value to decrypt.
     *
     * @return the decrypted value.
     */
    private static native byte[] decryptNative(byte[] secretKey, byte[] input);

}
