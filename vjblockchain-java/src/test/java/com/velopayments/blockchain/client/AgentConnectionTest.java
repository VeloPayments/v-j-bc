package com.velopayments.blockchain.client;

import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import java.nio.ByteBuffer;
import java.io.File;
import java.util.Optional;
import java.util.UUID;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.notNullValue;
import static org.mockito.Mockito.*;

public class AgentConnectionTest {

    private static short PRIVATE_KEY_SHORTS[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    private static EncryptionPrivateKey PRIVATE_KEY;

    UUID entityId = UUID.fromString("89921ea8-5cb8-4121-9f4f-4ab94c2a7bfd");
    UUID zeroUUID = UUID.fromString("00000000-0000-0000-0000-000000000000");

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
            conn.close();
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
            conn.close();
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
            conn.close();
        }
    }

    static {
        PRIVATE_KEY =
            new EncryptionPrivateKey(convShorts(PRIVATE_KEY_SHORTS).array());
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
