package com.velopayments.blockchain.cert;

/**
 * Wrapper around byte array to indicate that this array should be treated as a
 * certificate.
 */
public class Certificate {

    /**
     * Create a Certificate from a byte array.
     *
     * @param byteArray         The array from which this certificate is
     *                          created.
     */
    private Certificate(byte[] byteArray) {
        this.byteArray = byteArray;
    }

    /**
     * Static method to create a Certificate from a byte array.
     *
     * @param byteArray         The array from which this certificate is
     *                          created.
     *
     * @return a Certificate backed by this byte array.
     */
    public static Certificate fromByteArray(byte[] byteArray) {
        return new Certificate(byteArray);
    }

    /**
     * Get the bytes backing this Certificate.
     *
     * @return the bytes backing this certificate.
     */
    public byte[] toByteArray() {
        return this.byteArray;
    }

    private byte[] byteArray;
}
