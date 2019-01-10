package com.velopayments.blockchain.document;


import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
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
     * Get the encrypted document as an InputStream.  Note the document is
     * returned in decrypted form.
     *
     * @return an InputStream representing the encrypted document
     */
    public InputStream getEncrypted() throws IOException {

        return new ByteArrayInputStream(
                decryptNative(secretKey, inputStreamToByteArray(encryptedDocStream)));
    }

    /**
     * Convert an InputStream into a byte array
     * Note - with Java9 this can be replaced with
     *   byte[] array = is.readAllBytes();
     *
     * @param is InputStream to be read into the byte array
     *
     * @return byte array containing the contents of the InputStream
     *
     * @throws IOException
     */
    private byte[] inputStreamToByteArray(InputStream is) throws IOException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        while (true) {
            int r = is.read(buffer);
            if (r == -1) break;
            out.write(buffer, 0, r);
        }

        return out.toByteArray();
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
