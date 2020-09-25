package com.velopayments.blockchain.example.consumer;

import com.velopayments.blockchain.cert.*;
import com.velopayments.blockchain.client.*;
import com.velopayments.blockchain.crypt.*;
import java.io.*;
import java.nio.file.*;
import java.util.concurrent.*;
import java.util.*;

public class Consumer implements Runnable {

    public void run() {
        /*AgentConnection conn = null;

        try {
            createDirectoryIfMissing();

            conn =
                new AgentConnection(
                    blockDBDir.toFile().getPath(),
                    consumerEntityId, consumerPrivateKey);

            readCerts(conn);
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
        }*/
    }

    /**
     * Main entry point.
     */
    public static void main(String[] args) {
        new Consumer().run();
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
     * Read certificates from blockchain.
     */
    private void readCerts(VelochainConnection conn)
    throws ExecutionException, IOException, InterruptedException {
        UUID lastBlockId = ROOT_UUID;

        for (;;) {
            Optional<UUID> nextBlockId = conn.getNextBlockId(lastBlockId);
            if (nextBlockId.isPresent()) {
                readBlock(conn, nextBlockId.orElse(ROOT_UUID));
                lastBlockId = nextBlockId.orElse(ROOT_UUID);
            } else {
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                }
            }
        }
    }

    /**
     * Read the provided block.
     */
    private void
    readBlock(VelochainConnection conn, UUID blockId)
    throws ExecutionException, IOException, InterruptedException {
        Certificate cert =
            conn.getBlockById(blockId).orElseThrow(
                () -> new IOException("Could not read block " + blockId));
        byte[] block = cert.toByteArray();

        CertificateReader reader =
            new CertificateReader(
                new CertificateParser(
                    Certificate.fromByteArray(block)));

        for (int i = 0; i < reader.count(Field.WRAPPED_TRANSACTION_TUPLE);
             ++i) {

            try {
                readTransaction(
                    reader.get(
                        Field.WRAPPED_TRANSACTION_TUPLE, i).asByteArray());
            } catch (MissingFieldException e) {
                System.out.println("Missing transaction " + Integer.valueOf(i));
            }
        }
    }

    /**
     * Read the provided transaction.
     */
    private void
    readTransaction(byte[] transaction) {
        try {
            CertificateReader reader =
                new CertificateReader(
                    new CertificateParser(
                        Certificate.fromByteArray(transaction)));

            UUID transactionID = reader.getFirst(Field.CERTIFICATE_ID).asUUID();
            String message = reader.getFirst(0x0401).asString();

            System.out.println(transactionID + ": " + message);

        } catch (Throwable e) {
            System.out.println("Could not parse transaction.");
        }
    }

    private static final Path blockDBDir = Paths.get("blockdb");
    /* TODO - pull entity, encryption keypair, and signing keypair from private
     * cert. */
    private static final UUID consumerEntityId =
        UUID.fromString("10150a08-94f9-4351-a32a-8075aa65ad3b");
    private static final UUID ROOT_UUID =
        CertificateType.ROOT_BLOCK;
    private static final EncryptionPrivateKey consumerPrivateKey =
        new EncryptionPrivateKey(new byte[] { 0,  1,  2,  3,  4,  5,  6,  7,
                                              8,  9, 10, 11, 12, 13, 14, 15,
                                             16, 17, 18, 19, 20, 21, 22, 23,
                                             24, 25, 26, 27, 28, 29, 30, 31 });
}
