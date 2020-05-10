package com.velopayments.blockchain.init;

import java.io.*;

public final class Initializer {

    /**
     * Private constructor.
     */
    private Initializer() {
    }

    public static synchronized long getInstance() {

        /* create the instance once. */
        if (!initialized) {
            init();
            initialized = true;
        }

        /* return the native instance pointer. */
        return nativeInstance;
    }

    /**
     * Initialize this library.  This method must be called exactly once during
     * application initialization.
     */
    private static void init() {
        if (!loaded) {
            InputStream is = null;
            OutputStream os = null;
            File libFile = null;

            try {
                is =
                    Initializer.class
                       .getResourceAsStream("/libvjblockchain-native.so");
                libFile = File.createTempFile("lib", ".so");
                os = new FileOutputStream(libFile);
                byte[] buffer = new byte[16384];
                int length;

                while ((length = is.read(buffer)) != -1) {
                    os.write(buffer, 0, length);
                }

                System.load(libFile.getPath());

                /* attempt to initialize vjblockchain */
                nativeInstance = blockchainInit();
                if (0 == nativeInstance) {
                    /* unspecified error that couldn't be thrown. */
                    throw new IllegalStateException("Bad vjblockchain registration.");
                }

                loaded = true;
            } catch (IOException e) {
                throw new IllegalStateException("Could not load class lib", e);
            } finally {
                if (null != is) {
                    try { is.close(); } catch (IOException e) { }
                }
                if (null != os) {
                    try { os.close(); } catch (IOException e) { }
                }
                if (null != libFile) {
                    libFile.deleteOnExit();
                }
            }
        }
    }

    private static boolean initialized = false;
    private static long nativeInstance = 0;

    /**
     * Initialize internal descriptors used by this library.
     *
     * @return the address of the java native instance.
     */
    private static native long blockchainInit();

    /**
     * Set to true the first time this is loaded.
     */
    private static boolean loaded = false;
}
