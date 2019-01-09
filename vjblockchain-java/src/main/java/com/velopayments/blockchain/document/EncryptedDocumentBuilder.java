package com.velopayments.blockchain.document;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class EncryptedDocumentBuilder {

    private EncryptionKeyPair keyPair;
    private byte[] secretKey;
    private InputStream encryptedDocStream;

    /**
     * Protected constructor.  The create*() static methods should be used.
     */
    protected EncryptedDocumentBuilder(EncryptionKeyPair keyPair) {
        this.keyPair = keyPair;
        this.secretKey = generateEncryptionKey();
    }

    /**
     * Create an EncryptedDocumentBuilder instance from an EncryptionKeyPair
     */
    public static EncryptedDocumentBuilder createDocumentBuilder(EncryptionKeyPair keyPair) {
        return new EncryptedDocumentBuilder(keyPair);
    }

    /**
     * Set the document to be encrypted
     *
     * @param docStream InputStream of the document to be encrypted
     *
     * @return this builder for additional operations
     *
     * @throws IOException if an I/O error occurs
     */
    public EncryptedDocumentBuilder withDocument(InputStream docStream) throws IOException {

        // TODO: used "chunked approach"  (BLOC-158)
        byte[] docBytes = inputStreamToByteArray(docStream);

        encryptedDocStream = new ByteArrayInputStream(
            encryptData(secretKey, ByteBuffer.allocate(8).array(), docBytes));

        return this;
    }

    /**
     * Create a shared secret
     *
     * @param publicKey public encryption key of the audience for this shared secret
     *
     * @return the shared secret
     */
    public byte[] createEncryptedSharedSecret(EncryptionPublicKey publicKey) {
        return encryptKey(keyPair.getPrivateKey(), publicKey, secretKey);
    }

    /**
     * Emit an encrypted document
     *
     * @return an InputStream of the encrypted document
     *
     * @throws IllegalStateException if the document was not provided
     */
    public InputStream emit() {
        if (encryptedDocStream==null) {
            throw new IllegalStateException("Document not provided");
        }
        return encryptedDocStream;
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
     * Generate an encryption key suitable for this builder.
     */
    private static native byte[] generateEncryptionKey();

    /**
     * Encrypt data in the input buffer, returning an encrypted buffer.
     *
     * @param key   The key to use to encrypt this data.
     * @param iv    The initialization vector to use.
     * @param input The input to encrypt.
     *
     * @return the encrypted data with iv / HMAC prepended / appended.
     */
    private static native byte[] encryptData(byte[] key, byte[] iv, byte[] input);

    /**
     * Encrypt the key with a long-term key between two entities.
     *
     * @param localPrivateKey   The private key for this entity.
     * @param peerPublicKey     The public key of the peer.
     * @param inputKey          The input key to be encrypted.
     *
     * @return the encrypted key with iv / HMAC prepended / appended.
     */
    private static native byte[] encryptKey(
            EncryptionPrivateKey localPrivateKey, EncryptionPublicKey peerPublicKey,
            byte[] inputKey);

}
