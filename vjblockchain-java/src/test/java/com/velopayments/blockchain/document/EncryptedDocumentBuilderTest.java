package com.velopayments.blockchain.document;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import org.apache.commons.io.IOUtils;
import org.junit.Test;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;

public class EncryptedDocumentBuilderTest {

    @Test
    public void encryptedBuilderTest() throws Exception {

        EncryptionKeyPair creatorKey = EncryptionKeyPair.generate();
        EncryptionKeyPair subscriberKey = EncryptionKeyPair.generate();

        String plainTextDoc = "sample text";

        EncryptedDocumentBuilder builder = EncryptedDocumentBuilder.createDocumentBuilder(creatorKey)
                .withDocument(new ByteArrayInputStream(plainTextDoc.getBytes()));

        InputStream encryptedDoc = builder.emit();
        byte[] sharedSecret = builder.createEncryptedSharedSecret(subscriberKey.getPublicKey());

        // create a reader
        EncryptedDocumentReader reader = new EncryptedDocumentReader(subscriberKey.getPrivateKey(),
                creatorKey.getPublicKey(), sharedSecret, encryptedDoc);

        assertThat(IOUtils.toString(reader.getEncrypted(), StandardCharsets.UTF_8), is(plainTextDoc));

    }
}
