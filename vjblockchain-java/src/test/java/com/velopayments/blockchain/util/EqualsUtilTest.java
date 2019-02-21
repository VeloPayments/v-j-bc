package com.velopayments.blockchain.util;

import org.junit.Test;

import java.util.UUID;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class EqualsUtilTest {

    @Test
    public void byteArrays() {

        byte[] arr1 = new byte[] { 0, 1, 2, 3 };
        byte[] arr2 = new byte[] { 0, 1, 2, 3 };
        byte[] arr3 = new byte[0];
        byte[] arr4 = new byte[] { 0, 1, 2 };

        assertThat(EqualsUtil.constantTimeEqual(arr1,arr1),is(true));
        assertThat(EqualsUtil.constantTimeEqual(arr1,arr2),is(true));
        assertThat(EqualsUtil.constantTimeEqual(null,null),is(true));
        assertThat(EqualsUtil.constantTimeEqual(null,arr1),is(false));
        assertThat(EqualsUtil.constantTimeEqual(arr1,null),is(false));
        assertThat(EqualsUtil.constantTimeEqual(arr2,arr3),is(false));
        assertThat(EqualsUtil.constantTimeEqual(arr4,arr2),is(false));

        UUID uuid1 = UUID.randomUUID();
        UUID uuid2 = UUID.randomUUID();
        byte[] arr5 = UuidUtil.getBytesFromUUID(uuid1);
        byte[] arr6 = UuidUtil.getBytesFromUUID(uuid1);
        byte[] arr7 = UuidUtil.getBytesFromUUID(uuid2);

        assertThat(EqualsUtil.constantTimeEqual(arr5,arr6),is(true));
        assertThat(EqualsUtil.constantTimeEqual(arr5,arr7),is(false));
    }
}
