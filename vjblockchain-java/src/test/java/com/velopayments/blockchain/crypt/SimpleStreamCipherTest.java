package com.velopayments.blockchain.crypt;

import java.util.Arrays;
import org.junit.Test;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.notNullValue;

public class SimpleStreamCipherTest {

    /**
     * Test that we can encrypt a value using a random StreamCipher and decrypt
     * it.
     */
    @Test
    public void createRandomTest() throws Exception {

        /* input message. */
        byte INPUT[] = { 1, 2, 3, 4, 5 };

        /* create a cipher with a random key. */
        SimpleStreamCipher cipher = SimpleStreamCipher.createRandom();        

        /* the key should not be null. */
        assertThat(cipher.getKey(), is(notNullValue()));

        /* get the cipher text of the input. */
        byte cipherText[] = cipher.encrypt(INPUT);

        /* get the plaintext of the ciphertext. */
        byte plainText[] = cipher.decrypt(cipherText);

        /* the plaintext and INPUT should match. */
        assertThat(Arrays.equals(INPUT, plainText), is(true));
    }
}
