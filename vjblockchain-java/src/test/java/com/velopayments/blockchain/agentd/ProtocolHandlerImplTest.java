package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.crypt.HMAC;
import com.velopayments.blockchain.util.ByteUtil;
import com.velopayments.blockchain.util.UuidUtil;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

import java.io.IOException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.UUID;

import static org.mockito.Mockito.*;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

import static com.velopayments.blockchain.agentd.ProtocolHandlerImpl.*;

public class ProtocolHandlerImplTest {

    ProtocolHandlerImpl protocolHandler;

    DataChannel dataChannel;
    UUID agentId;
    UUID entityId;
    EncryptionPrivateKey entityPrivateKey;
    OuterEnvelopeReader outerEnvelopeReader;
    SecureRandom random;

    @Before
    public void setup() {

        dataChannel = mock(DataChannel.class);
        agentId = UUID.randomUUID();
        entityId = UUID.randomUUID();
        entityPrivateKey = EncryptionKeyPair.generate().getPrivateKey();
        outerEnvelopeReader = mock(OuterEnvelopeReader.class);
        random = mock(SecureRandom.class);

        protocolHandler = new ProtocolHandlerImpl(dataChannel, agentId,
                entityId, entityPrivateKey, outerEnvelopeReader, random);
    }

    @Test
    public void handshake_happyPath() throws Exception
    {
        stubDataChannel(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0, 1, 1, agentId,
                entityPrivateKey);

        // when the handshake is invoked
        protocolHandler.handshake();

        // then there should have been two round trips
        verify(dataChannel, times(2)).send(Mockito.any());
        // init and ack headers are 5 byte seach
        verify(dataChannel, times(2)).recv(5);
        // init response body
        verify(dataChannel, times(1)).recv(164);
        // ack HMAC
        verify(dataChannel, times(1)).recv(32);
        // ack response body
        verify(dataChannel, times(1)).recv(12);
        verifyNoMoreInteractions(dataChannel);

        // TODO: verify aspects of the requests
    }

    @Test(expected = InvalidPacketTypeException.class)
    public void initiateHandshakeResponse_InvalidPacketType() throws Exception
    {
        byte invalidPacketType = 0x01;
        stubDataChannel(invalidPacketType,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0, 1, 1,
                agentId, entityPrivateKey);

        protocolHandler.handshake();
    }

    @Test(expected = InvalidRequestIdException.class)
    public void initiateHandshakeResponse_InvalidRequestId() throws Exception
    {
        long badRequestId = 99;
        stubDataChannel(IPC_DATA_TYPE_DATA_PACKET,
                badRequestId,0, 1, 1,
                agentId, entityPrivateKey);


        protocolHandler.handshake();
    }

    @Test(expected = ErrorStatusException.class)
    public void initiateHandshakeResponse_UnsuccessfulStatus() throws Exception
    {
        int errorStatus = -1;
        stubDataChannel(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE, errorStatus, 1, 1,
                agentId, entityPrivateKey);

        protocolHandler.handshake();
    }

    @Test(expected = UnsupportedProtocolVersionException.class)
    public void initiateHandshakeResponse_InvalidProtocolVersion()
            throws Exception
    {
        int wrongProto = 2;
        stubDataChannel(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE, 0, wrongProto, 1,
                agentId, entityPrivateKey);

        protocolHandler.handshake();
    }

    @Test(expected = UnsupportedCryptoSuiteVersion.class)
    public void initiateHandshakeResponse_InvalidCryptoSuiteVersion()
            throws Exception
    {
        int wrongCrypto = 3;
        stubDataChannel(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE, 0, 1, wrongCrypto,
                agentId, entityPrivateKey);

        protocolHandler.handshake();
    }

    @Test(expected = AgentVerificationException.class)
    public void initiateHandshakeResponse_InvalidAgentId() throws Exception
    {
        UUID invalidAgentId = UUID.randomUUID();
        stubDataChannel(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0, 1, 1,
                invalidAgentId, entityPrivateKey);

        protocolHandler.handshake();
    }

    @Test(expected = MessageVerificationException.class)
    public void initiateHandshakeResponse_InvalidChallengeResponse()
            throws Exception
    {
        stubDataChannel(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0, 1, 1,
                agentId, EncryptionKeyPair.generate().getPrivateKey());

        protocolHandler.handshake();
    }


    /* HELPER METHODS AND UTILITIES BELOW THIS LINE */

