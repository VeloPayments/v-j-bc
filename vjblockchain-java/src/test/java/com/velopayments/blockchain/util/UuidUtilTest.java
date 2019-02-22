package com.velopayments.blockchain.util;

import org.junit.Test;

import java.util.UUID;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;


public class UuidUtilTest {

    @Test
    public void uuidToBytesAndBack() {

        UUID uuid = UUID.randomUUID();

        // get bytes
        byte[] uuidBytes = UuidUtil.getBytesFromUUID(uuid);
        assertThat(uuidBytes.length, is(16));

        // convert bytes back into a UUID
        UUID converted = UuidUtil.getUUIDFromBytes(uuidBytes);

        assertThat(converted, is(uuid));
    }

    @Test
    public void uuidToBytes() {

        UUID uuid = UUID.fromString("23000000-0000-0000-0000-000000000012");

        byte[] uuidBytes = UuidUtil.getBytesFromUUID(uuid);

        assertThat(uuidBytes[15], is((byte)0x12));
        assertThat(uuidBytes[0], is((byte)0x23));
    }

    @Test
    public void uuidToBytes_Optional() {

        UUID uuid = UUID.fromString("23000000-0000-0000-0000-000000000012");
        byte[] uuidBytes = UuidUtil.getBytesFromUUID(uuid);

        assertThat(UuidUtil.getOptUUIDFromBytes(uuidBytes).get(),
                is(UuidUtil.getUUIDFromBytes(uuidBytes)));

        assertThat(UuidUtil.getOptUUIDFromBytes(new byte[16]).isPresent(), is(false));
    }

}
