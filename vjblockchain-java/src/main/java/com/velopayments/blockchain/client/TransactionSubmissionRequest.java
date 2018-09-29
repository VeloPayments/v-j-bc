package com.velopayments.blockchain.client;

import com.velopayments.blockchain.cert.Certificate;
import java.util.concurrent.*;

/**
 * Transaction request managed by an AgentConnection.
 */
public class TransactionSubmissionRequest {

    public TransactionSubmissionRequest(Certificate txnData) {
        this.txnData = txnData;
        this.statusFuture = new CompletableFuture<TransactionStatus>();
        this.status = TransactionStatus.PENDING;
    }

    public TransactionStatus getStatus() {
        return status;
    }

    public void setStatus(TransactionStatus status) {
        this.status = status;
        this.statusFuture.complete(status);
    }

    public CompletableFuture<TransactionStatus> getStatusFuture() {
        return statusFuture;
    }

    public Certificate getTxnData() {
        return txnData;
    }

    private Certificate txnData;
    private CompletableFuture<TransactionStatus> statusFuture;
    private TransactionStatus status;
}
