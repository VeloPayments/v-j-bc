package com.velopayments.blockchain.util;

import org.junit.Test;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class ByteUtilTest {

    @Test
    public void littleEndian() {

        long val = 0x0a0b0c0dL;

        byte[] bytes = ByteUtil.longToBytes(val, false);

        assertThat(bytes.length, is(8));
        assertThat(bytes[0], is((byte)0x0d));
        assertThat(bytes[1], is((byte)0x0c));
        assertThat(bytes[2], is((byte)0x0b));
        assertThat(bytes[3], is((byte)0x0a));
        assertThat(bytes[4], is((byte)0));
        assertThat(bytes[5], is((byte)0));
        assertThat(bytes[6], is((byte)0));
        assertThat(bytes[7], is((byte)0));


        bytes = ByteUtil.longToBytes(val, 2,false);

        assertThat(bytes.length, is(2));
        assertThat(bytes[0], is((byte)0x0d));
        assertThat(bytes[1], is((byte)0x0c));
    }

    @Test
    public void bigEndian() {
        long val = 0x0a0b0c0dL;

        byte[] bytes = ByteUtil.longToBytes(val, true);

        assertThat(bytes.length, is(8));
        assertThat(bytes[7], is((byte)0x0d));
        assertThat(bytes[6], is((byte)0x0c));
        assertThat(bytes[5], is((byte)0x0b));
        assertThat(bytes[4], is((byte)0x0a));
        assertThat(bytes[3], is((byte)0));
        assertThat(bytes[2], is((byte)0));
        assertThat(bytes[1], is((byte)0));
        assertThat(bytes[0], is((byte)0));

        bytes = ByteUtil.longToBytes(val, 2,true);

        assertThat(bytes.length, is(2));
        assertThat(bytes[1], is((byte)0x0d));
        assertThat(bytes[0], is((byte)0x0c));
    }

    @Test
    public void roundTrip_LittleEndian() {

        conversionRoundtrip(0L,false);
        conversionRoundtrip(123L,false);
        conversionRoundtrip(67892L,false);
        conversionRoundtrip(-10L,false);
        conversionRoundtrip(Long.MAX_VALUE,false);
        conversionRoundtrip(Long.MIN_VALUE,false);
    }

    @Test
    public void roundTrip_BigEndian() {

        conversionRoundtrip(0L,true);
        conversionRoundtrip(123L,true);
        conversionRoundtrip(67892L,true);
        conversionRoundtrip(-10L,true);
        conversionRoundtrip(Long.MAX_VALUE,true);
        conversionRoundtrip(Long.MIN_VALUE,true);
    }

    @Test
    public void fixedLength_LittleEndian() {

        long val = 0x0807060504030201L;

        assertThat(ByteUtil.longToBytes(val, 1, false),
                is(new byte[]{ (byte)0x01 }));

        assertThat(ByteUtil.longToBytes(val, 2, false),
                is(new byte[]{ (byte)0x01, (byte)0x02 }));

        assertThat(ByteUtil.longToBytes(val, 5, false),
                is(new byte[]{ (byte)0x01, (byte)0x02, (byte)0x03,
                        (byte)0x04, (byte)0x05
                }));

        assertThat(ByteUtil.longToBytes(val, 8, false),
                is(new byte[]{(byte) 0x01, (byte) 0x02, (byte) 0x03,
                        (byte) 0x04, (byte) 0x05, (byte) 0x06,
                        (byte) 0x07, (byte) 0x08
                }));
    }

    @Test
    public void fixedLength_BigEndian() {

        long val = 0x0807060504030201L;

        assertThat(ByteUtil.longToBytes(val, 1, true),
                is(new byte[]{ (byte)0x01 }));

        assertThat(ByteUtil.longToBytes(val, 2, true),
                is(new byte[]{ (byte)0x02, (byte)0x01 }));

        assertThat(ByteUtil.longToBytes(val, 5, true),
                is(new byte[]{ (byte)0x05, (byte)0x04, (byte)0x03,
                        (byte)0x02, (byte)0x01
                }));

        assertThat(ByteUtil.longToBytes(val, 8, true),
                is(new byte[]{(byte) 0x08, (byte) 0x07, (byte) 0x06,
                        (byte) 0x05, (byte) 0x04, (byte) 0x03,
                        (byte) 0x02, (byte) 0x01
                }));
    }

    private void conversionRoundtrip(long val, boolean bigEndian) {
        byte[] bytes = ByteUtil.longToBytes(val,bigEndian);
        long converted = ByteUtil.bytesToLong(bytes, bigEndian);
        assertThat(converted, is(val));
    }
}
