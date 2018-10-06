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
        UUID dummyArtifactId =
            UUID.fromString("c8ff8b66-7238-45ef-8abb-4259940a0327");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            Certificate dummyTxn =
                makeDummyTxn(
                    DUMMY_TXN_TYPE, dummyTxnId, zeroUUID, dummyArtifactId);
            Future<TransactionStatus> stat = conn.submit(dummyTxn);
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
        UUID dummyArtifactId =
            UUID.fromString("a726da7b-addf-4bff-abb9-54573e65c538");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            Certificate dummyTxn =
                makeDummyTxn(
                    DUMMY_TXN_TYPE, dummyTxnId, zeroUUID, dummyArtifactId);
            Future<TransactionStatus> stat = conn.submit(dummyTxn);
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
        UUID dummyArtifactId =
            UUID.fromString("abc2aaca-8f46-4ee9-917c-1ec9205bfb90");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            /* Precondition: the transaction should not exist in the blockchain.
             */
            assertThat(
                conn.getTransactionById(dummyTxnId).isPresent(), is(false));

            Certificate dummyTxn =
                makeDummyTxn(
                    DUMMY_TXN_TYPE, dummyTxnId, zeroUUID, dummyArtifactId);
            Future<TransactionStatus> stat = conn.submit(dummyTxn);
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

    /**
     * Test that, by default, getLastTransactionIdForArtifactById() returns
     * Empty when a an artifact with the given ID does not exist.
     */
    @Test
    public void getLastTransactionIdForArtifactByIdReturnsEmpty()
            throws Exception {
        File dbLoc = tempDir.newFolder();
        UUID missingArtifactId =
            UUID.fromString("a7b7ec6f-dc9b-4fce-b44d-f35f6572af32");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            assertThat(
                conn.getLastTransactionIdForArtifactById(missingArtifactId)
                    .isPresent(),
                is(false));
        } finally {
            if (null != conn) conn.close();
        }
    }

    /**
     * Test that, if we insert a transaction and commit, we can query it by
     * artifact ID.
     */
    @Test
    public void getLastTransactionIdForArtifactById()
            throws Exception {
        File dbLoc = tempDir.newFolder();
        UUID dummyTxnId =
            UUID.fromString("aab2bf7d-e6c2-45d9-83a6-137f9fd3e9f1");
        UUID dummyArtifactId =
            UUID.fromString("ca3fb085-f0b4-416a-a9ef-dd75a979b0a1");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            /* Precondition: querying by this artifact id returns nothing. */
            assertThat(
                conn.getLastTransactionIdForArtifactById(dummyArtifactId)
                    .isPresent(),
                is(false));

            Certificate dummyTxn =
                makeDummyTxn(
                    DUMMY_TXN_TYPE, dummyTxnId, zeroUUID, dummyArtifactId);
            Future<TransactionStatus> stat = conn.submit(dummyTxn);

            /* commit transactions. */
            conn.commitTransactions();

            /* the status future is now complete. */
            assertThat(stat.isDone(), is(true));
            /* the transaction succeeded. */
            assertThat(stat.get(), is(TransactionStatus.SUCCEEDED));

            /* we should be able to get this transaction ID using the artifact
             * ID. */
            Optional<UUID> lastUUID =
                conn.getLastTransactionIdForArtifactById(dummyArtifactId);
            assertThat(lastUUID.isPresent(), is(true));
            assertThat(lastUUID.get(), is(dummyTxnId));

        } finally {
            if (null != conn) conn.close();
        }
    }

    /**
     * Test that, if we insert three transactions and commit, we can query the
     * next and previous transaction IDs for each.
     */
    @Test
    public void getPreviousNextTransactionIds()
            throws Exception {
        File dbLoc = tempDir.newFolder();
        UUID dummyTxnId1 =
            UUID.fromString("02c00a97-f0f3-4923-8c91-5e8e52536320");
        UUID dummyTxnId2 =
            UUID.fromString("5904f374-16c5-493d-b069-2d4e7bac216e");
        UUID dummyTxnId3 =
            UUID.fromString("e942a15b-7058-430a-8d83-c4fb6eac1d67");
        UUID dummyArtifactId =
            UUID.fromString("53e13c8c-7ca4-4f99-9886-aa08035347ed");

        AgentConnection conn = null;

        try {
            conn = new AgentConnection(dbLoc.getPath(), entityId, PRIVATE_KEY);

            /* Precondition: querying the next UUIDs for each transaction ID
             * returns nothing. */
            assertThat(
                conn.getNextTransactionIdForTransactionById(dummyTxnId1)
                    .isPresent(),
                is(false));
            assertThat(
                conn.getNextTransactionIdForTransactionById(dummyTxnId2)
                    .isPresent(),
                is(false));
            assertThat(
                conn.getNextTransactionIdForTransactionById(dummyTxnId3)
                    .isPresent(),
                is(false));
            assertThat(
                conn.getPreviousTransactionIdForTransactionById(dummyTxnId1)
                    .isPresent(),
                is(false));
            assertThat(
                conn.getPreviousTransactionIdForTransactionById(dummyTxnId2)
                    .isPresent(),
                is(false));
            assertThat(
                conn.getPreviousTransactionIdForTransactionById(dummyTxnId3)
                    .isPresent(),
                is(false));

            Certificate dummyTxn1 =
                makeDummyTxn(
                    DUMMY_TXN_TYPE, dummyTxnId1, zeroUUID, dummyArtifactId);
            Future<TransactionStatus> stat1 = conn.submit(dummyTxn1);
            Certificate dummyTxn2 =
                makeDummyTxn(
                    DUMMY_TXN_TYPE, dummyTxnId2, dummyTxnId1, dummyArtifactId);
            Future<TransactionStatus> stat2 = conn.submit(dummyTxn2);
            Certificate dummyTxn3 =
                makeDummyTxn(
                    DUMMY_TXN_TYPE, dummyTxnId3, dummyTxnId2, dummyArtifactId);
            Future<TransactionStatus> stat3 = conn.submit(dummyTxn3);

            /* commit transactions. */
            conn.commitTransactions();

            /* the status futures are now complete. */
            assertThat(stat1.isDone(), is(true));
            assertThat(stat2.isDone(), is(true));
            assertThat(stat3.isDone(), is(true));
            /* the transactions succeeded. */
            assertThat(stat1.get(), is(TransactionStatus.SUCCEEDED));
            assertThat(stat2.get(), is(TransactionStatus.SUCCEEDED));
            assertThat(stat3.get(), is(TransactionStatus.SUCCEEDED));

            /* we should be able to query the next and previous IDs for these
             * IDs. */
            assertThat(
                conn.getNextTransactionIdForTransactionById(dummyTxnId1)
                    .isPresent(),
                is(true));
            assertThat(
                conn.getNextTransactionIdForTransactionById(dummyTxnId1).get(),
                is(dummyTxnId2));
            assertThat(
                conn.getNextTransactionIdForTransactionById(dummyTxnId2)
                    .isPresent(),
                is(true));
            assertThat(
                conn.getNextTransactionIdForTransactionById(dummyTxnId2).get(),
                is(dummyTxnId3));
            assertThat(
                conn.getNextTransactionIdForTransactionById(dummyTxnId3)
                    .isPresent(),
                is(false));
            assertThat(
                conn.getPreviousTransactionIdForTransactionById(dummyTxnId1)
                    .isPresent(),
                is(false));
            assertThat(
                conn.getPreviousTransactionIdForTransactionById(dummyTxnId2)
                    .isPresent(),
                is(true));
            assertThat(
                conn.getPreviousTransactionIdForTransactionById(dummyTxnId2)
                    .get(),
                is(dummyTxnId1));
            assertThat(
                conn.getPreviousTransactionIdForTransactionById(dummyTxnId3)
                    .isPresent(),
                is(true));
            assertThat(
                conn.getPreviousTransactionIdForTransactionById(dummyTxnId3)
                    .get(),
                is(dummyTxnId2));

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
    private Certificate makeDummyTxn(
            UUID txnType, UUID txnId, UUID prevTxnId, UUID artifactId) {
        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(
                CertificateType.TRANSACTION);
        builder.addUUID(Field.TRANSACTION_TYPE, txnType);
        builder.addUUID(Field.ARTIFACT_ID, artifactId);
        builder.addUUID(Field.CERTIFICATE_ID, txnId);
        builder.addUUID(Field.PREVIOUS_CERTIFICATE_ID, prevTxnId);
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
