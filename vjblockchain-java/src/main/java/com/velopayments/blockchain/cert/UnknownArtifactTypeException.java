package com.velopayments.blockchain.cert;

import java.util.UUID;

public class UnknownArtifactTypeException extends Exception {

    static final long serialVersionUID = 1;

    /**
     * Create an UnknownArtifactTypeException from an error String.
     *
     * @param msg                   The error message for this exception.
     */
    public UnknownArtifactTypeException(String msg) {
        super(msg);
    }

    /**
     * Create an UnknownArtifactTypeException from an artifact type UUID.
     *
     * @param artifactTypeId        The artifact typeUUID.
     */
    public UnknownArtifactTypeException(UUID artifactTypeId) {
        super("Unknown Artifact Type " + artifactTypeId.toString());
    }
}
