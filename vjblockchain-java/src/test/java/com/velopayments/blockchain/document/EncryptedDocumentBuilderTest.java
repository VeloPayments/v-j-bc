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

    /**
     * Test document encryption when the document size < buffer size
     *
     * @throws Exception
     */
    @Test
    public void singlePass() throws Exception {

        testRoundTrip("a");
        testRoundTrip("abc");
    }

    /**
     * Test document encryption when the document size > buffer size
     *
     * @throws Exception
     */
    @Test
    public void multiplePasses() throws Exception {

        String plainTextDoc = IOUtils.toString(
                this.getClass().getResourceAsStream("/test_message.txt"),"UTF-8");

        assertThat(plainTextDoc.startsWith("Lorem ipsum"), is(true));
        assertThat(plainTextDoc.length() > EncryptedDocumentBuilder.BUFFER_SIZE, is(true));

        testRoundTrip(plainTextDoc);
    }

    private void testRoundTrip(String plainTextDoc) throws Exception {

        EncryptionKeyPair creatorKey = EncryptionKeyPair.generate();
        EncryptionKeyPair subscriberKey = EncryptionKeyPair.generate();

        EncryptedDocumentBuilder builder = EncryptedDocumentBuilder.createDocumentBuilder(creatorKey)
                .withDocument(new ByteArrayInputStream(plainTextDoc.getBytes()));

        InputStream encryptedDoc = builder.emit();
        byte[] sharedSecret = builder.createEncryptedSharedSecret(subscriberKey.getPublicKey());

        // create a reader
        EncryptedDocumentReader reader = new EncryptedDocumentReader(subscriberKey.getPrivateKey(),
                creatorKey.getPublicKey(), sharedSecret, encryptedDoc);

        // verify the decrypted document matches what we started with
        assertThat(IOUtils.toString(reader.getEncrypted(), StandardCharsets.UTF_8), is(plainTextDoc));

    }

}
