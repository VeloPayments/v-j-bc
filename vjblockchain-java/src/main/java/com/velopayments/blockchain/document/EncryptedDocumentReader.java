package com.velopayments.blockchain.document;


import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.init.Initializer;

import java.io.*;
import java.util.Arrays;

public class EncryptedDocumentReader {

    private byte[] secretKey;

    private InputStream source;
    private OutputStream destination;

    /**
     * Create an EncryptedDocumentReader from a private key, a public key, a
     * shared secret and an InputStream.
     *
     * @param localPrivateKey     The private key of the entity reading this
     *                            document.
     * @param peerPublicKey       The public key of the peer that created this
     *                            document.
     * @param sharedSecret        The shared secret produced for the entity
     *                            reading this document.
     */
    public EncryptedDocumentReader(
        EncryptionPrivateKey localPrivateKey, EncryptionPublicKey peerPublicKey,
        byte[] sharedSecret) {

        this.secretKey =
            decryptKey(
                Initializer.getInstance(), localPrivateKey, peerPublicKey,
                sharedSecret);
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
     * Read from the source stream, decrypting the data and writing to the 
     * destination stream
     *
     * @throws IllegalStateException if the source or destination
     *  streams were not provided
     */
    public void decrypt() throws IOException {
        if (source==null) {
            throw new IllegalStateException("source not provided");
        }
        if (destination==null) {
            throw new IllegalStateException("destination not provided");
        }


        // unpack this in the same way it was packed up
        int r;
        long offset = 0;
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
            byte[] decrypted =
                decryptData(
                    Initializer.getInstance(), secretKey, iv, chunk, offset);
            offset += r;

            destination.write(decrypted);
        }
    }

    /**
     * Decrypt the input value using the provided secret key.
     *
     * @param nativeInst    The native instance pointer.
     * @param secretKey     The secret key to use to decrypt this value.
     * @param iv            The initialization vector to use
     * @param chunk         The input value to decrypt.
     * @param offset        The number of bytes previously decrypted.
     *
     * @return the decrypted value.
     */
    private static native byte[] decryptData(
            long nativeInst, byte[] secretKey, byte[] iv, byte[] chunk,
            long offset);

    /**
     * Recover the secret key from the given local private key, peer public key,
     * and encrypted key.
     *
     * @param nativeInst        The native instance pointer.
     * @param localPrivateKey   The local private key.
     * @param peerPublicKey     The peer public key.
     * @param encryptedKey      The encrypted key to decrypt.
     *
     * @return the decrypted key.
     */
    private static native byte[] decryptKey(
            long nativeInst, EncryptionPrivateKey localPrivateKey,
            EncryptionPublicKey peerPublicKey, byte[] encryptedKey);
}
