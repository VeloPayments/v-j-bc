package com.velopayments.blockchain.cert;

import java.util.UUID;

public class UnknownArtifactException extends RuntimeException {

    static final long serialVersionUID = 1;

    /**
     * Create an UnknownArtifactException from an error String.
     *
     * @param msg The error message for this exception.
     */
    public UnknownArtifactException(String msg) {
        super(msg);
    }

    /**
     * Create an UnknownArtifactException from an artifact UUID.
     *
     * @param artifactId The artifact UUID.
     */
    public UnknownArtifactException(UUID artifactId) {
        super("Unknown Artifact " + artifactId.toString());
    }
}
