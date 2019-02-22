package com.velopayments.blockchain.util;

public class ByteUtil {

    public static byte[] longToBytes(long val, boolean bigEndian) {
        return longToBytes(val, 8, bigEndian);
    }

    /**
     * Convert a long value to a byte array
     *
     * @param val          the value to convert
     * @param numBytes     the number of (least significant) bytes of the long value to
     *                     convert.  The remaining (most significant) bytes are ignored.
     * @param bigEndian    whether the byte array should be written in big endian
     *
     * @return
     */
    public static byte[] longToBytes(long val, int numBytes,
                                     boolean bigEndian) {
        byte[] bytes = new byte[numBytes];

        for (int i=0; i<numBytes; i++) {
            int shift = bigEndian ? (8*(numBytes-1-i)) : (8 * i);
            bytes[i] = (byte) (val >> shift);
        }

        return bytes;
    }

    /**
     * Convert a byte array to a long value
     *
     * @param bytes         the bytes to convert
     * @param bigEndian     whether the returned value should be written
     *                      in big endian format
     * @return
     */
    public static long bytesToLong(byte[] bytes, boolean bigEndian) {
        long val = 0L;

        for (int i=0; i<bytes.length && i<8; i++) {
            int shift = bigEndian ? (8*(bytes.length-1-i)) : (8 * i);
            val |= ((long) bytes[i] & 0xFF) << shift;
        }

        return val;
    }

}
