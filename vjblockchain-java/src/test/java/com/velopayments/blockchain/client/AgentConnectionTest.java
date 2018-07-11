package com.velopayments.blockchain.client;

import com.velopayments.blockchain.cert.*;
import com.velopayments.blockchain.crypt.*;
import java.nio.ByteBuffer;
import java.io.File;
import java.io.IOException;
import java.util.concurrent.Future;
import java.util.Optional;
import java.util.UUID;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;
import static org.mockito.Mockito.*;

public class AgentConnectionTest {

    private static short PRIVATE_KEY_SHORTS[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    private static EncryptionPrivateKey PRIVATE_KEY;
    private static short PRIVATE_SIGNING_KEY_SHORTS[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
    };
    private static SigningPrivateKey PRIVATE_SIGNING_KEY;

    UUID entityId = UUID.fromString("89921ea8-5cb8-4121-9f4f-4ab94c2a7bfd");
    UUID zeroUUID = UUID.fromString("00000000-0000-0000-0000-000000000000");
    UUID DUMMY_TXN_TYPE =
        UUID.fromString("c6ea3c21-8a74-45ad-b13c-c0f94bb288a7");

    /**
     * Create a temporary folder for testing.
     */
    @Rule
    public TemporaryFolder tempDir = new TemporaryFolder();

    /**
     * Test that it is possible to connect to a new AgentConnection and get the
     * root block ID as the latest block ID.
     */
    @Test
    public void emptyBlockchainHasLatestBlockAsRootBlock() throws Exception {
        File dbLoc = tempDir.newFolder();

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            assertThat(conn.getLatestBlockId(), is(zeroUUID));
        } finally {
            if (null != conn) conn.close();
        }
    }

