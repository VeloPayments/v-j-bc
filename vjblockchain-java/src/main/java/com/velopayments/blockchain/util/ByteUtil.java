package com.velopayments.blockchain.util;


public class ByteUtil {

    public static byte[] longToBytes(long val, boolean bigEndian) {
        return longToBytes(val, 8, bigEndian);
    }

    public static byte[] longToBytes(long val, int numBytes,
                                     boolean bigEndian) {
        byte[] bytes = new byte[numBytes];

        for (int i=0; i<numBytes; i++) {
            int shift = bigEndian ? (8*(numBytes-1-i)) : (8 * i);
            bytes[i] = (byte) (val >> shift);
        }

        return bytes;
    }

    public static long bytesToLong(byte[] bytes, boolean bigEndian) {
        long val = 0L;

        for (int i=0; i<bytes.length && i<8; i++) {
            int shift = bigEndian ? (8*(bytes.length-1-i)) : (8 * i);
            val |= ((long) bytes[i] & 0xFF) << shift;
        }

        return val;
    }

}
