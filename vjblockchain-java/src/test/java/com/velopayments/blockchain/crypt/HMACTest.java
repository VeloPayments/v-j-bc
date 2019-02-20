package com.velopayments.blockchain.crypt;

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
}