    /**
     * Test that, by default, getBlockById() returns Empty when a block with a
     * given ID is not found.
     */
    @Test
    public void getBlockByIdReturnsEmptyForMissingId() throws Exception {
        File dbLoc = tempDir.newFolder();
        UUID missingBlockId =
            UUID.fromString("d2ad046d-6b76-430b-9bc7-f1268287b6e9");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            assertThat(
                conn.getBlockById(missingBlockId).isPresent(), is(false));
        } finally {
            if (null != conn) conn.close();
        }
    }

    /**
     * Test that, by default, getTransactionById() returns Empty when a
     * transaction with a given ID is not found.
     */
    @Test
    public void getTransactionByIdReturnsEmptyForMissingId() throws Exception {
        File dbLoc = tempDir.newFolder();
        UUID missingBlockId =
            UUID.fromString("d2ad046d-6b76-430b-9bc7-f1268287b6e9");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            assertThat(
                conn.getTransactionById(missingBlockId).isPresent(), is(false));
        } finally {
            if (null != conn) conn.close();
        }
    }

    /**
     * Test that submitting a transaction returns a Future.
     */
    @Test
    public void transactionSubmissionReturnsAFuture() throws Exception {
        File dbLoc = tempDir.newFolder();
        UUID dummyTxnId =
            UUID.fromString("59c1b17f-6d34-4f28-b227-dd9609468aed");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            byte[] dummyTxn = makeDummyTxn(DUMMY_TXN_TYPE, dummyTxnId);
            Future<TransactionStatus> stat =
                conn.submit(Certificate.fromByteArray(dummyTxn));
            assertThat(stat, is(notNullValue()));
        } finally {
            if (null != conn) conn.close();
        }
    }

    /**
     * Test that canceling the current transactions causes a transaction status
     * Future to be resolved as TransactionStatus.CANCELED.
     */
    @Test
    public void cancelTransactionsTest() throws Exception {
        File dbLoc = tempDir.newFolder();
        UUID dummyTxnId =
            UUID.fromString("59c1b17f-6d34-4f28-b227-dd9609468aed");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            byte[] dummyTxn = makeDummyTxn(DUMMY_TXN_TYPE, dummyTxnId);
            Future<TransactionStatus> stat =
                conn.submit(Certificate.fromByteArray(dummyTxn));
            assertThat(stat, is(notNullValue()));

            /* cancel transactions. */
            conn.cancelTransactions();

            /* the status future is now complete. */
            assertThat(stat.isDone(), is(true));
            /* the transaction is now canceled. */
            assertThat(stat.get(), is(TransactionStatus.CANCELED));

        } finally {
            if (null != conn) conn.close();
        }
    }

    /**
     * Test that committing the current transactions causes a transaction status
     * Future to be resolved as TransactionStatus.SUCCEEDED.  Furthermore,
     * getting the transaction from the agent connection succeeds.
     */
    @Test
    public void commitTransactionsTest() throws Exception {
        File dbLoc = tempDir.newFolder();
        UUID dummyTxnId =
            UUID.fromString("59c1b17f-6d34-4f28-b227-dd9609468aed");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            /* Precondition: the transaction should not exist in the blockchain.
             */
            assertThat(
                conn.getTransactionById(dummyTxnId).isPresent(), is(false));

            byte[] dummyTxn = makeDummyTxn(DUMMY_TXN_TYPE, dummyTxnId);
            Future<TransactionStatus> stat =
                conn.submit(Certificate.fromByteArray(dummyTxn));
            assertThat(stat, is(notNullValue()));

            /* commit transactions. */
            conn.commitTransactions();

            /* the status future is now complete. */
            assertThat(stat.isDone(), is(true));
            /* the transaction is now canceled. */
            assertThat(stat.get(), is(TransactionStatus.SUCCEEDED));

            /* we should be able to get this transaction from the agent
             * connection. */
            assertThat(
                conn.getTransactionById(dummyTxnId).isPresent(), is(true));

            /* get the new block UUID. */
            UUID newBlockId = conn.getLatestBlockId();

            /* the latest block in the blockchain should not be the root block.
             */
            assertThat(
                newBlockId, not(zeroUUID));

            /* we should be able to get the latest block from the agent
             * connection. */
            assertThat(
                conn.getBlockById(newBlockId).isPresent(),
                is(true));

            /* given the zero UUID, the next UUID should be the newBlockId. */
            assertThat(
                conn.getNextBlockId(zeroUUID).orElseThrow(
                    () -> new IOException()),
                is(newBlockId));

            /* given the zero UUID, the previous UUID should be empty. */
            assertThat(
                conn.getPrevBlockId(zeroUUID).isPresent(),
                is(false));

            /* given the new block UUID, the previous UUID should be the zero
             * UUID. */
            assertThat(
                conn.getPrevBlockId(newBlockId).orElseThrow(
                    () -> new IOException()),
                is(zeroUUID));

            /* given the new block UUID, the next UUID should be empty. */
            assertThat(
                conn.getNextBlockId(newBlockId).isPresent(),
                is(false));

            /* given the transaction id, the block id should be the new block
             * id.
             */
            assertThat(
                conn.getTransactionBlockId(dummyTxnId).orElseThrow(
                    () -> new IOException()),
                is(newBlockId));

        } finally {
            if (null != conn) conn.close();
        }
    }

    static {
        PRIVATE_KEY =
            new EncryptionPrivateKey(convShorts(PRIVATE_KEY_SHORTS).array());
        PRIVATE_SIGNING_KEY =
            new SigningPrivateKey(
                convShorts(PRIVATE_SIGNING_KEY_SHORTS).array());
    }

    /**
     * Helper method to make a dummy transaction with a given UUID.
     */
    private byte[] makeDummyTxn(UUID txnType, UUID txnId) {
        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(
                CertificateType.TRANSACTION);
        builder.addUUID(Field.TRANSACTION_TYPE, txnType);
        builder.addUUID(Field.CERTIFICATE_ID, txnId);
        return builder.sign(entityId, PRIVATE_SIGNING_KEY);
    }

    /**
     * Helper method to build packed ByteBuffer from array of short hex values.
     */
    private static ByteBuffer convShorts(short in[]) {
        ByteBuffer retVal = ByteBuffer.allocate(in.length);
        for (short val : in) {
            retVal.put((byte) val);
        }

        return retVal;
    }
}
