package com.velopayments.blockchain.agentd;

import java.util.Optional;
import java.util.UUID;

public class GetNextBlockId {

    private RemoteAgentChannel remoteAgentChannel;

    public GetNextBlockId(RemoteAgentChannel remoteAgentChannel) {
        this.remoteAgentChannel = remoteAgentChannel;
    }


    public Optional<UUID> getNextBlockId(long requestId,UUID blockId) {

        // build request payload: UUID (16 bytes) in big endian

        // wrap in inner envelope -- API method, request ID, payload

        // wrap in outer envelope (will need key)

        // send down channel

        // wait for response

        // unwrap outer envelope

        // unwrap inner envelope

        // build response

        return Optional.empty();
    }

}
