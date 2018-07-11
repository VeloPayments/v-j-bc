package com.velopayments.blockchain.client;

import com.velopayments.blockchain.cert.Certificate;
import java.util.concurrent.*;

/**
 * Transaction request managed by an AgentConnection.
 */
public class TransactionSubmissionRequest {

    public TransactionSubmissionRequest(Certificate txnData) {
        this.txnData = txnData;
        this.statusFuture = new FutureTask<TransactionStatus>(() -> {
                return this.getStatus();
            });
        this.status = TransactionStatus.PENDING;
    }

    public TransactionStatus getStatus() {
        return status;
    }

    public void setStatus(TransactionStatus status) {
        this.status = status;
        this.statusFuture.run();
    }

    public Future<TransactionStatus> getStatusFuture() {
        return statusFuture;
    }

    public Certificate getTxnData() {
        return txnData;
    }

    private Certificate txnData;
    private FutureTask<TransactionStatus> statusFuture;
    private TransactionStatus status;
}
