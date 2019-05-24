package com.velopayments.blockchain.crypt;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.security.SecureRandom;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class HMACTest {

    SecureRandom secureRandom;
    HMAC hmac;

    @Before
    public void setup() {
        byte[] key = new byte[32];
        secureRandom = new SecureRandom();
        secureRandom.nextBytes(key);
        hmac = new HMAC(key);
    }

    @Test
    public void createHMACLong() {

        // generate a random message
        byte[] message1 = new byte[secureRandom.nextInt(100) + 1];
        secureRandom.nextBytes(message1);

        // HMAC the message
        byte[] digest1 = hmac.createHMACLong(message1);

        // the digest value should be 64 bytes long
        assertThat(digest1.length, is(64));

        // the digest value should be reproducible
        assertThat(hmac.createHMACLong(message1),is(digest1));
    }

    @Test
    public void createHMACShort() {

        // generate a random message
        byte[] message1 = new byte[secureRandom.nextInt(100) + 1];
        secureRandom.nextBytes(message1);

        // HMAC the message
        byte[] digest1 = hmac.createHMACShort(message1);

        // the digest value should be 32 bytes long
        assertThat(digest1.length, is(32));

        // the digest value should be reproducible
        assertThat(hmac.createHMACShort(message1),is(digest1));
    }

    @Test
    public void createHMACShort_multipleMessages() {

        // generate three random messages
        byte[] message1 = new byte[secureRandom.nextInt(100) + 1];
        secureRandom.nextBytes(message1);

        byte[] message2 = new byte[secureRandom.nextInt(100) + 1];
        secureRandom.nextBytes(message2);

        byte[] message3 = new byte[secureRandom.nextInt(100) + 1];
        secureRandom.nextBytes(message3);


        // add the first message to a multi-dimensional array, and compare
        // the HMAC output to the output of the single dimensional function.

        byte[][] messages = new byte[1][];
        messages[0] = message1;
        byte[] digest_multi = hmac.createHMACShort(messages);

        assertThat(digest_multi, is(hmac.createHMACShort(message1)));

        // add all three messages to multi-dimensional array, and ensure
        // the output has changed

        messages = new byte[3][0];
        messages[0] = message1;
        messages[1] = message2;
        messages[2] = message3;
        byte[] digest_multi2 = hmac.createHMACShort(messages);

        assertThat(digest_multi2, not(digest_multi));

        // finally ensure the output is repeatable
        assertThat(hmac.createHMACShort(messages), is(digest_multi2));
    }

    @Test
    public void multiDimensionalArrayWithNullElements() {

        try {
            hmac.createHMACShort(new byte[1][]);
            Assert.fail();
        } catch (NullPointerException e) {
            // good
        }
    }
}
