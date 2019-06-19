package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.util.ByteUtil;
import com.velopayments.blockchain.util.UuidUtil;

import java.util.UUID;

public class TestUtil {


    public static byte[] createHandshakeResponse(UUID agentdId)
    {
        byte response[] = new byte[164];

        // protocol version
        System.arraycopy(ByteUtil.htonl(1), 0, response, 12,4);

        // crypto version suite
        System.arraycopy(ByteUtil.htonl(1), 0, response, 16, 4);

        // agentd ID
        System.arraycopy(UuidUtil.getBytesFromUUID(agentdId), 0, response,
                20, 16);



        return response;
    }

}
