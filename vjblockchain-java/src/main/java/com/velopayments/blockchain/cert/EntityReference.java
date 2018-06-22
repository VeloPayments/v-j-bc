package com.velopayments.blockchain.cert;

import java.util.UUID;

/**
 * Reference to an entity, including the public and private keys.
 */
public class EntityReference {

    private byte[] publicEncryptionKey;
    private byte[] publicSigningKey;
    private UUID artifactType;

    /**
     * Create an entity reference from a public encryption key byte buffer and a
     * public signing key byte buffer.
     *
     * @param publicEncryptionKey The public encryption key.
     * @param publicSigningKey    The public signing key.
     * @param artifactType        The type of this entity.
     */
    public EntityReference(
        byte[] publicEncryptionKey, byte[] publicSigningKey, UUID artifactType) {

        this.publicEncryptionKey = publicEncryptionKey;
        this.publicSigningKey = publicSigningKey;
        this.artifactType = artifactType;
    }

    /**
     * Get the public encryption key associated with this entity.
     *
     * @return the public encryption key associated with this entity.
     */
    public byte[] getPublicEncryptionKey() {
        return publicEncryptionKey;
    }

    /**
     * Get the public signing key associated with this entity.
     *
     * @return the public signing key associated with this entity.
     */
    public byte[] getPublicSigningKey() {
        return publicSigningKey;
    }

    /**
     * Get the artifact type associated with this entity.
     *
     * @return the artifact type associated with this entity.
     */
    public UUID getArtifactType() {
        return artifactType;
    }
}
