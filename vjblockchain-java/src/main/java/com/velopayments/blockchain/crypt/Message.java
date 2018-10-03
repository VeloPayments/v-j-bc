package com.velopayments.blockchain.crypt;

public class Message {

    /**
     * Create a Message from a raw byte array.
     *
     * @param rawBytes The raw bytes of the message.
     */
    public Message(byte[] rawBytes) {
        this.rawBytes = rawBytes;
    }

    /**
     * Get the raw bytes of a message.
     */
    public byte[] getRawBytes() {
        return rawBytes;
    }

    private byte[] rawBytes;
}
