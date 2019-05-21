package com.velopayments.blockchain.util;

import org.junit.Test;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class ByteUtilTest {

    @Test
    public void htonll() {
        long val = 0x0a0b0c0dL;

        byte[] bytes = ByteUtil.htonll(val);

        assertThat(bytes.length, is(8));
        assertThat(bytes[7], is((byte)0x0d));
        assertThat(bytes[6], is((byte)0x0c));
        assertThat(bytes[5], is((byte)0x0b));
        assertThat(bytes[4], is((byte)0x0a));
        assertThat(bytes[3], is((byte)0));
        assertThat(bytes[2], is((byte)0));
        assertThat(bytes[1], is((byte)0));
        assertThat(bytes[0], is((byte)0));
    }

    @Test
    public void ntohll() {

        byte[] bytes = new byte[] { 0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d };

        assertThat(ByteUtil.ntohll(bytes), is(0x060708090a0b0c0dL));
    }

    @Test
    public void roundTrip() {

        conversionRoundtrip(0L);
        conversionRoundtrip(123L);
        conversionRoundtrip(67892L);
        conversionRoundtrip(-10L);
        conversionRoundtrip(Long.MAX_VALUE);
        conversionRoundtrip(Long.MIN_VALUE);
    }

    private void conversionRoundtrip(long val) {
        byte[] bytes = ByteUtil.longToBytes(val);
        long converted = ByteUtil.bytesToLong(bytes);
        assertThat(converted, is(val));
    }
}
