package com.velopayments.blockchain.crypt;

import org.junit.Test;

import java.nio.charset.StandardCharsets;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.notNullValue;

public class SigningKeyPairTest {

    /**
     * Test that we can generate a signing keypair.
     */
    @Test
    public void generateSigningKeyPairTest() throws Exception {
        SigningKeyPair key = SigningKeyPair.generate();

        assertThat(key.getPublicKey().getRawBytes(), is(notNullValue()));
        assertThat(key.getPrivateKey().getRawBytes(), is(notNullValue()));
    }

    /**
     * Test that we can sign a message with the private key and verify the
     * message with the public key.
     */
    @Test
    public void signVerifyMessage() throws Exception {
        SigningKeyPair key = SigningKeyPair.generate();
        String testMsg = "This is a test.";

        /* create the message to sign. */
        Message m = new Message(testMsg.getBytes(StandardCharsets.UTF_8));

        /* sign message. */
        Signature s = key.getPrivateKey().sign(m);

        /* verify that the signed message matches the signature. */
        assertThat(key.getPublicKey().verify(s, m), is(true));
    }

    /**
     * Test that we passing a null Message causes sign to throw an NPE.
     */
    @Test(expected = NullPointerException.class)
    public void signMessageNPE() throws Exception {
        SigningKeyPair key = SigningKeyPair.generate();
        String testMsg = "This is a test.";

        /* sign null throws exception. */
        Signature s = key.getPrivateKey().sign(null);
    }

    /**
     * Test that we passing a null to verify throws an NPE.
     */
    @Test(expected = NullPointerException.class)
    public void verifyMessageNPE1() throws Exception {
        SigningKeyPair key = SigningKeyPair.generate();
        String testMsg = "This is a test.";

        /* verify null, null throws exception. */
        key.getPublicKey().verify(null, null);
    }

    /**
     * Test that a bad signature fails verification.
     */
    @Test
    public void signVerifyMessageFails() throws Exception {
        SigningKeyPair key = SigningKeyPair.generate();
        SigningKeyPair fake = SigningKeyPair.generate();
        String testMsg = "This is a test.";

        /* create the message to sign. */
        Message m = new Message(testMsg.getBytes(StandardCharsets.UTF_8));

        /* sign message. */
        Signature fakeSignature = fake.getPrivateKey().sign(m);

        /* verify that the signed message matches the signature. */
        assertThat(key.getPublicKey().verify(fakeSignature, m), is(false));
    }
}
