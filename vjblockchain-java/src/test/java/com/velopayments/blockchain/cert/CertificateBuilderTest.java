package com.velopayments.blockchain.cert;

import com.velopayments.blockchain.crypt.SigningPrivateKey;

import org.junit.Test;

import java.nio.ByteBuffer;
import java.time.ZonedDateTime;
import java.time.ZoneId;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.notNullValue;
import static org.mockito.Mockito.*;

public class CertificateBuilderTest {

    private static short PRIVATE_KEY_SHORTS[] = {
        0x65, 0x93, 0x21, 0xd0, 0x35, 0xa9, 0xf8, 0xcf,
        0x35, 0x37, 0xd1, 0xd1, 0x82, 0xfd, 0xee, 0xf8,
        0x92, 0x8e, 0x0c, 0xfe, 0xb4, 0x56, 0x4b, 0x2d,
        0xb5, 0x11, 0x60, 0x6d, 0xc6, 0xf6, 0x13, 0xbd,
        0x47, 0x83, 0xe9, 0xf6, 0x78, 0xd1, 0x49, 0xac,
        0xd2, 0x09, 0x66, 0xb0, 0xab, 0x88, 0xf7, 0xd0,
        0x5d, 0x6d, 0x4f, 0x54, 0x0f, 0x1f, 0x23, 0x82,
        0x86, 0x00, 0x3a, 0xda, 0x0c, 0x27, 0xcc, 0x35
    };
    private static SigningPrivateKey PRIVATE_KEY;

    private static short PARENT_ENCRYPTION_KEY_SHORTS[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    private static short PARENT_SIGNING_KEY_SHORTS[] = {
        0x47, 0x83, 0xe9, 0xf6, 0x78, 0xd1, 0x49, 0xac,
        0xd2, 0x09, 0x66, 0xb0, 0xab, 0x88, 0xf7, 0xd0,
        0x5d, 0x6d, 0x4f, 0x54, 0x0f, 0x1f, 0x23, 0x82,
        0x86, 0x00, 0x3a, 0xda, 0x0c, 0x27, 0xcc, 0x35,
    };
    private static byte PARENT_ENCRYPTION_KEY[];
    private static byte PARENT_SIGNING_KEY[];

    private static UUID PARENT_UUID =
        UUID.fromString("00010203-0405-0607-0809-0a0b0c0d0e0f");

    //run once to convert private key and parent public certificate stub
    static {
        PRIVATE_KEY =
            new SigningPrivateKey(convShorts(PRIVATE_KEY_SHORTS).array());
        PARENT_ENCRYPTION_KEY = convShorts(PARENT_ENCRYPTION_KEY_SHORTS).array();
        PARENT_SIGNING_KEY = convShorts(PARENT_SIGNING_KEY_SHORTS).array();
    }

    /**
     * Helper method to build packed ByteBuffer from array of short hex values.
     */
    private static ByteBuffer convShorts(short in[]) {
        ByteBuffer retVal = ByteBuffer.allocate(in.length);
        for (short val : in) {
            retVal.put((byte) val);
        }

        return retVal;
    }

    /**
     * Test that an empty certificate includes the version, crypto suite, and
     * certificate type.
     */
    @Test
    public void emptyCertificateTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        Map<Integer, List<byte[]>> certMap = p.parse();
        CertificateReader r = new CertificateReader(p);

        assertThat(certMap, is(notNullValue()));
        assertThat(certMap.isEmpty(), is(false));

        //check field CERTIFICATE_VERSION
        assertThat(r.count(Field.CERTIFICATE_VERSION), is(1));
        assertThat(
            r.getFirst(Field.CERTIFICATE_VERSION).asInt(), is(0x00010000));

        //check field CERTIFICATE_CRYPTO_SUITE
        assertThat(r.count(Field.CERTIFICATE_CRYPTO_SUITE), is(1));
        assertThat(
            r.getFirst(Field.CERTIFICATE_CRYPTO_SUITE).asShort(),
            is((short) 0x0001));

        //check field CERTIFICATE_TYPE
        assertThat(r.count(Field.CERTIFICATE_TYPE), is(1));
        assertThat(
            r.getFirst(Field.CERTIFICATE_TYPE).asUUID(),
            is(CERT_TYPE));
    }

