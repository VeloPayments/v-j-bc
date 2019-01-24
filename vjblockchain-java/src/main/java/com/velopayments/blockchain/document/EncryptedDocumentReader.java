package com.velopayments.blockchain.document;


import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;

import java.io.*;
import java.util.Arrays;

public class EncryptedDocumentReader {

    private byte[] secretKey;

    private InputStream source;
    private OutputStream destination;


    /**
     * Create an EncryptedDocumentReader from a private key, a public key, a shared secret
     * and an InputStream.
     *
     * @param localPrivateKey     The private key of the entity reading this document.
     * @param peerPublicKey       The public key of the peer that created this document.
     * @param sharedSecret        The shared secret produced for the entity reading this document.
     */
    public EncryptedDocumentReader(EncryptionPrivateKey localPrivateKey, EncryptionPublicKey peerPublicKey,
                                   byte[] sharedSecret) {

        this.secretKey = decryptKey(localPrivateKey, peerPublicKey, sharedSecret);
    }

    /**
     * Set the document to be decrypted
     *
     * @param source InputStream of the document to be decrypted
     *
     * @return this builder for additional operations
     *
     */
    public EncryptedDocumentReader withSource(InputStream source) {
        this.source = source;
        return this;
    }

    /**
     * Set the destination the decrypted document should be written to
     *
     * @param destination OutputStream the decrypted document should be written to
     *
     * @return this builder for additional operations
     *
     */
    public EncryptedDocumentReader withDestination(OutputStream destination) {
        this.destination = destination;
        return this;
    }


    /**
     * Emit the decrypted document to the destination stream
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


        // unpack this in the same way it was packed up
        int r,offset = 0;
        byte[] buffer = new byte[EncryptedDocumentBuilder.BUFFER_SIZE];

        // the first 8 bytes are the IV
        byte[] iv = new byte[8];
        source.read(iv, 0, iv.length);

        while ((r = source.read(buffer, 0, buffer.length)) != -1) {
            byte[] chunk;
            if (0 == offset) {
                chunk = Arrays.copyOf(iv, iv.length + r);
                System.arraycopy(buffer, 0, chunk, iv.length, r);
            } else {
                chunk = Arrays.copyOf(buffer, r);
            }
            byte[] decrypted = decryptData(secretKey, iv, chunk, offset);
            offset += r;

            destination.write(decrypted);
        }
    }

    /**
     * Decrypt the input value using the provided secret key.
     *
     * @param secretKey     The secret key to use to decrypt this value.
     * @param iv            The initialization vector to use
     * @param input         The input value to decrypt.
     * @param offset
     *
     * @return the decrypted value.
     */
    private static native byte[] decryptData(byte[] secretKey, byte[] iv, byte[] input, int offset);

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
    private static native byte[] decryptKey(
            EncryptionPrivateKey localPrivateKey, EncryptionPublicKey peerPublicKey,
            byte[] encryptedKey);

}
