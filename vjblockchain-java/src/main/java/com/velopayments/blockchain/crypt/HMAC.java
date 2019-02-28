package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.init.Initializer;

public class HMAC {

    static {
        Initializer.init();
    }

    private byte[] key;

    public HMAC(byte[] key) {
        this.key = key;
    }


    public byte[] createHMACLong(byte[] message) {
        return digestNative(key, message, false);
    }

    public byte[] createHMACShort(byte[] message) {
        return digestNative(key, message, true);
    }


    private native byte[] digestNative(byte[] key, byte[]  message, boolean shortHmac);

}