    /**
     * Test that an empty certificate fragment contains just the added field.
     */
    @Test
    public void certificateFragmentTest() throws Exception {
        final int TEST_FIELD = 0x4301;
        final byte TEST_VALUE = 0x12;

        CertificateBuilder builder =
            CertificateBuilder.createCertificateFragmentBuilder();
        builder.addByte(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        Map<Integer, List<byte[]>> certMap = p.parse();
        CertificateReader r = new CertificateReader(p);

        assertThat(certMap, is(notNullValue()));
        assertThat(certMap.isEmpty(), is(false));

        //check that field CERTIFICATE_VERSION was not emitted
        assertThat(r.count(Field.CERTIFICATE_VERSION), is(0));

        //check that field CERTIFICATE_CRYPTO_SUITE was not emitted
        assertThat(r.count(Field.CERTIFICATE_CRYPTO_SUITE), is(0));

        //check that field CERTIFICATE_TYPE was not emitted
        assertThat(r.count(Field.CERTIFICATE_TYPE), is(0));

        //check that our test field was emitted
        assertThat(r.getFirst(TEST_FIELD).asByte(), is(TEST_VALUE));
    }

    /**
     * Test that an empty certificate fragment throws an IllegalStateException
     * if an attempt is made to emit it.
     */
    @Test(expected = IllegalStateException.class)
    public void emptyCertificateFragmentFailsEmitTest() throws Exception {
        final int TEST_FIELD = 0x4301;
        final byte TEST_VALUE = 0x12;

        CertificateBuilder builder =
            CertificateBuilder.createCertificateFragmentBuilder();
        Certificate outCert = builder.emit();
    }

    /**
     * Test that we can add a byte field to a certificate and parse it.
     */
    @Test
    public void buildByteValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final int TEST_FIELD = 0x4301;
        final byte TEST_VALUE = 0x12;

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addByte(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asByte(), is(TEST_VALUE));
    }

