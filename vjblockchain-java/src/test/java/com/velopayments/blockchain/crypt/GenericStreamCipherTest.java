package com.velopayments.blockchain.crypt;

import com.velopayments.blockchain.util.ByteUtil;
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
        byte privateKey[] = keyPair.getPrivateKey().getRawBytes();

        byte iv[] = ByteUtil.longToBytes((long)(Math.random()*Long.MAX_VALUE),false);

        // when the message is encrypted
        byte encrypted[] = GenericStreamCipher.encrypt(privateKey, iv, input);

        // the encrypted message should be non-null and not the same as the input
        assertThat(encrypted, notNullValue());
        assertThat(encrypted, not(input));

        // when the message is decrypted
        byte decrypted[] = GenericStreamCipher.decrypt(privateKey, encrypted);

        // it should match the original message
        assertThat(decrypted, is(input));
    }
}
