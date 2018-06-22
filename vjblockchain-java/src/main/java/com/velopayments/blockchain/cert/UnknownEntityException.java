package com.velopayments.blockchain.cert;

import java.util.UUID;

public class UnknownEntityException extends Exception {

    static final long serialVersionUID = 1;

    /**
     * Create an UnknownEntityException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public UnknownEntityException(String msg) {
        super(msg);
    }

    /**
     * Create an UnknownEntityException from an entity UUID.
     *
     * @param entityId The entity UUID.
     */
    public UnknownEntityException(UUID entityId) {
        super("Unknown Entity " + entityId.toString());
    }
}
