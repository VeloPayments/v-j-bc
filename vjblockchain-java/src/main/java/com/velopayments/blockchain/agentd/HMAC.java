package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.init.Initializer;

public class HMAC {

    static {
        Initializer.init();
    }

    private byte[] key;

    public HMAC(byte[] key) {
        this.key = key;
    }


    public byte[] digest(byte[] message) {
        return digestNative(key, message);
    }

    private native byte[] digestNative(byte[] key, byte[]  message);

}
