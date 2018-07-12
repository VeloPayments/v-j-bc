package com.velopayments.blockchain.cert;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import java.util.UUID;
import org.junit.Test;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.notNullValue;

public class EncryptedCertificateBuilderTest {

    /**
     * Test that we can use the EncryptedCertificateBuilder to build encrypted
     * data.
     */
    @Test
    public void encryptedBuilderTest() throws Exception {
        UUID dummyUUID = UUID.randomUUID();
        EncryptionKeyPair creatorKey = EncryptionKeyPair.generate();
        EncryptionKeyPair subscriberKey = EncryptionKeyPair.generate();

        EncryptedCertificateBuilder builder =
            EncryptedCertificateBuilder.createCertificateBuilder(
                creatorKey, CertificateType.PRIVATE_ENTITY);

        builder.addEncryptedSharedSecret(
                    dummyUUID, subscriberKey.getPublicKey());

        builder.addEncryptedInt(7, 7);
        builder.addEncryptedString(8, "Foo Bar");
        builder.addEncryptedInt(9, 22);

        Certificate cert = builder.emit();

        EncryptedCertificateReader reader =
            new EncryptedCertificateReader(
                    dummyUUID,
                    subscriberKey.getPrivateKey(),
                    creatorKey.getPublicKey(),
                    new CertificateParser(cert));

        reader.loadSecretKey();

        assertThat(reader.getFirstEncrypted(7).asInt(), is(7));
        assertThat(reader.getFirstEncrypted(8).asString(), is("Foo Bar"));
        assertThat(reader.getFirstEncrypted(9).asInt(), is(22));
    }
}
