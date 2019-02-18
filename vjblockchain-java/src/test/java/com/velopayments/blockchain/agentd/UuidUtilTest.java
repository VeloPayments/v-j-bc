package com.velopayments.blockchain.agentd;

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

}
