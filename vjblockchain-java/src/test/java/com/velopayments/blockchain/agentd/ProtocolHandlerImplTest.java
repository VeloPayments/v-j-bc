package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.mockito.Mockito;

import java.util.UUID;


public class ProtocolHandlerImplTest {

    ProtocolHandlerImpl protocolHandler;

    DataChannel dataChannel;

    UUID agentId;

    EncryptionPrivateKey entityPrivateKey;

    @Before
    public void setup() {

        dataChannel = Mockito.mock(DataChannel.class);

        agentId = UUID.randomUUID();

        entityPrivateKey = EncryptionKeyPair.generate().getPrivateKey();

        protocolHandler = new ProtocolHandlerImpl(dataChannel,
                agentId, EncryptionKeyPair.generate().getPublicKey(),
                UUID.randomUUID(), entityPrivateKey);
    }

    @Ignore // until HMAC sorted out
    @Test
    public void handshake() throws Exception {

        // given a valid response
        /*byte[] response = TestUtil.createHandshakeResponse(agentId);
        Mockito.when(dataChannel.recv()).thenReturn(response);

        // when the handshake is invoked
        protocolHandler.handshake();

        // then there should have been one round trip  (TODO: two round trips)
        verify(dataChannel, times(1)).send(Mockito.any());
        verify(dataChannel, times(1)).recv();
        verifyNoMoreInteractions(dataChannel);*/
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
//    private int stubChannel(byte[] responseBytes) throws IOException {
//        Mockito.when(dataChannel.recv()).thenReturn(responseBytes);
//
//        return responseBytes.length;
//    }

    /*private void verifyChannelInteractions(int numBytes) throws IOException {
        verify(dataChannel, times(1)).send(Mockito.any());
        verify(dataChannel, times(1)).recv();
        verifyNoMoreInteractions(dataChannel);
    }*/
}
