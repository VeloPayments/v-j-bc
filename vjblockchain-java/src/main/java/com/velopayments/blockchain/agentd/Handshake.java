package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.client.RemoteAgentConfiguration;
import com.velopayments.blockchain.util.UuidUtil;

import java.io.IOException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.UUID;

public class Handshake {

    public static final int INITIATE_RESPONSE_SIZE = 112;

    private RemoteAgentConfiguration remoteAgentConfiguration;
    private RemoteAgentChannel remoteAgentChannel;
    private SecureRandom random;

    private byte[] serverChallengeNonce;

    public Handshake(RemoteAgentConfiguration remoteAgentConfiguration,
                     RemoteAgentChannel remoteAgentChannel) {
        this.remoteAgentConfiguration = remoteAgentConfiguration;
        this.remoteAgentChannel = remoteAgentChannel;
        this.random = new SecureRandom();
        this.serverChallengeNonce = new byte[32];
    }

    public void initiate() throws IOException {

        /*
         * request:
         *    bytes 0-15: entity ID
         *    bytes 16-47: client key nonce
         *    bytes 48-79: client challenge nonce
         */
        byte[] entityIdBytes = UuidUtil.getBytesFromUUID(remoteAgentConfiguration.getEntityId());

        byte clientKeyNonce[] = new byte[32];
        random.nextBytes(clientKeyNonce);

        byte clientChallengeNonce[] = new byte[32];
        random.nextBytes(clientChallengeNonce);

        byte[] request = new byte[entityIdBytes.length + clientKeyNonce.length + clientChallengeNonce.length];
        System.arraycopy(entityIdBytes, 0, request, 0, 16);
        System.arraycopy(clientKeyNonce, 0, request, 16, 32);
        System.arraycopy(clientChallengeNonce, 0, request, 48, 32);


        // send handshake request
        remoteAgentChannel.send(request);

        // receive response
        byte[] response = remoteAgentChannel.recv(INITIATE_RESPONSE_SIZE);

        /*
         * response:
         *    bytes 0-15: agent ID
         *    bytes 16-47: server key nonce
         *    bytes 48-79: server challenge nonce
         *    bytes 80-111 : server challenge/response
         */

        // verify agent UUID
        // TODO - use timing resistant equality check
        UUID agentUUID = UuidUtil.getUUIDFromBytes(Arrays.copyOfRange(response, 0, 16));
        if (! remoteAgentConfiguration.getAgentId().equals(agentUUID)) {
            throw new AgentVerificationException("Expected agent ID " + remoteAgentConfiguration.getAgentId()
                    + " but received " + agentUUID);
        }

        byte[] serverKeyNonce = new byte[32];
        serverKeyNonce = Arrays.copyOfRange(response, 16,48);

        byte[] serverChallengeNonce = new byte[32];
        serverChallengeNonce = Arrays.copyOfRange(response, 48, 80);

        byte[] serverChallengeResponse = Arrays.copyOfRange(response, 80, 112);

        // TODO - verify challenge / response
    }


    public void acknowledge() {

        /*
         * request:
         *    HMAC: server challenge nonce / session key
         */
        byte[] request = new byte[32];
        // TODO: construct request


        // TODO: send and receive

        /*
         * response:
         *    "OK" [AUTHED]
         */

        // TODO: verify response
    }
}
