package com.velopayments.blockchain.util;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

/**
 * Utility class for operations involving byte arrays.
 */
public class ByteUtil {

    public static byte[] htonl(int val)
    {
        return ByteBuffer.allocate(4).putInt(val)
                .order(ByteOrder.BIG_ENDIAN).array();
    }

    public static byte[] htonll(long val)
    {
        return ByteBuffer.allocate(8).putLong(val)
                .order(ByteOrder.BIG_ENDIAN).array();
    }

    public static long ntohl(byte[] val)
    {
        if (val.length != 4)
        {
            throw new IllegalArgumentException("expected 4 byte array");
        }

        byte[] padded = new byte[8];
        System.arraycopy(val, 0, padded, 4, 4);

        return ByteBuffer.wrap(padded)
                .order(ByteOrder.BIG_ENDIAN).getLong();
    }

    public static long ntohll(byte[] val)
    {
        if (val.length != 8)
        {
            throw new IllegalArgumentException("expected 8 byte array");
        }

        return ByteBuffer.wrap(val)
                .order(ByteOrder.BIG_ENDIAN).getLong();
    }

    /**
     * Convert a long value to a byte array
     *
     * @param val          the value to convert
     *
     * @return the byte array
     */
    public static byte[] longToBytes(long val) {

        ByteBuffer buffer = ByteBuffer.allocate(8);
        buffer.putLong(val);

        return buffer.array();
    }

    /**
     * Convert a byte array to a long value
     *
     * @param bytes         the bytes to convert
     *
     * @return the converted long
     */
    public static long bytesToLong(byte[] bytes) {

        if (bytes.length != 8)
        {
            throw new IllegalArgumentException("expected 8 byte array");
        }

        ByteBuffer buffer = ByteBuffer.allocate(8);
        buffer.put(bytes);
        buffer.flip();

        return buffer.getLong();
    }

    /**
     * Merge two byte arrays into a single array.
     *
     * @param array1 an array of bytes
     * @param array2 an array of bytes
     *
     * @return an array containing all the elements of array1 followed
     *   by all the elements of array2.
     */
    public static byte[] merge(byte[] array1, byte[] array2) {
        byte[] joinedArray = Arrays.copyOf(
                array1, array1.length + array2.length);

        System.arraycopy(
                array2, 0, joinedArray, array1.length, array2.length);

        return joinedArray;
    }
}
