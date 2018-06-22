package com.velopayments.blockchain.init;

public final class Initializer {

    /**
     * Private constructor.
     */
    private Initializer() {
    }

    /**
     * Initialize this library.  This method must be called exactly once during
     * application initialization.
     */
    public static synchronized void init() {
        System.loadLibrary("vjblockchain-native");

        /* attempt to initialize vjblockchain */
        if (!blockchainInit()) {
            /* unspecified error that couldn't be thrown. */
            throw new IllegalStateException("Bad vjblockchain registration.");
        }
    }

    /**
     * Initialize internal descriptors used by this library.
     *
     * @return true on success.  Returns false or throws exception on failure.
     */
    private static native boolean blockchainInit();
}
