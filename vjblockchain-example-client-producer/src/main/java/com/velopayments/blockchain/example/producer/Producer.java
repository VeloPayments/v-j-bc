package com.velopayments.blockchain.example.producer;

import com.velopayments.blockchain.cert.*;
import com.velopayments.blockchain.client.AgentConnection;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import java.io.*;
import java.nio.file.*;
import java.util.UUID;

public class Producer implements Runnable {

    public void run() {
        AgentConnection conn = null;

        try {
            createDirectoryIfMissing();

            conn =
                new AgentConnection(
                    blockDBDir.toFile().getPath(),
                    producerEntityId, producerPrivateKey);

            writeCerts(conn);

        } catch (IOException e) {
            System.out.println("Got IOException.");
        } finally {
            try {
                if (conn != null) {
                    conn.close();
                }
            } catch (IOException e) {
                System.out.println("Got IOException closing Agent Connection");
            }
        }
    }

    /**
     * Main entry point.
     */
    public static void main(String[] args) {
        new Producer().run();
    }

    /**
     * Create the blockdb directory.
     */
    private void createDirectoryIfMissing() throws IOException {
        if (Files.notExists(blockDBDir)) {
            Files.createDirectories(blockDBDir);
        }
    }

    /**
     * Write certificates to blockchain.
     */
    private void writeCerts(AgentConnection conn) throws IOException {
        for (int i = 0; i < 1000; ++i) {
            CertificateBuilder builder =
                CertificateBuilder.createCertificateBuilder(
                    EXAMPLE_TRANSACTION_TYPE);

            builder.addUUID(
                Field.CERTIFICATE_ID, UUID.randomUUID());
            builder.addString(
                0x0401, "Example transaction " + Integer.valueOf(i + 1));

            System.out.println(
                "Submitting transaction " + Integer.valueOf(i + 1));

            conn.submit(Certificate.fromByteArray(builder.emit()));

            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
            }
        }

        conn.commitTransactions();
    }

    private static final UUID EXAMPLE_TRANSACTION_TYPE =
        UUID.fromString("c1625721-4930-4b7a-b321-8386285330f4");
    private static final Path blockDBDir = Paths.get("blockdb");
    /* TODO - pull entity, encryption keypair, and signing keypair from private
     * cert. */
    private static final UUID producerEntityId =
        UUID.fromString("10150a08-94f9-4351-a32a-8075aa65ad3b");
    private static final EncryptionPrivateKey producerPrivateKey =
        new EncryptionPrivateKey(new byte[] { 0,  1,  2,  3,  4,  5,  6,  7,
                                              8,  9, 10, 11, 12, 13, 14, 15,
                                             16, 17, 18, 19, 20, 21, 22, 23,
                                             24, 25, 26, 27, 28, 29, 30, 31 });
}
