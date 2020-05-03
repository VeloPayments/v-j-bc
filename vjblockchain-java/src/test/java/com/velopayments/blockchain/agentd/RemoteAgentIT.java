package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.cert.*;
import com.velopayments.blockchain.client.RemoteAgentConfiguration;
import com.velopayments.blockchain.client.RemoteAgentConnection;
import com.velopayments.blockchain.crypt.*;

import javax.net.SocketFactory;
import java.io.IOException;
import java.util.Optional;
import java.util.UUID;

/**
 * This is a temporary utility class while developing the remoting capabilities.
 * It will be replaced with an integration test.
 */
public class RemoteAgentIT {

    public static void main(String[] args) {
        System.out.println("testing agentd connection...");

        UUID agentId = UUID.fromString("cb6c02aa-605f-4f81-bb01-5bb6f5975746");
        RemoteAgentConfiguration config = new RemoteAgentConfiguration(
                "localhost",4931,agentId,null);

        UUID entityId = UUID.fromString("aca029b6-2602-4b20-a8a4-cd8a95985a9a");

        byte[] entityPrivateKeyBytes = {
                (byte)0x77, (byte)0x07, (byte)0x6d, (byte)0x0a, (byte)0x73, (byte)0x18, (byte)0xa5, (byte)0x7d,
                (byte)0x3c, (byte)0x16, (byte)0xc1, (byte)0x72, (byte)0x51, (byte)0xb2, (byte)0x66, (byte)0x45,
                (byte)0xdf, (byte)0x4c, (byte)0x2f, (byte)0x87, (byte)0xeb, (byte)0xc0, (byte)0x99, (byte)0x2a,
                (byte)0xb1, (byte)0x77, (byte)0xfb, (byte)0xa5, (byte)0x1d, (byte)0xb9, (byte)0x2c, (byte)0x2a };

        SigningKeyPair signPair = SigningKeyPair.generate();

        RemoteAgentConnection conn = new RemoteAgentConnection(
                config, SocketFactory.getDefault(),
                entityId, new EncryptionPrivateKey(entityPrivateKeyBytes));

        try {
            conn.connect();
            System.out.println("handshake success!");
            UUID firstId = conn.getLatestBlockId();
            System.out.println("Initial Block UUID: " + firstId);
            for (int i = 0; i < 2000; ++i) {
                System.out.println("Submitting txn: " + i);
                Certificate txn =
                    makeTransactionCertificate(
                        signPair, entityId, UUID.randomUUID(),
                        UUID.randomUUID());
                conn.submit(txn);
            }
            System.out.println("batch submit success!");

            /* make txn we can query later. */
            UUID certId = UUID.randomUUID();
            UUID artifactId = UUID.randomUUID();
            Certificate txn =
                makeTransactionCertificate(
                    signPair, entityId, artifactId, certId);
            conn.submit(txn);
            System.out.println("individual submit success!");

            System.out.println("Sleeping for 10 seconds...");
            Thread.sleep(10000);
            Optional<UUID> prevIdMaybe = Optional.of(conn.getLatestBlockId());

            while (prevIdMaybe.isPresent()) {
                UUID prevId = prevIdMaybe.get();
                System.out.println("New Block UUID: " + prevId);
                if (prevId.compareTo(firstId) != 0) {
                    Optional<Certificate> blockCert = conn.getBlockById(prevId);
                    if (blockCert.isPresent()) {
                        System.out.println("Read block " + prevId + " from agentd");
                        readBlockCert(blockCert.get());
                    } else {
                        System.out.println("Could not retrieve block " + prevId);
                    }
                }

                prevIdMaybe = conn.getPrevBlockId(prevId);
                if (!prevIdMaybe.isPresent()) {
                    System.out.println("No other block IDs found.");
                }
            }

            /* query our single transaction. */
            Optional<Certificate> singleCert = conn.getTransactionById(certId);
            if (singleCert.isPresent()) {
                System.out.println("Query for " + certId + " successful!");
            } else {
                System.out.println("*** Query for " + certId + " FAILED!");
            }

            /* there is no next transaction id. */
            Optional<UUID> nextId =
                conn.getNextTransactionIdForTransactionById(certId);
            if (nextId.isPresent()) {
                System.out.println("*** Txn Next Id FAIL.");
            } else {
                System.out.println("    Next is not found as expected.");
            }

            /* there is no prev transaction id. */
            Optional<UUID> prevId =
                conn.getPreviousTransactionIdForTransactionById(certId);
            if (prevId.isPresent()) {
                System.out.println("*** Txn Prev Id FAIL.");
            } else {
                System.out.println("    Prev is not found as expected.");
            }

            /* there is a block id. */
            Optional<UUID> blockId = conn.getTransactionBlockId(certId);
            if (blockId.isPresent()) {
                System.out.println("    Block ID found: " + blockId.get());
            } else {
                System.out.println("*** Txn Block Id FAIL.");
            }

            /* get the first txn id for the artifact id. */
            Optional<UUID> firstTxnId =
                conn.getFirstTransactionIdForArtifactById(artifactId);
            if (firstTxnId.isPresent()) {
                System.out.println(
                    "    First Txn ID found: " + firstTxnId.get());
            } else {
                System.out.println("*** First Txn Id FAIL.");
            }

            /* get the last txn id for the artifact id. */
            Optional<UUID> lastTxnId =
                conn.getLastTransactionIdForArtifactById(artifactId);
            if (lastTxnId.isPresent()) {
                System.out.println(
                    "    Last Txn ID found: " + lastTxnId.get());
            } else {
                System.out.println("*** Last Txn Id FAIL.");
            }

        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (Throwable e) {
            e.printStackTrace();
        } finally {
            try {
                conn.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        System.out.println("execution complete.");
    }

    private static void readBlockCert(Certificate cert) {

        CertificateReader reader =
            new CertificateReader(
                new CertificateParser(cert));

        System.out.println(
            "Found " +
            Integer.valueOf(reader.count(Field.WRAPPED_TRANSACTION_TUPLE)) +
            " transactions in cert.");

        for (int i = 0; i < reader.count(Field.WRAPPED_TRANSACTION_TUPLE); ++i)
        {
            try {
                readTransaction(
                    reader.get(
                        Field.WRAPPED_TRANSACTION_TUPLE, i).asByteArray());
            } catch (MissingFieldException e) {
                System.out.println("Missing transaction " + Integer.valueOf(i));
            }
        }
    }

    private static void readTransaction(byte[] txn) {
        try {
            CertificateReader reader =
                new CertificateReader(
                    new CertificateParser(
                        Certificate.fromByteArray(txn)));

            UUID transactionID = reader.getFirst(Field.CERTIFICATE_ID).asUUID();

            System.out.println("    Parsed transaction " + transactionID);
        } catch (Throwable e) {
            System.out.println("Could not parse transaction.");
        }
    }

    private static UUID DUMMY_TXN_TYPE =
        UUID.fromString("bc91987a-d2bd-46d7-bccb-a8d94ff49906");

    private static Certificate makeTransactionCertificate(
                                    SigningKeyPair signPair, UUID entityId,
                                    UUID artifactId,
                                    UUID certificateUuid) {

        UUID DUMMY_CERTIFICATE_ID = certificateUuid;
        UUID ZERO_UUID = new UUID(0, 0);

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(DUMMY_TXN_TYPE);
        builder.addUUID(Field.PREVIOUS_CERTIFICATE_ID, ZERO_UUID);
        builder.addUUID(Field.CERTIFICATE_ID, DUMMY_CERTIFICATE_ID);
        builder.addUUID(Field.ARTIFACT_ID, artifactId);
        builder.addInt(Field.PREVIOUS_ARTIFACT_STATE, -1);
        builder.addInt(Field.NEW_ARTIFACT_STATE, 0x00000000);
        builder.addString(0x405, "An Example String");
        return builder.sign(entityId, signPair.getPrivateKey());
    }
}