    /**
     * Test that we can add a short field to a certificate and parse it.
     */
    @Test
    public void buildShortValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final int TEST_FIELD = 0x4302;
        final int TEST_VALUE = 0x8051;

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addShort(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asShort(),
            is((short) TEST_VALUE));
    }

    /**
     * Test that we can add an int field to a certificate and parse it.
     */
    @Test
    public void buildIntValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final int TEST_FIELD = 0x4303;
        final int TEST_VALUE = -6782;

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addInt(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        Map<Integer, List<byte[]>> certMap = p.parse();
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asInt(), is(TEST_VALUE));
    }

    /**
     * Test that we can add a long field to a certificate and parse it.
     */
    @Test
    public void buildLongValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final int TEST_FIELD = 0x4304;
        final long TEST_VALUE = 1745;

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addLong(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asLong(), is(TEST_VALUE));
    }

    /**
     * Test that we can add a UUID field to a certificate and parse it.
     */
    @Test
    public void buildUUIDValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final int TEST_FIELD = 0x4305;
        final UUID TEST_VALUE =
            UUID.fromString("b2dd6d91-a30e-425f-a06e-9f1b4bcc2d87");

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addUUID(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asUUID(), is(TEST_VALUE));
    }

    /**
     * Test that we can add a String field to a certificate and parse it.
     */
    @Test
    public void buildStringValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final int TEST_FIELD = 0x4306;
        final String TEST_VALUE = "This is a test string.";

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addString(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asString(),
            is(TEST_VALUE));
    }

    /**
     * Test that we can add a Date field to a certificate and parse it.
     */
    @Test
    public void buildDateValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final int TEST_FIELD = 0x4307;
        //we don't store milliseconds in the certificate
        final Date TEST_VALUE = new Date(
            new Date().toInstant().getEpochSecond() * 1000);

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addDate(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asDate(), is(TEST_VALUE));
    }

    /**
     * Test that we can add a byte array field to a certificate and parse it.
     */
    @Test
    public void buildByteArrayValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final int TEST_FIELD = 0x4308;
        //we don't store milliseconds in the certificate
        final byte[] TEST_VALUE = {1, 2, 3, 4, 5};

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addByteArray(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.emit();
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asByteArray(), is(TEST_VALUE));
    }

    /**
     * Test that we can add a byte field to a certificate, sign it, and parse
     * it.
     */
    @Test
    public void buildAndSignByteValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final UUID ARTIFACT_TYPE =
            UUID.fromString("897d4d04-d5d8-4bf4-b1d1-6884bb362a44");
        final int TEST_FIELD = 0x4301;
        final byte TEST_VALUE = 0x12;
        final long BLOCK_HEIGHT = 77;

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addByte(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.sign(PARENT_UUID, PRIVATE_KEY);
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);

        //create a mock certificate contract
        CertificateContract mockContract = mock(CertificateContract.class);
        when(
            mockContract.verify(any(CertificateParser.class))).thenReturn(true);

        //create a mock parser delegate
        CertificateParserDelegate mockDelegate =
            mock(CertificateParserDelegate.class);
        //resolve the parent's UUID in the certificate chain
        when(mockDelegate.resolveEntity(PARENT_UUID, BLOCK_HEIGHT)).thenReturn(
            new EntityReference(
                PARENT_ENCRYPTION_KEY, PARENT_SIGNING_KEY, ARTIFACT_TYPE));

        //attestation should be successful
        assertThat(p.attest(mockDelegate, BLOCK_HEIGHT, false), is(true));

        //read the certificate
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asByte(), is(TEST_VALUE));
    }

    /**
     * Test that we can add a Date field to a certificate, sign it, and parse
     * it.
     */
    @Test
    public void buildAndSignDateValueTest() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final UUID ARTIFACT_TYPE =
            UUID.fromString("897d4d04-d5d8-4bf4-b1d1-6884bb362a44");
        final int TEST_FIELD = 0x4307;
        final long BLOCK_HEIGHT = 66;
        final Date TEST_VALUE = new Date(
            new Date().toInstant().getEpochSecond() * 1000);

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addDate(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.sign(PARENT_UUID, PRIVATE_KEY);
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);

        //create a mock certificate contract
        CertificateContract mockContract = mock(CertificateContract.class);
        when(
            mockContract.verify(any(CertificateParser.class))).thenReturn(true);

        //create a mock parser delegate
        CertificateParserDelegate mockDelegate =
            mock(CertificateParserDelegate.class);
        //resolve the parent's UUID in the certificate chain
        when(mockDelegate.resolveEntity(PARENT_UUID, BLOCK_HEIGHT)).thenReturn(
            new EntityReference(
                PARENT_ENCRYPTION_KEY, PARENT_SIGNING_KEY, ARTIFACT_TYPE));

        //attestation should be successful
        assertThat(p.attest(mockDelegate, BLOCK_HEIGHT, false), is(true));

        //read the certificate
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD).asDate(), is(TEST_VALUE));
    }

    /**
     * We do not support ZonedDateTime for non-custom fields.
     */
    @Test(expected = IllegalArgumentException.class)
    public void buildZonedDateTimeFailsOnReservedDates() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final ZoneId TEST_ZONE = ZoneId.of("Europe/Paris");
        ZonedDateTime TEST_VALUE =
            ZonedDateTime.now(TEST_ZONE);

        TEST_VALUE = TEST_VALUE.minusNanos(TEST_VALUE.getNano());

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        /* this blows up with an exception. */
        builder.addZonedDateTime(Field.CERTIFICATE_VALID_FROM, TEST_VALUE);
    }

    /**
     * Test that we cannot turn a Date into a ZonedDateTime.
     */
    @Test(expected = FieldConversionException.class)
    public void buildDateAndParseZonedDateTimeFails() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final UUID ARTIFACT_TYPE =
            UUID.fromString("897d4d04-d5d8-4bf4-b1d1-6884bb362a44");
        final int TEST_FIELD = 0x4307;
        final long BLOCK_HEIGHT = 66;
        final ZoneId TEST_ZONE = ZoneId.of("Europe/Paris");
        final Date TEST_VALUE = new Date(
            new Date().toInstant().getEpochSecond() * 1000);

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addDate(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.sign(PARENT_UUID, PRIVATE_KEY);
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);

        //create a mock certificate contract
        CertificateContract mockContract = mock(CertificateContract.class);
        when(
            mockContract.verify(any(CertificateParser.class))).thenReturn(true);

        //create a mock parser delegate
        CertificateParserDelegate mockDelegate =
            mock(CertificateParserDelegate.class);
        //resolve the parent's UUID in the certificate chain
        when(mockDelegate.resolveEntity(PARENT_UUID, BLOCK_HEIGHT)).thenReturn(
            new EntityReference(
                PARENT_ENCRYPTION_KEY, PARENT_SIGNING_KEY, ARTIFACT_TYPE));

        //attestation should be successful
        assertThat(p.attest(mockDelegate, BLOCK_HEIGHT, false), is(true));

        //read the certificate
        CertificateReader r = new CertificateReader(p);

        //This fails with an exception.
        ZonedDateTime boomVal = r.getFirst(TEST_FIELD).asZonedDateTime();
    }

    /**
     * Test that we can add a ZonedDateTime field to a certificate and parse it.
     */
    @Test
    public void buildAndParseZonedDateTime() throws Exception {
        final UUID CERT_TYPE =
            UUID.fromString("23b11e7f-4260-48b3-993e-6baf1df0e8dd");
        final UUID ARTIFACT_TYPE =
            UUID.fromString("897d4d04-d5d8-4bf4-b1d1-6884bb362a44");
        final int TEST_FIELD = 0x4307;
        final long BLOCK_HEIGHT = 66;
        final ZoneId TEST_ZONE = ZoneId.of("Europe/Paris");
        ZonedDateTime TEST_VALUE =
            ZonedDateTime.now(TEST_ZONE);

        TEST_VALUE = TEST_VALUE.minusNanos(TEST_VALUE.getNano());

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(CERT_TYPE);
        builder.addZonedDateTime(TEST_FIELD, TEST_VALUE);

        Certificate outCert = builder.sign(PARENT_UUID, PRIVATE_KEY);
        assertThat(outCert, is(notNullValue()));

        CertificateParser p = new CertificateParser(outCert);

        //create a mock certificate contract
        CertificateContract mockContract = mock(CertificateContract.class);
        when(
            mockContract.verify(any(CertificateParser.class))).thenReturn(true);

        //create a mock parser delegate
        CertificateParserDelegate mockDelegate =
            mock(CertificateParserDelegate.class);
        //resolve the parent's UUID in the certificate chain
        when(mockDelegate.resolveEntity(PARENT_UUID, BLOCK_HEIGHT)).thenReturn(
            new EntityReference(
                PARENT_ENCRYPTION_KEY, PARENT_SIGNING_KEY, ARTIFACT_TYPE));

        //attestation should be successful
        assertThat(p.attest(mockDelegate, BLOCK_HEIGHT, false), is(true));

        //read the certificate
        CertificateReader r = new CertificateReader(p);

        //check field TEST_FIELD
        assertThat(r.getFirst(TEST_FIELD)
                    .asZonedDateTime()
                    .withZoneSameInstant(TEST_ZONE),
                    is(TEST_VALUE));
    }
}
