package com.velopayments.blockchain.cert;

import java.util.UUID;

public final class CertificateType {

    /**
     * The Root block is the initial block in the blockchain.
     */
    public static final UUID ROOT_BLOCK =
        UUID.fromString("a231383d-a63d-4743-86aa-61fb03a38f39");

    /**
     * The Root Entity Create Transaction creates the root entity.
     */
    public static final UUID ROOT_ENTITY_CREATE =
        UUID.fromString("1f2e615b-585b-46cc-9ffc-95d618c11b92");

    /**
     * The Root Entity Destroy Transaction destroys the root entity.
     */
    public static final UUID ROOT_ENTITY_DESTROY =
        UUID.fromString("b5fc204c-f544-4c30-a53b-c97bbd33b8c6");

    /**
     * The Block Transaction serializes a block in the blockchain.
     */
    public static final UUID BLOCK =
        UUID.fromString("734eacd2-8b13-4a37-aa02-bef5628a6c68");

    /**
     * The Transaction represents a state change to an individual artifact.
     */
    public static final UUID TRANSACTION =
        UUID.fromString("52a7f0fb-8a6b-4d03-86a5-7f612fcf7eff");

    /**
     * The Private Entity represents private data associated with an entity.
     */
    public static final UUID PRIVATE_ENTITY =
        UUID.fromString("814e6a74-87aa-4595-9d31-bcc627cfe44e");

    /**
     * The Agent Subtype Identifier is used for consensus messages between block agents.
     */
    public static final UUID AGENT_SUBTYPE = UUID.fromString("9985d937-31d4-4aa7-8222-c317878d5373");

    /**
     * Can't construct this class.
     */
    private CertificateType() {
        throw new AssertionError();
    }
}
