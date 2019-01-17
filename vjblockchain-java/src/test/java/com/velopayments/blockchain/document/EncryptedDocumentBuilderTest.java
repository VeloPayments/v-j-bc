package com.velopayments.blockchain.document;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import org.apache.commons.io.IOUtils;
import org.junit.Test;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

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

        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        EncryptedDocumentBuilder builder = EncryptedDocumentBuilder.createDocumentBuilder(creatorKey)
                .withSource(new ByteArrayInputStream(plainTextDoc.getBytes()))
                .withDestination(bos);

        byte[] sharedSecret = builder.createEncryptedSharedSecret(subscriberKey.getPublicKey());
        builder.emit();

        // create a reader
        ByteArrayOutputStream decrypted = new ByteArrayOutputStream();
        new EncryptedDocumentReader(subscriberKey.getPrivateKey(),
                creatorKey.getPublicKey(), sharedSecret)
                .withSource(new ByteArrayInputStream(bos.toByteArray()))
                .withDestination(decrypted)
                .emit();

        // verify the decrypted document matches what we started with
        assertThat(new String(decrypted.toByteArray()), is(plainTextDoc));
    }

}
