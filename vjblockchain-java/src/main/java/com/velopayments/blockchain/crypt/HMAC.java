package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.init.Initializer;

/**
 * HMAC.  This class exposes the HMAC algorithm defined in the Velochain
 * specification.  The constructor takes a variable length key as input.
 * The createHMACLong function accepts a byte array representing a message
 * to MAC as input, and returns a 64 byte message authentication code.
 * The createHMACShort function returns a 32 byte message authentication
 * code.
 */
public class HMAC {

    static {
        Initializer.init();
    }

    private byte[] key;

    public HMAC(byte[] key) {
        this.key = key;
    }

    /**
     * Create a long (64 byte) message authentication code.
     *
     * @param message the message
     * @return the MAC
     */
    public byte[] createHMACLong(byte[] message) {
        return digestNative(key, message, false);
    }

    /**
     * Create a short (32 byte) message authentication code.
     *
     * @param message the message
     * @return the MAC
     */
    public byte[] createHMACShort(byte[] message) {
        return digestNative(key, message, true);
    }


    private native byte[] digestNative(byte[] key, byte[]  message, boolean shortHmac);

}
