package com.velopayments.blockchain.client;

import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.init.Initializer;
import java.io.IOException;
import java.util.concurrent.*;
import java.util.*;

/**
 * Connect to an agent and perform basic client functions.
 */
public class AgentConnection {

    /**
     * Connect using a given connection string, authenticating using the given
     * uuid and private encryption key.
     *
     * Note that this AgentConnection MUST be closed by calling
     * AgentConnection#close() in a finally block or using a with-pattern.
     *
     * @param connect       The connection string to use to connect to the
     *                      agent.
     * @param entityId      The entity UUID.
     * @param privateEncKey The private encryption key for this entity.
     */
    public AgentConnection(String connect, UUID entityId,
                           EncryptionPrivateKey privateEncKey) 
            throws IOException {

        this.handle = 0;
        this.connect = connect;
        this.entityId = entityId;
        this.privateEncKey = privateEncKey;
        this.submissionList = new LinkedList<TransactionSubmissionRequest>();
        this.handle = connectNative(connect);
    }

    /**
     * Commit the current transactions to the blockchain, creating a new block
     * if necessary.
     */
    public void commitTransactions() throws IOException {

        /* make a block with any remaining transactions. */
        if (submissionList.size() > 0) {
            makeBlockNative();
        }
    }

    /**
     * Cancel the current transactions to the blockchain.
     */
    public void cancelTransactions() throws IOException {

        for (TransactionSubmissionRequest req : submissionList) {
            req.setStatus(TransactionStatus.CANCELED);
        }

        submissionList.clear();
    }

    /**
     * Close the client connection.
     */
    public void close() throws IOException {

        /* commit outstanding transactions. */
        commitTransactions();

        /* clean up. */
        closeNative(handle);
        handle = 0;
    }

    /**
     * Submit a transaction to the blockchain / agent.
     *
     * This method returns a future that indicates the status of the
     * transaction.  The Future#get() method must be called either on a separate
     * thread from the thread used by this AgentConnection, or only after the
     * connection has been closed.  Note that the Future can only be completed
     * successfully if a status update is received while the connection is
     * valid.  Otherwise, it will be completed with a status code indicating
     * that the status for this transaction was unknown.  This mechanism exists
     * only as a convenience.  At any point, the status of a transaction can be
     * queried using AgentConnection#getTransactionStatus().
     *
     * @param transaction       The transaction to be added.
     *
     * @return a Future which evaluates into the status of the transaction.
     */
    public Future<TransactionStatus> submit(byte[] transaction)
            throws IOException {

        /* drain the list by creating a block if it has 100 requests. */
        if (submissionList.size() >= 100) {
            makeBlockNative();
        }

        /* create and store a submission request. */
        TransactionSubmissionRequest req =
            new TransactionSubmissionRequest(transaction);
        submissionList.add(req);

        /* the future will hold the status of this request. */
        return req.getStatusFuture();
    }

    /**
     * Get the latest block UUID in the blockchain, according to this agent.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @return the latest block UUID.
     */
    public UUID getLatestBlockId() throws IOException {

        return getLatestBlockIdNative();
    }

    /**
     * Get the block transaction for a given UUID.
     *
     * Note - this method blocks until the given block is available.
     *
     * @param blockId   The transaction ID to look up.
     *
     * @return the raw bytes for a given block, or empty if not found.
     */
    public Optional<byte[]> getBlockById(UUID blockId) throws IOException {

        return getBlockByIdNative(blockId);
    }

    /**
     * Get a transaction for a given UUID.
     *
     * Note - this method blocks until the given transaction is available.
     *
     * @param txnId     The transaction ID to look up.
     *
     * @return the raw bytes for a given transaction, or empty() if not found.
     */
    public Optional<byte[]> getTransactionById(UUID txnId)
            throws IOException {

        return getTransactionByIdNative(txnId);
    }

    /**
     * Connect to the agent using the given connection string.
     *
     * @param connect       The connection string to use to connect to the
     *                      agent.
     *
     * @return the handle for this connection.
     */
    private native long connectNative(String connect) throws IOException;

    /**
     * Close the connection to the agent.
     *
     * @param handle        The handle to the agent to be closed.
     */
    private native void closeNative(long handle) throws IOException;

    /**
     * Make a block using the current list of transactions.
     */
    private native void makeBlockNative() throws IOException;

    /**
     * Get the latest block UUID in the blockchain, according to this agent.
     *
     * Note - this method blocks until the block id is retrieved.
     *
     * @return the latest block UUID.
     */
    private native UUID getLatestBlockIdNative() throws IOException;

    /**
     * Get the block transaction for a given UUID.
     *
     * Note - this method blocks until the given block is available.
     *
     * @param blockId   The transaction ID to look up.
     *
     * @return the raw bytes for a given block, or empty if not found.
     */
    public native Optional<byte[]>
    getBlockByIdNative(UUID blockId) throws IOException;

    /**
     * Get a transaction for a given UUID.
     *
     * Note - this method blocks until the given transaction is available.
     *
     * @param txnId     The transaction ID to look up.
     *
     * @return the raw bytes for a given transaction, or empty() if not found.
     */
    public native Optional<byte[]>
    getTransactionByIdNative(UUID txnId) throws IOException;

    static {
        Initializer.init();
    }

    /**
     * The connection string.
     */
    private String connect;

    /**
     * The client entity UUID.
     */
    private UUID entityId;

    /**
     * The private encryption key for the client entity.
     */
    private EncryptionPrivateKey privateEncKey;

    /**
     * The handle to the agent.
     */
    private long handle;

    /**
     * List of transactions that have been submitted.
     */
    private List<TransactionSubmissionRequest> submissionList;
}
