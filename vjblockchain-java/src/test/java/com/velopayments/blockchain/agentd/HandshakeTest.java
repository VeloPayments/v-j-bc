package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.client.RemoteAgentConfiguration;
import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.util.UuidUtil;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

import java.util.UUID;

import static org.mockito.Mockito.*;

public class HandshakeTest {

    Handshake handshake;

    EncryptionPublicKey encryptionPublicKey;
    UUID agentId;
    UUID entityId;

    RemoteAgentChannel remoteAgent;

    @Before
    public void setup() {
        encryptionPublicKey = EncryptionKeyPair.generate().getPublicKey();
        agentId = UUID.randomUUID();
        entityId = UUID.randomUUID();

        RemoteAgentConfiguration config = new RemoteAgentConfiguration(
                "localhost",999, entityId, agentId, encryptionPublicKey);

        remoteAgent = Mockito.mock(RemoteAgentChannel.class);

        handshake = new Handshake(config, remoteAgent);
    }

    @Test
    public void initiateHandshake() throws Exception {

        // given a properly configured handshake instance
        byte[] response = new byte[Handshake.INITIATE_RESPONSE_SIZE];
        System.arraycopy(UuidUtil.getBytesFromUUID(agentId), 0, response, 0, 16);

        Mockito.when(remoteAgent.recv(Handshake.INITIATE_RESPONSE_SIZE)).thenReturn(response);

        // when the handshake is initiated
        handshake.initiate();

       // then there should have been one round trip
        verify(remoteAgent, times(1)).send(Mockito.any());
        verify(remoteAgent, times(1)).recv(Handshake.INITIATE_RESPONSE_SIZE);
        verifyNoMoreInteractions(remoteAgent);
    }

    @Test
    public void initiateHandshake_IncorrectAgentId() throws Exception {

        // given an improperly configured handshake instance
        byte[] response = new byte[Handshake.INITIATE_RESPONSE_SIZE];
        System.arraycopy(UuidUtil.getBytesFromUUID(UUID.randomUUID()), 0, response, 0, 16);
        Mockito.when(remoteAgent.recv(Handshake.INITIATE_RESPONSE_SIZE)).thenReturn(response);

        // when the handshake is initiated
        try {
            handshake.initiate();
            Assert.fail();
        } catch (AgentVerificationException e) {
            // good
        }


    }


}
