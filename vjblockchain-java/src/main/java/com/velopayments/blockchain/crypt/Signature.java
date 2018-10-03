package com.velopayments.blockchain.crypt;

public class Signature {

    /**
     * Create a signature from a byte array.
     *
     * @param signatureBytes The raw bytes for this signature.
     */
    public Signature(byte[] signatureBytes) {
        this.signatureBytes = signatureBytes;
    }

    /**
     * Get the raw bytes for this signature.
     */
    public byte[] getSignatureBytes() {
        return this.signatureBytes;
    }

    private byte[] signatureBytes;
}
