package com.velopayments.blockchain.agentd;

import org.junit.Test;

import java.io.ByteArrayInputStream;
import java.security.SecureRandom;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.notNullValue;
import static org.hamcrest.Matchers.not;

public class HMACTest {

    SecureRandom secureRandom = new SecureRandom();

    @Test
    public void digest() {


        byte[] key = new byte[32];
        secureRandom.nextBytes(key);

        HMAC hmac = new HMAC(key);

        byte[] message = "some message".getBytes();

        byte[] digest = hmac.digest(message);

        assertThat(digest, notNullValue());
        assertThat(digest.length, not(0));

        // TODO
    }
}
