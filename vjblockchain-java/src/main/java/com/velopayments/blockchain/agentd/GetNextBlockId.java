package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.util.UuidUtil;

import java.io.IOException;
import java.util.Optional;
import java.util.UUID;

public class GetNextBlockId {

    public static final int RESPONSE_SIZE = 16 + Envelope.OUTER_ENVELOPE_OVERHEAD;

    private RemoteAgentChannel remoteAgentChannel;

    public GetNextBlockId(RemoteAgentChannel remoteAgentChannel) {
        this.remoteAgentChannel = remoteAgentChannel;
    }


    public Optional<UUID> getNextBlockId(EncryptionPublicKey encryptionKey,
                                         EncryptionPrivateKey decryptionKey,
                                         long requestId, UUID blockId) throws IOException {

        // build request payload: UUID (16 bytes) in big endian
        byte[] reqPayload = UuidUtil.getBytesFromUUID(blockId);

        // wrap in inner envelope
        byte[] reqInner = Envelope.wrapInner(ApiMethod.GET_NEXT_BLOCK_ID, requestId, reqPayload);

        // wrap in outer envelope
        byte[] reqOuter = Envelope.wrapOuter(MessageType.AUTHENTICATED,
                encryptionKey.getRawBytes(), reqInner);

        // send down channel
        remoteAgentChannel.send(reqOuter);

        // wait for response
        byte[] respOuter = remoteAgentChannel.recv(RESPONSE_SIZE);

        // unwrap outer envelope
        byte[] respInner = Envelope.unwrapOuter(decryptionKey.getRawBytes(), respOuter);

        // unwrap inner envelope
        byte[] respPayload = Envelope.unwrapInner(respInner).getPayload();

        // return response
        return UuidUtil.getOptUUIDFromBytes(respPayload);
    }

}
