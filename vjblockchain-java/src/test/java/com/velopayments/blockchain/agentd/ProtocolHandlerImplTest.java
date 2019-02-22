package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.client.RemoteAgentConfiguration;
import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.util.UuidUtil;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

import java.io.IOException;
import java.util.Optional;
import java.util.UUID;

import static org.mockito.Mockito.*;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;
import static com.velopayments.blockchain.agentd.TestUtil.createAgentdResponse;
import static com.velopayments.blockchain.agentd.ProtocolHandlerImpl.*;

public class ProtocolHandlerImplTest {

    ProtocolHandlerImpl protocolHandler;

    RemoteAgentConfiguration config;

    RemoteAgentChannel remoteAgentChannel;

    @Before
    public void setup() {
        EncryptionKeyPair encryptionKeyPair = EncryptionKeyPair.generate();
        config = new RemoteAgentConfiguration(
                "localhost",
                999,
                UUID.randomUUID(), encryptionKeyPair.getPrivateKey(), // entity
                UUID.randomUUID(), encryptionKeyPair.getPublicKey()); // agent

        remoteAgentChannel = Mockito.mock(RemoteAgentChannel.class);

        protocolHandler = new ProtocolHandlerImpl(config, remoteAgentChannel);
    }

    @Test
    public void handshake() throws Exception {

        // given a properly configured handshake instance
        byte[] response = new byte[HANDSHAKE_INITIATE_RESPONSE_SIZE];
        System.arraycopy(UuidUtil.getBytesFromUUID(config.getAgentId()), 0, response, 0, 16);

        Mockito.when(remoteAgentChannel.recv(HANDSHAKE_INITIATE_RESPONSE_SIZE)).thenReturn(response);

        // when the handshake is invoked
        protocolHandler.handshake();

        // then there should have been one round trip  (TODO: two round trips)
        verify(remoteAgentChannel, times(1)).send(Mockito.any());
        verify(remoteAgentChannel, times(1)).recv(HANDSHAKE_INITIATE_RESPONSE_SIZE);
        verifyNoMoreInteractions(remoteAgentChannel);
    }

    @Test
    public void initiateHandshake_IncorrectAgentId() throws Exception {

        // given an improperly configured handshake instance
        byte[] response = new byte[HANDSHAKE_INITIATE_RESPONSE_SIZE];
        System.arraycopy(UuidUtil.getBytesFromUUID(UUID.randomUUID()), 0, response, 0, 16);
        Mockito.when(remoteAgentChannel.recv(HANDSHAKE_INITIATE_RESPONSE_SIZE)).thenReturn(response);

        // when the handshake is initiated
        try {
            protocolHandler.handshake();
            Assert.fail();
        } catch (AgentVerificationException e) {
            // good
        }

    }

    @Test
    public void sendAndReceiveUUID() throws IOException {

        // given a valid request
        ApiMethod apiMethod = ApiMethod.GET_PREV_BLOCK_ID;
        long requestId = 3L;
        UUID reqUuid = UUID.randomUUID();
        UUID respUuid = UUID.randomUUID();

        // set up the response
        Mockito.when(remoteAgentChannel.recv(SEND_RECV_UUID_RESPONSE_SIZE))
                .thenReturn(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                        apiMethod, requestId,0,
                        UuidUtil.getBytesFromUUID(respUuid)));

        // when the API is invoked
        Optional<UUID> apiResponse = protocolHandler.sendAndReceiveUUID(
                apiMethod, reqUuid);


        // then there should have been one round trip
        verify(remoteAgentChannel, times(1)).send(Mockito.any());
        verify(remoteAgentChannel, times(1)).recv(SEND_RECV_UUID_RESPONSE_SIZE);
        verifyNoMoreInteractions(remoteAgentChannel);

        // and the returned value should be correct
        assertThat(apiResponse.get(), is(respUuid));

    }
}
