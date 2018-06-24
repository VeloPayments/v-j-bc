package com.velopayments.blockchain.cert;

import org.junit.Test;

import java.nio.ByteBuffer;
import java.util.UUID;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;

public class CertificateFieldReaderTest {

    /**
     * Test that we can read a byte from an array containing a byte.
     */
    @Test
    public void asByteTest() throws Exception {
        byte[] input = {88};

        assertThat(new CertificateFieldReader(input).asByte(), is(input[0]));
    }

    /**
     * Reading a byte from a buffer that is too small will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asByteSmallBufferException() throws Exception {
        byte[] input = {};

        new CertificateFieldReader(input).asByte();
    }

    /**
     * Reading a byte from a buffer that is too large will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asByteLargeBufferException() throws Exception {
        byte[] input = {1, 2};

        new CertificateFieldReader(input).asByte();
    }

    /**
     * Test that we can read a short from an appropriately sized array.
     */
    @Test
    public void asShortTest() throws Exception {
        final short TEST_VALUE = 0x1234;
        ByteBuffer buf = ByteBuffer.allocate(2);
        buf.putShort(TEST_VALUE);

        assertThat(
            new CertificateFieldReader(buf.array()).asShort(), is(TEST_VALUE));
    }

    /**
     * Reading a short from a buffer that is too small will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asShortSmallBufferException() throws Exception {
        byte[] input = {1};

        new CertificateFieldReader(input).asShort();
    }

    /**
     * Reading a short from a buffer that is too large will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asShortLargeBufferException() throws Exception {
        byte[] input = {1, 2, 3};

        new CertificateFieldReader(input).asShort();
    }

    /**
     * Test that we can read an int from an appropriately sized array.
     */
    @Test
    public void asIntTest() throws Exception {
        final int TEST_VALUE = -15;
        ByteBuffer buf = ByteBuffer.allocate(4);
        buf.putInt(TEST_VALUE);

        assertThat(
            new CertificateFieldReader(buf.array()).asInt(), is(TEST_VALUE));
    }

    /**
     * Reading an int from a buffer that is too small will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asIntSmallBufferException() throws Exception {
        byte[] input = {1, 2, 3};

        new CertificateFieldReader(input).asInt();
    }

    /**
     * Reading an int from a buffer that is too large will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asIntLargeBufferException() throws Exception {
        byte[] input = {1, 2, 3, 4, 5};

        new CertificateFieldReader(input).asInt();
    }

    /**
     * Test that we can read a long from an appropriately sized array.
     */
    @Test
    public void asLongTest() throws Exception {
        final long TEST_VALUE = 8773;
        ByteBuffer buf = ByteBuffer.allocate(8);
        buf.putLong(TEST_VALUE);

        assertThat(
            new CertificateFieldReader(buf.array()).asLong(), is(TEST_VALUE));
    }

    /**
     * Reading a long from a buffer that is too small will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asLongSmallBufferException() throws Exception {
        byte[] input = {1, 2, 3, 4, 5, 6, 7};

        new CertificateFieldReader(input).asLong();
    }

    /**
     * Reading a long from a buffer that is too large will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asLongLargeBufferException() throws Exception {
        byte[] input = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        new CertificateFieldReader(input).asLong();
    }

    /**
     * Test that we can read a UUID from an appropriately sized array.
     */
    @Test
    public void asUUIDTest() throws Exception {
        final UUID TEST_VALUE =
            UUID.fromString("cd319764-5586-43d9-a17a-76b12e108fa7");
        ByteBuffer buf = ByteBuffer.allocate(16);
        buf.putLong(TEST_VALUE.getMostSignificantBits());
        buf.putLong(TEST_VALUE.getLeastSignificantBits());

        assertThat(
            new CertificateFieldReader(buf.array()).asUUID(), is(TEST_VALUE));
    }

    /**
     * Reading a UUID from a buffer that is too small will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asUUIDSmallBufferException() throws Exception {
        byte[] input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

        new CertificateFieldReader(input).asUUID();
    }

    /**
     * Reading a UUID from a buffer that is too large will raise an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asUUIDLargeBufferException() throws Exception {
        byte[] input = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

        new CertificateFieldReader(input).asUUID();
    }

    /**
     * Test that we can read a String from an array.
     */
    @Test
    public void asStringTest() throws Exception {
        byte[] TEST_VALUE = {65, 66, 67, 68, 69, 70};

        assertThat(
            new CertificateFieldReader(TEST_VALUE).asString(), is("ABCDEF"));
    }

    /**
     * Reading an empty array returns an empty String.
     */
    @Test
    public void asStringEmptyBufferTest() throws Exception {
        byte[] input = {};

        assertThat(
            new CertificateFieldReader(input).asString(), is(""));
    }

    /**
     * Reading a String with an invalid mapping throws an exception.
     */
    @Test(expected = FieldConversionException.class)
    public void asStringInvalidSequenceExceptionTest() throws Exception {
        byte[] input = {(byte) 0xc3, (byte) 0x28};

        new CertificateFieldReader(input).asString();
    }

    /**
     * Test that asByteArray just returns the byte array.
     */
    @Test
    public void asByteArrayTest() throws Exception {
        byte[] TEST_VALUE = {2, 3, 4};

        assertThat(
            new CertificateFieldReader(TEST_VALUE).asByteArray(), is(TEST_VALUE));
    }
}
