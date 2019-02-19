package com.velopayments.blockchain.util;


public class ByteUtil {

    public static byte[] longToBytes(long val) {
        return longToBytes(val, 8);
    }

    public static byte[] longToBytes(long val, int numBytes) {
        byte[] bytes = new byte[numBytes];

        for (int i=0; i<numBytes; i++) {
            bytes[i] = (byte) (val >> (8 * i));
        }

        return bytes;
    }

    public static long bytesToLong(byte[] bytes) {

        long val = 0L;

        for (int i=0; i<bytes.length && i<8; i++) {
            val |= ((long) bytes[i] & 0xFF) << (i*8);
        }

        return val;
        /*return ((long) bytes[7] << 56)
             | ((long) bytes[6] & 0xff) << 48
             | ((long) bytes[5] & 0xff) << 40
             | ((long) bytes[4] & 0xff) << 32
             | ((long) bytes[3] & 0xff) << 24
             | ((long) bytes[2] & 0xff) << 16
             | ((long) bytes[1] & 0xff) << 8
             | ((long) bytes[0] & 0xff);*/
    }

}
