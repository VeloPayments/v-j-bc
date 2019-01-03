package com.velopayments.blockchain.document;


import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import org.apache.commons.io.IOUtils;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

public class EncryptedDocumentReader {

    private byte[] secretKey;
    private InputStream encryptedDocStream;

    /**
     * Create an EncryptedDocumentReader from a private key, a public key, a shared secret
     * and an InputStream.
     *
     * @param localPrivateKey     The private key of the entity reading this document.
     * @param peerPublicKey       The public key of the peer that created this document.
     * @param sharedSecret        The shared secret produced for the entity reading this document.
     * @param encryptedDocStream  The document to be decrypted.
     */
    public EncryptedDocumentReader(EncryptionPrivateKey localPrivateKey, EncryptionPublicKey peerPublicKey,
                                   byte[] sharedSecret, InputStream encryptedDocStream) {

        this.secretKey = decryptSecretNative(localPrivateKey, peerPublicKey, sharedSecret);
        this.encryptedDocStream = encryptedDocStream;
    }

    /**
     * Get the encrypted document as an InputStream.  Note the document is returned in decrypted form.
     *
     * @return an InputStream representing the encrypted document
     */
    public InputStream getEncrypted() throws IOException {

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
