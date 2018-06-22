package com.velopayments.blockchain.cert;

import java.util.UUID;

public interface CertificateParserDelegate {

    /**
     * Resolve an entity referenced by UUID.
     *
     * @param entityId      The UUID of the entity to resolve.
     * @param height        The height of the blockchain at which the entity
     *                      should be resolved.
     * @return an EntityReference containing the entity's public encryption and
     * signing keys.
     */
    EntityReference resolveEntity(UUID entityId, long height)
    throws UnknownEntityException;

    /**
     * Get the integer state of the artifact, based on the current blockchain
     * state of this artifact.
     *
     * @param artifactId        The UUID of the artifact.
     * @return the artifact state of the given artifact.
     */
    ArtifactState resolveArtifactState(UUID artifactId, long height)
    throws UnknownArtifactException;

    /**
     * Get the contract for the given artifact type and transaction type.
     *
     * @param artifactTypeId    The artifact type UUID.
     * @param transactionTypeId The UUID of the transaction.
     *
     * @return the contract for this artifact type and transaction type.
     */
    CertificateContract resolveEntityContract(
        UUID artifactTypeId, UUID transactionTypeId)
    throws UnknownArtifactTypeException, UnknownTransactionTypeException;
}
