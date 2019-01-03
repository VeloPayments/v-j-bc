package com.velopayments.blockchain.document;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import org.apache.commons.io.IOUtils;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class EncryptedDocumentBuilder {

    private EncryptionKeyPair keyPair;
    private byte[] secretKey;
    private InputStream encryptedDocStream;

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
     * Set the raw bytes of the document
     *
     * @param docStream The document to be encrypted
     * @return this builder for additional operations.
     */
    public EncryptedDocumentBuilder withDocument(InputStream docStream) throws IOException {

        // TODO: don't need IV
        encryptedDocStream = new ByteArrayInputStream(
                encryptData(secretKey, ByteBuffer.allocate(8).array(), IOUtils.toByteArray(docStream)));

        return this;
    }

    public byte[] createEncryptedSharedSecret(EncryptionPublicKey publicKey) {
        return encryptKey(keyPair.getPrivateKey(), publicKey, secretKey);
    }

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