    private void stubDataChannel(
            byte packetType, long requestId, int status, int protocolVersion,
            int cryptoSuiteVersion, UUID agentId,
            EncryptionPrivateKey entityPrivateKey)
        throws IOException
    {
        // stub the initiate handshake response header
        // and the ack response header
        byte[] ackResponseHeader = createAckHandshakeResponseHeader();
        when(dataChannel.recv(5))
                .thenReturn(
                        createInitiateHandshakeResponseHeader(packetType))
                .thenReturn(ackResponseHeader);


        // stub the initiate handshake response body
        ByteArrayPair bap = createInitiateHandshakeResponse(
                requestId, status, protocolVersion, cryptoSuiteVersion,
                agentId, entityPrivateKey);
        byte[] sharedSecret = bap.e1;
        byte[] initHandshakeResponse = bap.e2;
        when(dataChannel.recv(164)).thenReturn(initHandshakeResponse);


        // stub the ack response payload
        byte[] ackResponse = new byte[12];
        when(dataChannel.recv(12)).thenReturn(ackResponse);


        // stub the decryption for the ack header
        when(outerEnvelopeReader.decryptHeader(any(), any()))
                .thenReturn(12);

        // set up the HMAC in the ack response
        HMAC hmac = new HMAC(sharedSecret);
        byte[][] hmacParts = new byte[2][];
        hmacParts[0] = ackResponseHeader;
        hmacParts[1] = ackResponse;
        when(dataChannel.recv(32))
                .thenReturn(hmac.createHMACShort(hmacParts));

        // stub the decryption of the ack payload
        when(outerEnvelopeReader.decryptPayload(sharedSecret, ackResponse))
                .thenReturn(createAckDecryptedPayload());

    }


    private byte[] createInitiateHandshakeResponseHeader(byte packetType)
    {
        byte[] responseHeader = new byte[5];

        // response type
        responseHeader[0] = packetType;

        // response size
        System.arraycopy(ByteUtil.htonl(164), 0, responseHeader, 1, 4);

        return responseHeader;
    }

    /* | ---------------------------------------------- | ------------ | */
    /* | Handshake request response packet.                            | */
    /* | ---------------------------------------------- | ------------ | */
    /* | DATA                                           | SIZE         | */
    /* | ---------------------------------------------- | ------------ | */
    /* | UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE      |   4 bytes    | */
    /* | offset                                         |   4 bytes    | */
    /* | status                                         |   4 bytes    | */
    /* | record:                                        | 152 bytes    | */
    /* |    protocol_version                            |   4 bytes    | */
    /* |    crypto_suite                                |   4 bytes    | */
    /* |    agent_id                                    |  16 bytes    | */
    /* |    server public key                           |  32 bytes    | */
    /* |    server key nonce                            |  32 bytes    | */
    /* |    server challenge nonce                      |  32 bytes    | */
    /* |    server_cr_hmac                              |  32 bytes    | */
    /* | ---------------------------------------------- | ------------ | */

    class ByteArrayPair {
        byte[] e1;
        byte[] e2;
    }
    private ByteArrayPair createInitiateHandshakeResponse(
            long requestId, int status, int proto, int crypto, UUID agentdId,
            EncryptionPrivateKey entityPrivateKey)
    {
        SecureRandom sr = new SecureRandom();

        byte[] response = new byte[164];

        // request ID
        System.arraycopy(ByteUtil.htonl((int)requestId), 0, response, 0, 4);

        // status
        System.arraycopy(ByteUtil.htonl(status), 0, response, 8, 4);

        // protocol
        System.arraycopy(ByteUtil.htonl(proto), 0, response, 12, 4);

        // crypto version
        System.arraycopy(ByteUtil.htonl(crypto), 0, response, 16, 4);

        // agentd ID
        System.arraycopy(UuidUtil.getBytesFromUUID(agentdId), 0, response,
                20, 16);

        byte[] serverPublicKey = new byte[32];
        sr.nextBytes(serverPublicKey);
        System.arraycopy(serverPublicKey, 0, response, 36,32);

        byte[] serverKeyNonce = new byte[32];
        sr.nextBytes(serverKeyNonce);
        System.arraycopy(serverKeyNonce, 0, response, 68,32);

        byte[] serverChallengeNonce = new byte[32];
        sr.nextBytes(serverChallengeNonce);
        System.arraycopy(serverChallengeNonce, 0, response, 100,32);

        byte[] sharedSecret = ProtocolHandlerImpl.computeSharedSecret(entityPrivateKey,
                new EncryptionPublicKey(serverPublicKey),
                serverKeyNonce,
                new byte[32]); // client key nonce generated using a mocked SecureRandom

        HMAC hmac = new HMAC(sharedSecret);
        byte[][] hmacParts = new byte[2][];
        hmacParts[0] = Arrays.copyOfRange(response, 0, 132);
        hmacParts[1] = new byte[32];  // client challenge nonce
        byte[] computedHMAC = hmac.createHMACShort(hmacParts);
        System.arraycopy(computedHMAC, 0, response, 132,32);

        ByteArrayPair bap = new ByteArrayPair();
        bap.e1 = sharedSecret;
        bap.e2 = response;
        return bap;
    }

    private byte[] createAckHandshakeResponseHeader()
    {
        byte[] responseHeader = new byte[5];

        // response type
        responseHeader[0] = 0x30; // IPC_DATA_TYPE_AUTHED_PACKET

        // response size
        System.arraycopy(ByteUtil.htonl(12), 0, responseHeader, 1, 4);

        return responseHeader;
    }

    private byte[] createAckDecryptedPayload()
    {
        byte[] decryptedPayload = new byte[12];

        // set the request ID
        System.arraycopy(ByteUtil.htonl(1), 0, decryptedPayload, 0, 4);

        return decryptedPayload;
    }
}
