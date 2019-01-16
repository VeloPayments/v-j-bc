package com.velopayments.blockchain.document;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class EncryptedDocumentBuilder {

    public static final int BUFFER_SIZE = 4096;

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

        ByteArrayOutputStream bos = new ByteArrayOutputStream();

        int r,offset = 0;
        byte[] buffer = new byte[BUFFER_SIZE];
        byte[] iv = ByteBuffer.allocate(8).array(); // filled with 0
        while ((r = docStream.read(buffer, 0, buffer.length)) != -1) {

            byte[] chunk = Arrays.copyOf(buffer, r);

            // encrypt chunk
            byte[] encryptedChunk = encryptData(secretKey, iv, chunk, offset++);

            // write encrypted bytes to outputstream
            bos.write(encryptedChunk);
        }

        // convert (encrypted) outputstream to inputstream
        encryptedDocStream = new ByteArrayInputStream(bos.toByteArray());

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
     * Generate an encryption key suitable for this builder.
     */
    private static native byte[] generateEncryptionKey();

    /**
     * Encrypt data in the input buffer, returning an encrypted buffer.
     *
     * @param key   The key to use to encrypt this data.
     * @param iv    The initialization vector to use.
     * @param input The input to encrypt.
     * @param offset
     *
     * @return the encrypted data with iv / HMAC prepended / appended.
     */
    private static native byte[] encryptData(byte[] key, byte[] iv, byte[] input, int offset);

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
