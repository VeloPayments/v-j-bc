package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.client.RemoteAgentConfiguration;
import com.velopayments.blockchain.util.UuidUtil;

import java.io.IOException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.Optional;
import java.util.UUID;

public class ProtocolHandlerImpl implements ProtocolHandler {

    public static final int HANDSHAKE_INITIATE_RESPONSE_SIZE = 112;

    public static final int SEND_RECV_UUID_RESPONSE_SIZE = 16 +
            Envelope.OUTER_ENVELOPE_OVERHEAD;

    private RemoteAgentConfiguration remoteAgentConfiguration;
    private RemoteAgentChannel remoteAgentChannel;

    private SecureRandom random;
    private long requestId; // TODO: details on this

    public ProtocolHandlerImpl(RemoteAgentConfiguration remoteAgentConfiguration,
                               RemoteAgentChannel remoteAgentChannel) {
        this.remoteAgentConfiguration = remoteAgentConfiguration;
        this.remoteAgentChannel = remoteAgentChannel;
        this.random = new SecureRandom();
    }

    @Override
    public void handshake() throws IOException {
        byte[] serverChallengeNonce = initiateHandshake();
        acknowledgeHandshake(serverChallengeNonce);
    }

    private byte[] initiateHandshake() throws IOException {
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
        byte[] response = remoteAgentChannel.recv(HANDSHAKE_INITIATE_RESPONSE_SIZE);

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

        byte[] serverKeyNonce = Arrays.copyOfRange(response, 16,48);

        byte[] serverChallengeNonce = Arrays.copyOfRange(response, 48, 80);

        byte[] serverChallengeResponse = Arrays.copyOfRange(response, 80, 112);

        // TODO - verify challenge / response

        return serverChallengeNonce;
    }

    private void acknowledgeHandshake(byte[] serverChallengeNonce) {

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


    public Optional<UUID> sendAndReceiveUUID(ApiMethod apiMethod, UUID uuid) throws IOException {

        // build request payload: UUID (16 bytes) in big endian
        byte[] reqPayload = UuidUtil.getBytesFromUUID(uuid);

        // wrap in inner envelope
        byte[] reqInner = Envelope.wrapInner(apiMethod, requestId, reqPayload);

        // wrap in outer envelope
        byte[] reqOuter = Envelope.wrapOuter(MessageType.AUTHENTICATED,
                remoteAgentConfiguration.getAgentPublicKey().getRawBytes(),
                reqInner);

        // send down channel
        remoteAgentChannel.send(reqOuter);

        // wait for response
        byte[] respOuter = remoteAgentChannel.recv(SEND_RECV_UUID_RESPONSE_SIZE);

        // unwrap outer envelope
        byte[] respInner = Envelope.unwrapOuter(
                remoteAgentConfiguration.getEntityPrivateKey().getRawBytes(),
                respOuter);

        // unwrap inner envelope
        byte[] respPayload = Envelope.unwrapInner(respInner).getPayload();

        // return response
        return UuidUtil.getOptUUIDFromBytes(respPayload);
    }

}
