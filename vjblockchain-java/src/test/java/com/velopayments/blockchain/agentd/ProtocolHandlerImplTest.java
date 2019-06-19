package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

import java.io.IOException;
import java.security.SecureRandom;
import java.util.UUID;

import static org.mockito.Mockito.*;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class ProtocolHandlerImplTest {

    ProtocolHandlerImpl protocolHandler;

    DataChannel dataChannel;
    UUID agentId;
    UUID entityId;
    EncryptionPrivateKey entityPrivateKey;
    SecureRandom random;

    @Before
    public void setup() {

        dataChannel = Mockito.mock(DataChannel.class);
        agentId = UUID.randomUUID();
        entityId = UUID.randomUUID();
        entityPrivateKey = EncryptionKeyPair.generate().getPrivateKey();
        random = Mockito.mock(SecureRandom.class);

        protocolHandler = new ProtocolHandlerImpl(dataChannel, agentId,
                entityId, entityPrivateKey, random);
    }

    @Test
    public void handshake_happyPath() throws Exception {

        // given a valid initiate handshake response header
        // and a valid ack response header
        Mockito.when(dataChannel.recv(5))
                .thenReturn(TestUtil.createInitiateHandshakeResponseHeader())
                .thenReturn(TestUtil.createAckHandshakeResponseHeader());


        // and a valid initiate handshake response body
        Mockito.when(dataChannel.recv(164))
                .thenReturn(TestUtil.createInitiateHandshakeResponse(
                        agentId, entityPrivateKey));


        // when the handshake is invoked
        //protocolHandler.handshake();

        // then there should have been two round trips
        //verify(dataChannel, times(1)).send(Mockito.any());
        //verify(dataChannel, times(1)).recv();
        //verifyNoMoreInteractions(dataChannel);

        // TODO: verify aspects of the requests
    }

//    @Test
//    public void initiateHandshake_IncorrectAgentId() throws Exception {
//
//        // given an response with an invalid agent ID
//        byte[] response = TestUtil.createHandshakeResponse(UUID.randomUUID());
//        Mockito.when(dataChannel.recv()).thenReturn(response);
//
//        // when the handshake is initiated
//        try {
//            protocolHandler.handshake();
//            Assert.fail();
//        } catch (AgentVerificationException e) {
//            // good
//        }
//    }
//
//

    private void verifyChannelInteractions(int numBytes) throws IOException {
        verify(dataChannel, times(1)).send(Mockito.any());
        verify(dataChannel, times(1)).recv(numBytes);
        verifyNoMoreInteractions(dataChannel);
    }
}
