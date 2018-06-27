package com.velopayments.blockchain.db;

public class DatabaseRegistry {

    /**
     * Register an adapter handle and engine name with the database registry.
     *
     * @param handle        The database adapter handle to register.
     * @param engine        The name of the engine.
     */
    public static native void register(long handle, String engine);
}
