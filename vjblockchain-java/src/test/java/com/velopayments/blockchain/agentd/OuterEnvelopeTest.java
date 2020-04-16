package com.velopayments.blockchain.agentd;

import org.apache.commons.codec.binary.Hex;
import org.junit.Test;

import java.util.Arrays;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class OuterEnvelopeTest {

    /**
     * Subsequent payloads should be different, because the writer IV starts at
     * 0x0000000000000001 and increments each time it is written.  The server
     * keeps in lock step with this IV.
     */
    @Test
    public void writerIncrementsIV() {
        byte[] TEST_KEY = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
        byte[] TEST_PAYLOAD = {
            1, 2, 3, 4, 5, 6 ,7, 8 };
        byte[] EXPECTED_RESULT_01 = {
            (byte)0xa2, (byte)0x1b, (byte)0x44, (byte)0x16, (byte)0xff,
            (byte)0x68, (byte)0xfe, (byte)0x59, (byte)0x78, (byte)0x18,
            (byte)0x1d, (byte)0xf1, (byte)0x82, (byte)0xf4, (byte)0x82,
            (byte)0x8a, (byte)0xbe, (byte)0x14, (byte)0x26, (byte)0x1d,
            (byte)0x48, (byte)0xf9, (byte)0xa3, (byte)0x1a, (byte)0x86,
            (byte)0x72, (byte)0x1f, (byte)0xca, (byte)0xec, (byte)0x62,
            (byte)0xde, (byte)0x9c, (byte)0x4d, (byte)0x27, (byte)0x90,
            (byte)0x6e, (byte)0x5c, (byte)0x59, (byte)0x0e, (byte)0xbd,
            (byte)0x43, (byte)0x0b, (byte)0xa6, (byte)0xe5, (byte)0x3c };
        byte[] EXPECTED_RESULT_02 = {
            (byte)0x78, (byte)0x2f, (byte)0xc1, (byte)0x59, (byte)0x23,
            (byte)0x43, (byte)0x33, (byte)0x07, (byte)0x6f, (byte)0xa9,
            (byte)0x32, (byte)0xaf, (byte)0x0c, (byte)0xf3, (byte)0x04,
            (byte)0x61, (byte)0xce, (byte)0x61, (byte)0x5f, (byte)0x49,
            (byte)0xec, (byte)0x7d, (byte)0xc7, (byte)0x8e, (byte)0x59,
            (byte)0xc0, (byte)0xa1, (byte)0xb5, (byte)0x2d, (byte)0xe6,
            (byte)0x98, (byte)0x99, (byte)0xf3, (byte)0x56, (byte)0xed,
            (byte)0xeb, (byte)0x1c, (byte)0x7c, (byte)0x81, (byte)0xb7,
            (byte)0xac, (byte)0x26, (byte)0xb5, (byte)0x49, (byte)0xde };

        OuterEnvelopeWriter writer = new OuterEnvelopeWriter();

        assertThat(
            writer.encryptPayload(TEST_KEY, TEST_PAYLOAD),
            equalTo(EXPECTED_RESULT_01));
        assertThat(
            writer.encryptPayload(TEST_KEY, TEST_PAYLOAD),
            equalTo(EXPECTED_RESULT_02));
    }

    /**
     * Subsequent payloads should be different, because the reader IV starts at
     * 0x8000000000000001 and increments each time it is written.  The server
     * keeps in lock step with this IV.
     */
    @Test
    public void readerIncrementsIV() {
        byte[] TEST_KEY = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
        byte[] EXPECTED_PAYLOAD = {
            1, 2, 3, 4, 5, 6 ,7, 8 };
        byte[] INPUT_BYTES_01 = {
            (byte)0x7e, (byte)0x51, (byte)0xdf, (byte)0x6f, (byte)0x09,
            (byte)0x01, (byte)0x27, (byte)0x90, (byte)0x51, (byte)0xe0,
            (byte)0x70, (byte)0x4b, (byte)0x34, (byte)0xc8, (byte)0xef,
            (byte)0x71, (byte)0x82, (byte)0xbc, (byte)0x88, (byte)0xd9,
            (byte)0xea, (byte)0x2e, (byte)0x66, (byte)0xa6, (byte)0xc0,
            (byte)0xd6, (byte)0xcb, (byte)0x57, (byte)0xf4, (byte)0xa0,
            (byte)0x5e, (byte)0x04, (byte)0x07, (byte)0xd0, (byte)0xdf,
            (byte)0x2c, (byte)0xb3, (byte)0xb2, (byte)0x9c, (byte)0x17,
            (byte)0xad, (byte)0xe5, (byte)0xb1, (byte)0xcd, (byte)0x36 };
        byte[] INPUT_BYTES_02 = {
            (byte)0x8d, (byte)0xdb, (byte)0xec, (byte)0xb2, (byte)0x03,
            (byte)0x49, (byte)0xc0, (byte)0x78, (byte)0xa6, (byte)0x16,
            (byte)0x80, (byte)0xc9, (byte)0xff, (byte)0xff, (byte)0x0c,
            (byte)0x03, (byte)0x55, (byte)0xe0, (byte)0x95, (byte)0x3a,
            (byte)0x4c, (byte)0xbd, (byte)0x70, (byte)0x60, (byte)0xa0,
            (byte)0x6d, (byte)0x38, (byte)0xa1, (byte)0xba, (byte)0x4e,
            (byte)0x03, (byte)0xc2, (byte)0x6a, (byte)0x8e, (byte)0x34,
            (byte)0x66, (byte)0x29, (byte)0xa3, (byte)0x8f, (byte)0x50,
            (byte)0x16, (byte)0x52, (byte)0xdd, (byte)0x28, (byte)0x58 };

        OuterEnvelopeReader reader = new OuterEnvelopeReader();

        assertThat(
            reader.decryptPayload(
                TEST_KEY,
                Arrays.copyOfRange(INPUT_BYTES_01, 0, 5),
                Arrays.copyOfRange(INPUT_BYTES_01, 5, INPUT_BYTES_01.length)),
            equalTo(EXPECTED_PAYLOAD));
        assertThat(
            reader.decryptPayload(
                TEST_KEY,
                Arrays.copyOfRange(INPUT_BYTES_02, 0, 5),
                Arrays.copyOfRange(INPUT_BYTES_02, 5, INPUT_BYTES_02.length)),
            equalTo(EXPECTED_PAYLOAD));
    }

    /**
     * When a packet has an invalid MAC, reader#decryptPayload() throws a
     * MessageVerificationException.
     */
    @Test(expected = MessageVerificationException.class)
    public void readerValidatesMAC() {
        byte[] TEST_KEY = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
        byte[] EXPECTED_PAYLOAD = {
            1, 2, 3, 4, 5, 6 ,7, 8 };
        byte[] BAD_INPUT_BYTES = {
            (byte)0x7e, (byte)0x51, (byte)0xdf, (byte)0x6f, (byte)0x09,
            (byte)0x01, (byte)0x27, (byte)0x90, (byte)0x51, (byte)0xe0,
            /*(byte)0x70, (byte)0x4b, (byte)0x34, (byte)0xc8, (byte)0xef,*/
            (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
            (byte)0x71, (byte)0x82, (byte)0xbc, (byte)0x88, (byte)0xd9,
            (byte)0xea, (byte)0x2e, (byte)0x66, (byte)0xa6, (byte)0xc0,
            (byte)0xd6, (byte)0xcb, (byte)0x57, (byte)0xf4, (byte)0xa0,
            (byte)0x5e, (byte)0x04, (byte)0x07, (byte)0xd0, (byte)0xdf,
            (byte)0x2c, (byte)0xb3, (byte)0xb2, (byte)0x9c, (byte)0x17,
            (byte)0xad, (byte)0xe5, (byte)0xb1, (byte)0xcd, (byte)0x36 };

        OuterEnvelopeReader reader = new OuterEnvelopeReader();

        reader.decryptPayload(
            TEST_KEY,
            Arrays.copyOfRange(BAD_INPUT_BYTES, 0, 5),
            Arrays.copyOfRange(BAD_INPUT_BYTES, 5, BAD_INPUT_BYTES.length));
    }

}
