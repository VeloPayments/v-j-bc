package com.velopayments.blockchain.util;

/**
 * Utility class for doing timing resistant equality checks.
 *
 */
public class EqualsUtil {

    /**
     * Timing resistant equality check. Assuming both arrays
     * are non-null, the time is proportional to the length of
     * the longest array.  That is, there are no early exits
     * if byte a[n] != b[n].
     *
     * @param a  first byte array
     * @param b  second byte array
     *
     * @return true if a==b, otherwise false
     */
    public static boolean constantTimeEqual(byte[] a, byte[] b) {


        if (a==null && b==null) {
            return true;
        }

        if (a==null || b==null) {
            return false;
        }

        // scan each array fully
        int n = a.length > b.length ? a.length : b.length;

        boolean match = true;
        for (int i=0; i<n; i++) {
            if (i >= a.length || i >= b.length || a[i] != b[i])
            {
                match = false;
            }
        }

        return match;
    }

}
