package com.velopayments.blockchain.agentd;

import org.junit.Test;

import java.util.UUID;

public class UuidUtilTest {

    @Test
    public void uuidToBytesAndBack() {

        UUID uuid = UUID.randomUUID();

        byte[] uuidBytes = UuidUtil.getBytesFromUUID(uuid);



    }

}
