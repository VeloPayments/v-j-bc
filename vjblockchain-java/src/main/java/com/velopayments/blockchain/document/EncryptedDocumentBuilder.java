package com.velopayments.blockchain.document;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.io.*;
import java.security.SecureRandom;
import java.util.Arrays;

public class EncryptedDocumentBuilder {

    public static final int BUFFER_SIZE = 4096;

    private EncryptionKeyPair keyPair;
    private byte[] secretKey;

    private InputStream source;
    private OutputStream destination;


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
     * @param source InputStream of the document to be encrypted
     *
     * @return this builder for additional operations
     *
     */
    public EncryptedDocumentBuilder withSource(InputStream source) {
        this.source = source;
        return this;
    }

    /**
     * Set the destination the encrypted document should be written to
     *
     * @param destination OutputStream the encrypted document should be written to
     *
     * @return this builder for additional operations
     *
     */
    public EncryptedDocumentBuilder withDestination(OutputStream destination) {
        this.destination = destination;
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
     * Emit the encrypted document to the destination stream
     *
     * @throws IllegalStateException if the source or destination
     *  streams were not provided
     */
    public void emit() throws IOException {
        if (source==null) {
            throw new IllegalStateException("source not provided");
        }
        if (destination==null) {
            throw new IllegalStateException("destination not provided");
        }

        int r,offset = 0;
        byte[] buffer = new byte[BUFFER_SIZE];

        // generate a random initialization vector
        SecureRandom sr = new SecureRandom();
        byte[] iv = new byte[8];
        sr.nextBytes(iv);

        while ((r = source.read(buffer, 0, buffer.length)) != -1) {

            byte[] chunk = Arrays.copyOf(buffer, r);

            // encrypt chunk
            byte[] encryptedChunk = encryptData(secretKey, iv, chunk, offset);

            // increment offset by bytes written
            offset += r;

            // write encrypted bytes to outputstream
            destination.write(encryptedChunk);
        }
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
