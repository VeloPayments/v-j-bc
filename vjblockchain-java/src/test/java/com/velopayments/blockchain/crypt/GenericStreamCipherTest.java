package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.util.ByteUtil;
import org.junit.Ignore;
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

    @Ignore // TODO: need to do bounds checking to ensure key doesn't exceed
            // size specified in stream_cipher_options.key_size
    @Test
    public void decryptionKeyTooLong() {

        // given a simple message to encrypt, a key, and an IV
        byte input[] = { 1, 2, 3, 4, 5 };

        EncryptionKeyPair keyPair = EncryptionKeyPair.generate();
        byte shortKey[] = keyPair.getPrivateKey().getRawBytes();


        byte iv[] = ByteUtil.longToBytes((long)(Math.random()*Long.MAX_VALUE),false);

        // when the message is encrypted
        byte encrypted[] = GenericStreamCipher.encrypt(shortKey, iv, input);

        // the encrypted message should be non-null and not the same as the input
        assertThat(encrypted, notNullValue());
        assertThat(encrypted, not(input));

        // when the message is decrypted using a key that contains the encryption
        // key in the low order bytes, but extra bytes
        byte[] longKey = new byte[64];
        System.arraycopy(shortKey, 0, longKey, 0, 32);
        System.arraycopy(EncryptionKeyPair.generate().getPrivateKey().getRawBytes(),
                0, longKey, 32, 32);

        byte decrypted[] = GenericStreamCipher.decrypt(longKey, encrypted);

        // the decrypted text should NOT match the original message(?)
        assertThat(decrypted, not(input));
    }

}
