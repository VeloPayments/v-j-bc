package com.velopayments.blockchain.crypt;

import org.junit.Test;

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
}
