package com.velopayments.blockchain.agentd;

import java.security.SecureRandom;
import java.util.Arrays;
import java.util.UUID;

public class Handshake {

    private RemoteAgentConfiguration remoteAgentConfiguration;
    private RemoteAgent remoteAgent;
    private SecureRandom random;
    private HMAC hmac;

    private byte[] serverChallengeNonce;

    public Handshake(RemoteAgentConfiguration remoteAgentConfiguration,RemoteAgent remoteAgent) {
        this.remoteAgentConfiguration = remoteAgentConfiguration;
        this.remoteAgent = remoteAgent;

        this.random = new SecureRandom();
        this.hmac = new HMAC();

        this.serverChallengeNonce = new byte[32];
    }

    public void initiate() {

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


        // send request to remote agent (agentd)
        byte[] response = remoteAgent.send(request);

        /*
         * response:
         *    bytes 0-15: agent ID
         *    bytes 16-47: server key nonce
         *    bytes 48-79: server challenge nonce
         *    bytes 80-111 : server challenge/response
         */

        // verify agent UUID
        // TODO - use timing resistant equality check
        UUID agentUUID = UuidUtil.getUUIDFromBytes(Arrays.copyOfRange(response, 0, 15));
        if (! remoteAgentConfiguration.getAgentId().equals(agentUUID)) {
            throw new AgentVerificationException("Expected agent ID " + remoteAgentConfiguration.getAgentId()
                    + " but received " + agentUUID);
        }

        byte[] serverKeyNonce = new byte[32];
        serverKeyNonce = Arrays.copyOfRange(response, 16,31);

        byte[] serverChallengeNonce = new byte[32];
        serverChallengeNonce = Arrays.copyOfRange(response, 32, 63);

        byte[] serverChallengeResponse = Arrays.copyOfRange(response, 64, 95);

        // TODO - verify challenge / response
    }


    public void acknowledge() {

        /*
         * request:
         *    HMAC: server challenge / session key
         */
        byte[] request = new byte[32];
        // TODO: construct request


        byte[] response = remoteAgent.send(request);

        /*
         * response:
         *    "OK" [AUTHED]
         */

        // TODO: verify response
    }
}
