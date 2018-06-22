package com.velopayments.blockchain.cert;

import java.util.UUID;

/**
 * Reference to the state of an artifact, including the last transaction ID.
 */
public class ArtifactState {

    private int artifactState;
    private UUID lastTransactionID;

    /**
     * Create an artifact state from an integer state and a UUID.
     *
     * @param artifactState     The integer state of a given artifact.
     * @param lastTransactionID The last transaction ID for the given
     *                          artifact.
     */
    public ArtifactState(int artifactState,
                         UUID lastTransactionID) {
        this.artifactState = artifactState;
        this.lastTransactionID = lastTransactionID;
    }

    /**
     * Get the integer state of the artifact.
     *
     * @return the integer state of the artifact.
     */
    public int getArtifactState() {
        return artifactState;
    }

    /**
     * Get the last transaction ID associated with this artifact.
     *
     * @return the last transaction ID associated with this artifact.
     */
    public UUID getLastTransactionID() {
        return lastTransactionID;
    }
}
