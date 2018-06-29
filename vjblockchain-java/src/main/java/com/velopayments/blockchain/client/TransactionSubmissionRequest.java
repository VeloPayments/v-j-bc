package com.velopayments.blockchain.client;

import java.util.concurrent.*;

/**
 * Transaction request managed by an AgentConnection.
 */
public class TransactionSubmissionRequest {

    public TransactionSubmissionRequest(byte[] txnData) {
        this.txnData = txnData;
        this.statusFuture = new FutureTask<TransactionStatus>(() -> {
                return this.getStatus();
            });
        setStatus(TransactionStatus.PENDING);
    }

    public TransactionStatus getStatus() {
        return status;
    }

    public void setStatus(TransactionStatus status) {
        this.status = status;
    }

    public Future<TransactionStatus> getStatusFuture() {
        return statusFuture;
    }

    private byte[] txnData;
    private Future<TransactionStatus> statusFuture;
    private TransactionStatus status;
}
