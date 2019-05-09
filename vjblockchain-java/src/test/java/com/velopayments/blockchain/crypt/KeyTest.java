package com.velopayments.blockchain.crypt;

import org.junit.Test;

import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

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

    /**
     * Test that the native function produces a key that  matches the Java impl
     * using HMAC-SHA-512
     */
    @Test
    public void sha512EquivalenceTest() throws Exception {

        String password = "password";
        String salt = "salt";

        int iterations = 10000;
        int keyLength = 64;

        PBEKeySpec spec = new PBEKeySpec(
                password.toCharArray(), salt.getBytes(), iterations,
                keyLength * 8);

        SecretKeyFactory skf =
                SecretKeyFactory.getInstance("PBKDF2WithHmacSHA512");
        byte[] javaKey = skf.generateSecret(spec).getEncoded();
        assertThat(javaKey.length, is(keyLength));

        // now generate via native code
        byte[] nativeKey = Key.createFromPasswordAsBytes(
                salt.getBytes(), iterations, password, true);
        assertThat(nativeKey, is(notNullValue()));

        // the size should be 512 bits (64 bytes).
        assertThat(nativeKey.length, is(keyLength));

        // the byte arrays should be equal
        assertThat(nativeKey, is(equalTo(javaKey)));
    }

    @Test
    public void sha512_256Test() throws Exception {

        String password = "password";
        String salt = "salt";

        int iterations = 10000;
        int keyLength = 32;

        byte[] nativeKey = Key.createFromPasswordAsBytes(
                salt.getBytes(), iterations, password, false);
        assertThat(nativeKey, is(notNullValue()));

        // the size should be 256 bits (32 bytes).
        assertThat(nativeKey.length, is(keyLength));
    }

    static final String HEXES = "0123456789ABCDEF";
    public static String getHex( byte [] raw ) {
        if ( raw == null ) {
            return null;
        }
        final StringBuilder hex = new StringBuilder( 2 * raw.length );
        for ( final byte b : raw ) {
            hex.append(HEXES.charAt((b & 0xF0) >> 4))
                    .append(HEXES.charAt((b & 0x0F)));
        }
        return hex.toString();
    }
}
