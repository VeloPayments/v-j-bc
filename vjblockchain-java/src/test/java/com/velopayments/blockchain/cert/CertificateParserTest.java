package com.velopayments.blockchain.cert;

import org.junit.Test;

import java.nio.ByteBuffer;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.is;
import static org.hamcrest.Matchers.notNullValue;
import static org.mockito.Mockito.*;

public class CertificateParserTest {

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
    private static ByteBuffer PRIVATE_KEY;

    //run once to convert private key
    static {
        PRIVATE_KEY = convShorts(PRIVATE_KEY_SHORTS);
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
     * Test that an empty certificate throws an InvalidArgumentException.
     */
    @Test(expected = IllegalArgumentException.class)
    public void emptyCertificateTest() throws Exception {
        short TEST_CERT[] = {
        };

        //build the cert as byte values
        ByteBuffer cert = convShorts(TEST_CERT);

        //create a parser for this cert
        CertificateParser p =
            new CertificateParser(
                Certificate.fromByteArray(cert.array()));

        //parse the raw certificate
        Map<Integer, List<byte[]>> certMap = p.parse();
    }

    /**
     * Test that a truncated certificate throws an IllegalArgumentException.
     */
    @Test(expected = IllegalArgumentException.class)
    public void truncatedCertificateTest() throws Exception {
        short TEST_CERT[] = {
            //field 0x0001 is truncated
            0x00, 0x01, 0x00, 0x04, 0xFF, 0xFF
        };

        //build the cert as byte values
        ByteBuffer cert = convShorts(TEST_CERT);

        //create a parser for this cert
        CertificateParser p =
            new CertificateParser(
                Certificate.fromByteArray(cert.array()));

        //parse the raw certificate
        Map<Integer, List<byte[]>> certMap = p.parse();
    }

    /**
     * Test that we can parse a normal certificate.
     */
    @Test
    public void normalCertificateTest() throws Exception {
        short TEST_CERT[] = {
            //field 0x0001 is 0x01020304
            0x00, 0x01, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04,
            //field 0x7002 is 0x01
            0x70, 0x02, 0x00, 0x01, 0x01,
            //field 0x0001 is 0xFFFFFFFF
            0x00, 0x01, 0x00, 0x04, 0xFF, 0xFF, 0xFF, 0xFF,
            //field 0x7007 is 0x13
            0x70, 0x07, 0x00, 0x01, 0x13,
            //field 0x7000 is 0x56
            0x70, 0x00, 0x00, 0x01, 0x56,
            //field 0x0001 is 0x77777777
            0x00, 0x01, 0x00, 0x04, 0x77, 0x77, 0x77, 0x77
        };

        //build the cert as byte values
        ByteBuffer cert = convShorts(TEST_CERT);

        //create a parser for this cert
        CertificateParser p =
            new CertificateParser(
                Certificate.fromByteArray(cert.array()));

        //read the raw certificate
        CertificateReader r = new CertificateReader(p);
        Map<Integer, List<byte[]>> certMap = p.parse();

        // check that we can get the same certificate back from the parser and reader
        assertThat(p.getCertificate().toByteArray(), is(cert.array()));
        assertThat(r.getCertificate().toByteArray(), is(cert.array()));

        //check that we parsed each field value
        assertThat(certMap, is(notNullValue()));
        assertThat(certMap.isEmpty(), is(false));

        //check field 0x7002
        assertThat(r.count(0x7002), is(1));
        assertThat(r.getFirst(0x7002).asByte(), is((byte) 0x01));

        //check field 0x7007
        assertThat(r.count(0x7007), is(1));
        assertThat(r.getFirst(0x7007).asByte(), is((byte) 0x13));

        //check field 0x7000
        assertThat(r.count(0x7000), is(1));
        assertThat(r.getFirst(0x7000).asByte(), is((byte) 0x56));

        //There should be three 0x0001 fields
        assertThat(r.count(0x0001), is(3));
        assertThat(r.get(0x0001, 0).asInt(), is(0x01020304));
        assertThat(r.get(0x0001, 1).asInt(), is(-1));
        assertThat(r.get(0x0001, 2).asInt(), is(0x77777777));
    }

    /**
     * Test that we can successfully attest a valid certificate.
     */
    @Test
    public void attestationHappyPath() throws Exception {
        short TEST_CERT[] = {
            //field 0x0001 is 0x01020304
            0x00, 0x01, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04,
            //field 0x1002 is "Testing 1 2 3"
            0x10, 0x02, 0x00, 0x0d, 0x54, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x67,
            0x20, 0x31, 0x20, 0x32, 0x20, 0x33,
            //field 0x1735 is 0x01
            0x17, 0x35, 0x00, 0x01, 0x01,
            //field 0x0050 is the signer's UUID
            0x00, 0x50, 0x00, 0x10, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
            0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
            0x0e, 0x0f,
            //field 0x0051 is the signature
            0x00, 0x51, 0x00, 0x40, 0x8d, 0x7b, 0x94, 0x3e, 0x85, 0xa1, 0x17,
            0xda, 0x4e, 0x05, 0xbc, 0x90, 0x08, 0x89,
            0xd2, 0x90, 0x50, 0xad, 0x7d, 0x78, 0x99,
            0xb4, 0x17, 0xd9, 0xe1, 0xdf, 0xe5, 0xa2,
            0x3a, 0x95, 0x52, 0x6e, 0x02, 0xc5, 0xfd,
            0x71, 0xcc, 0x69, 0x01, 0x67, 0x75, 0xcf,
            0x0e, 0x53, 0x04, 0xd0, 0xb4, 0xff, 0x4c,
            0x69, 0xc7, 0x8e, 0xfa, 0x81, 0x0e, 0x8e,
            0xe9, 0x35, 0x38, 0xbf, 0x9d, 0xd8, 0xad,
            0x0d,
            //field 0x1738 is 0x07. This field occurs after the signature and
            //should NOT be part of the attested certificate
            0x17, 0x35, 0x00, 0x01, 0x07,
        };
        short PARENT_ENCRYPTION_KEY[] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        };
        short PARENT_SIGNING_KEY[] = {
            0x47, 0x83, 0xe9, 0xf6, 0x78, 0xd1, 0x49, 0xac,
            0xd2, 0x09, 0x66, 0xb0, 0xab, 0x88, 0xf7, 0xd0,
            0x5d, 0x6d, 0x4f, 0x54, 0x0f, 0x1f, 0x23, 0x82,
            0x86, 0x00, 0x3a, 0xda, 0x0c, 0x27, 0xcc, 0x35
        };
        UUID PARENT_UUID =
            UUID.fromString("00010203-0405-0607-0809-0a0b0c0d0e0f");
        UUID PARENT_ARTIFACT_TYPE =
            UUID.fromString("897d4d04-d5d8-4bf4-b1d1-6884bb362a44");

        long BLOCK_HEIGHT = 77;

        //build the cert as byte values
        ByteBuffer cert = convShorts(TEST_CERT);
        //build the parent cert as byte values
        ByteBuffer parentEncryptionKey = convShorts(PARENT_ENCRYPTION_KEY);
        ByteBuffer parentSigningKey = convShorts(PARENT_SIGNING_KEY);

        //create a parser for this cert
        CertificateParser p =
            new CertificateParser(
                Certificate.fromByteArray(cert.array()));

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
                    parentEncryptionKey.array(), parentSigningKey.array(),
                    PARENT_ARTIFACT_TYPE));

        //attestation should be successful
        assertThat(p.attest(mockDelegate, BLOCK_HEIGHT, false), is(true));

        //parse the attested certificate
        Map<Integer, List<byte[]>> certMap = p.parse();
        CertificateReader r = new CertificateReader(p);

        //The following fields should be part of the attested certificate...
        assertThat(r.count(0x0001), is(1));
        assertThat(r.getFirst(0x0001).asInt(), is(0x01020304));
        assertThat(r.count(0x1002), is(1));
        assertThat(r.getFirst(0x1002).asString(), is("Testing 1 2 3"));
        assertThat(r.count(0x1735), is(1));
        assertThat(r.getFirst(0x1735).asByte(), is((byte) 0x01));
        assertThat(r.count(Field.SIGNER_ID), is(1));
        assertThat(r.getFirst(Field.SIGNER_ID).asUUID(), is(PARENT_UUID));

        //The signature should NOT be part of the attested certificate
        assertThat(r.count(Field.SIGNATURE), is(0));
        //field 1738 occurs after the signature and SHOULD NOT be part of the
        //attested certificate
        assertThat(r.count(0x1738), is(0));
    }
}
