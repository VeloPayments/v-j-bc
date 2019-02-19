package com.velopayments.blockchain.util;

import org.junit.Test;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class ByteUtilTest {

    @Test
    public void longToBytesAndBack() {

        conversionRoundtrip(0L);
        conversionRoundtrip(123L);
        conversionRoundtrip(67892L);
        conversionRoundtrip(-10L);
        conversionRoundtrip(Long.MAX_VALUE);
        conversionRoundtrip(Long.MIN_VALUE);
    }

    @Test
    public void fixedLength() {

        long val = 0x0807060504030201L;

        assertThat(ByteUtil.longToBytes(val, 1),
                is(new byte[]{ (byte)0x01 }));

        assertThat(ByteUtil.longToBytes(val, 2),
                is(new byte[]{ (byte)0x01, (byte)0x02 }));

        assertThat(ByteUtil.longToBytes(val, 5),
                is(new byte[]{ (byte)0x01, (byte)0x02, (byte)0x03,
                        (byte)0x04, (byte)0x05
                }));

        assertThat(ByteUtil.longToBytes(val, 8),
                is(new byte[]{(byte) 0x01, (byte) 0x02, (byte) 0x03,
                        (byte) 0x04, (byte) 0x05, (byte) 0x06,
                        (byte) 0x07, (byte) 0x08
                }));
    }

    private void conversionRoundtrip(long val) {
        byte[] bytes = ByteUtil.longToBytes(val);
        long converted = ByteUtil.bytesToLong(bytes);
        assertThat(converted, is(val));
    }
}
