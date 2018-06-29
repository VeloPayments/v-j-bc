package com.velopayments.blockchain.client;

/**
 * Status of a transaction submitted to an AgentConnection endpoint.
 */
public enum TransactionStatus {
    /**
     * The transaction is pending.
     */
    PENDING,

    /**
     * The transaction failed.
     */
    FAILED,

    /**
     * The transaction succeeded.
     */
    SUCCEEDED,

    /**
     * The status of this transaction is unknown, because the endpoint was
     * disconnected before a status update was received.
     */
    UNKNOWN_DISCONNECTED;
}
