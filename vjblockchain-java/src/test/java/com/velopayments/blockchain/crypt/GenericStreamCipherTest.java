package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.util.ByteUtil;
import org.junit.Assert;
import org.junit.Test;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.not;
import static org.hamcrest.Matchers.notNullValue;

public class GenericStreamCipherTest {

    @Test
    public void simpleMessage() {

        // given a simple message to encrypt, a key, and an IV
        byte input[] = { 1, 2, 3, 4, 5 };

        EncryptionKeyPair keyPair = EncryptionKeyPair.generate();
        byte secretKey[] = keyPair.getPrivateKey().getRawBytes();

        byte iv[] = ByteUtil.longToBytes((long)(Math.random()*Long.MAX_VALUE),false);

        // when the message is encrypted
        byte encrypted[] = GenericStreamCipher.encrypt(secretKey, iv, input);

        // the encrypted message should be non-null and not the same as the input
        assertThat(encrypted, notNullValue());
        assertThat(encrypted, not(input));

        // when the message is decrypted
        byte decrypted[] = GenericStreamCipher.decrypt(secretKey, encrypted);

        // it should match the original message
        assertThat(decrypted, is(input));
    }

    @Test
    public void wrongSecretKey() {

        // given a simple message to encrypt, a key, and an IV
        byte input[] = { 1, 2, 3, 4, 5 };

        EncryptionKeyPair keyPair = EncryptionKeyPair.generate();
        byte secretKey[] = keyPair.getPrivateKey().getRawBytes();

        byte iv[] = ByteUtil.longToBytes((long)(Math.random()*Long.MAX_VALUE),false);

        // when the message is encrypted and then decrypted using a different key
        byte encrypted[] = GenericStreamCipher.encrypt(secretKey, iv, input);

        // when the message is decrypted using the wrong key
        byte decrypted[] = GenericStreamCipher.decrypt(
                EncryptionKeyPair.generate().getPrivateKey().getRawBytes(),
                encrypted);

        // it should not match the original message
        assertThat(decrypted, not(input));
    }

    @Test
    public void encryptionKeySize() {

        // given a simple message to encrypt, an IV, and a key
        byte input[] = { 1, 2, 3, 4, 5 };
        byte iv[] = ByteUtil.longToBytes((long)(Math.random()*Long.MAX_VALUE),false);
        EncryptionKeyPair keyPair = EncryptionKeyPair.generate();
        byte key[] = keyPair.getPrivateKey().getRawBytes();


        // when the message is encrypted using a key that is too long
        byte[] longKey = new byte[64];
        System.arraycopy(key, 0, longKey, 0, 32);
        System.arraycopy(EncryptionKeyPair.generate().getPrivateKey().getRawBytes(),
                0, longKey, 32, 32);

        try {
            GenericStreamCipher.encrypt(longKey, iv, input);
            Assert.fail();
        } catch (IllegalStateException e) {
            assertThat(e.getMessage(), is("Invalid key size."));
        }

        // when the message is encrypted using a key that is too short
        byte[] shortKey = new byte[16];
        System.arraycopy(key, 0, shortKey, 0, 16);

        try {
            GenericStreamCipher.encrypt(shortKey, iv, input);
            Assert.fail();
        } catch (IllegalStateException e) {
            assertThat(e.getMessage(), is("Invalid key size."));
        }

        // when the message is encrypted using the properly sized key
        // then no exception is thrown
        GenericStreamCipher.encrypt(key, iv, input);
    }


    @Test
    public void decryptionKeySize() {

        // given a simple message to decrypt, an IV, and a key
        byte input[] = { 1, 2, 3, 4, 5 };
        byte iv[] = ByteUtil.longToBytes((long)(Math.random()*Long.MAX_VALUE),false);
        EncryptionKeyPair keyPair = EncryptionKeyPair.generate();
        byte key[] = keyPair.getPrivateKey().getRawBytes();
        byte encrypted[] = GenericStreamCipher.encrypt(key, iv, input);

        // when the message is decrypted using a key that is too long
        byte[] longKey = new byte[64];
        System.arraycopy(key, 0, longKey, 0, 32);
        System.arraycopy(EncryptionKeyPair.generate().getPrivateKey().getRawBytes(),
                0, longKey, 32, 32);

        // then an exception should be thrown
        try {
            GenericStreamCipher.decrypt(longKey, encrypted);
            Assert.fail();
        } catch (IllegalStateException e) {
            assertThat(e.getMessage(), is("Invalid key size."));
        }

        // when the message is decrypted using a key that is too short
        byte[] shortKey = new byte[16];
        System.arraycopy(key, 0, shortKey, 0, 16);

        // then an exception should be thrown
        try {
            GenericStreamCipher.decrypt(shortKey, encrypted);
            Assert.fail();
        } catch (IllegalStateException e) {
            assertThat(e.getMessage(), is("Invalid key size."));
        }

        // when the message is decrypted using the properly sized key
        // then no exception is thrown
        GenericStreamCipher.decrypt(key, encrypted);
    }

}
