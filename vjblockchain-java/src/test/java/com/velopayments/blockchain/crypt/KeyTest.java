package com.velopayments.blockchain.crypt;

import org.junit.Test;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.notNullValue;

public class KeyTest {

    /**
     * Test that we can generate a key.
     */
    @Test
    public void createRandomTest() throws Exception {
        /* we can't really test this beyond ensuring that the generated key is
         * the right size.  We can't verify that it is random "enough". */
        Key key = Key.createRandom();

        /* the raw bytes should not be null. */
        assertThat(key.getRawBytes(), is(notNullValue()));
        /* the size should be 256 bits (32 bytes). */
        assertThat(key.getRawBytes().length, is(32));
    }

    /**
     * Test that using an invalid key size when creating a Key results in an
     * InvalidKeySizeException.
     */
    @Test(expected = InvalidKeySizeException.class)
    public void invalidKeySizeTest() throws Exception {
        byte[] INVALID_SIZED_KEY = {1, 2, 3, 4};

        /* this throws an exception. */
        Key key = new Key(INVALID_SIZED_KEY);
    }

    /**
     * Test that creating a dummy key of the right size works.
     */
    @Test
    public void correctKeySizeTest() throws Exception {
        byte[] CORRECTLY_SIZED_KEY = {0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0 };

        /* This succeeds. */
        Key key = new Key(CORRECTLY_SIZED_KEY);
    }
}
