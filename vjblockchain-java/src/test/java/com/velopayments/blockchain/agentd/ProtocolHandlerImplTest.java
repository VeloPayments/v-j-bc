package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.cert.*;
import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.EncryptionPrivateKey;
import com.velopayments.blockchain.crypt.EncryptionPublicKey;
import com.velopayments.blockchain.crypt.HMAC;
import com.velopayments.blockchain.util.ByteUtil;
import com.velopayments.blockchain.util.UuidUtil;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.assertEquals;
import org.mockito.Mockito;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.net.ConnectException;
import java.io.IOException;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.Optional;
import java.util.UUID;

import static org.mockito.Mockito.*;

import static com.velopayments.blockchain.agentd.ProtocolHandlerImpl.*;

public class ProtocolHandlerImplTest {

    ProtocolHandlerImpl protocolHandler;

    DataChannel dataChannel;
    UUID agentId;
    UUID entityId;
    EncryptionPrivateKey entityPrivateKey;
    OuterEnvelopeReader outerEnvelopeReader;
    OuterEnvelopeWriter outerEnvelopeWriter;
    SecureRandom random;

    byte[] clientKeyNonce;
    byte[] clientChallengeNonce;

    private final UUID DUMMY_CERTIFICATE_TYPE =
        UUID.fromString("1f55c307-2252-49a0-8b36-9aca10eca50c");
    private final UUID DUMMY_CERTIFICATE_ID =
        UUID.fromString("78776753-7b30-499b-9128-0f7b58dbdcaa");
    private final UUID DUMMY_ARTIFACT_ID =
        UUID.fromString("f113cac5-f112-49c8-ae66-208df9dbc4f1");
    private final UUID LATEST_BLOCK_UUID =
        UUID.fromString("59665c03-fa18-454c-864d-862b92c7035d");

    @Before
    public void setup() {

        dataChannel = mock(DataChannel.class);
        agentId = UUID.randomUUID();
        entityId = UUID.randomUUID();
        entityPrivateKey = EncryptionKeyPair.generate().getPrivateKey();
        outerEnvelopeReader = mock(OuterEnvelopeReader.class);
        outerEnvelopeWriter = mock(OuterEnvelopeWriter.class);
        random = mock(SecureRandom.class);

        protocolHandler = new ProtocolHandlerImpl(dataChannel, agentId,
                entityId, entityPrivateKey, outerEnvelopeReader,
                outerEnvelopeWriter, random);

        // predetermine the random values and program the rng
        SecureRandom sr = new SecureRandom();
        clientKeyNonce = new byte[32];
        sr.nextBytes(clientKeyNonce);

        clientChallengeNonce = new byte[32];
        sr.nextBytes(clientChallengeNonce);

        doAnswer(new Answer() {
            private int count = 0;
            @Override
            public Object answer(InvocationOnMock invocation)
            {
                byte[] target = invocation.getArgument(0);
                if (count++ == 0) {
                    System.arraycopy(
                            clientKeyNonce, 0, target, 0,
                            target.length);
                } else {
                    System.arraycopy(
                            clientChallengeNonce, 0, target, 0,
                            target.length);
                }
                return null;
            }
        }).when(random).nextBytes(any(byte[].class));;


    }

    @Test
    public void handshake_happyPath() throws Exception
    {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION, agentId,
                entityPrivateKey, null);

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
    }

    @Test
    public void initiateHandshakeRequest() throws Exception
    {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION, agentId,
                entityPrivateKey, null);

        protocolHandler.handshake();

        byte[] expectedRequest = new byte[101];

        /* | ---------------------------------------------- | ------------ | */
        /* | DATA                                           | SIZE         | */
        /* | ---------------------------------------------- | ------------ | */
        /* | UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE      |  4 bytes     | */
        /* | offset                                         |  4 bytes     | */
        /* | record:                                        | 88 bytes     | */
        /* |    protocol_version                            |  4 bytes     | */
        /* |    crypto_suite                                |  4 bytes     | */
        /* |    entity_id                                   | 16 bytes     | */
        /* |    client key nonce                            | 32 bytes     | */
        /* |    client challenge nonce                      | 32 bytes     | */
        /* | ---------------------------------------------- | ------------ | */


        expectedRequest[0] = IPC_DATA_TYPE_DATA_PACKET;
        System.arraycopy(ByteUtil.htonl(96), 0, expectedRequest, 1, 4);
        System.arraycopy(
                ByteUtil.htonl((int)UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE),
                0, expectedRequest, 5, 4);
        System.arraycopy(ByteUtil.htonl((int)PROTOCOL_VERSION), 0,
                expectedRequest, 13, 4);
        System.arraycopy(ByteUtil.htonl((int)CRYPTO_SUITE_VERSION), 0,
                expectedRequest, 17, 4);
        System.arraycopy(UuidUtil.getBytesFromUUID(entityId), 0,
                expectedRequest, 21, 16);
        System.arraycopy(clientKeyNonce, 0, expectedRequest, 37, 32);
        System.arraycopy(clientChallengeNonce, 0, expectedRequest, 69, 32);

        verify(dataChannel).send(expectedRequest);
    }

    @Test(expected = InvalidPacketTypeException.class)
    public void initiateHandshakeResponse_InvalidPacketType() throws Exception
    {
        byte invalidPacketType = 0x01;
        stubDataChannelForHandshake(invalidPacketType,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                agentId, entityPrivateKey, null);

        protocolHandler.handshake();
    }

    @Test(expected = InvalidRequestIdException.class)
    public void initiateHandshakeResponse_InvalidRequestId() throws Exception
    {
        long badRequestId = 99;
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                badRequestId,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                agentId, entityPrivateKey, null);


        protocolHandler.handshake();
    }

    @Test(expected = ErrorStatusException.class)
    public void initiateHandshakeResponse_UnsuccessfulStatus() throws Exception
    {
        int errorStatus = -1;
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE, errorStatus,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                agentId, entityPrivateKey, null);

        protocolHandler.handshake();
    }

    @Test(expected = UnsupportedProtocolVersionException.class)
    public void initiateHandshakeResponse_InvalidProtocolVersion()
            throws Exception
    {
        int wrongProto = 2;
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE, 0,
                wrongProto, CRYPTO_SUITE_VERSION,
                agentId, entityPrivateKey, null);

        protocolHandler.handshake();
    }

    @Test(expected = UnsupportedCryptoSuiteVersion.class)
    public void initiateHandshakeResponse_InvalidCryptoSuiteVersion()
            throws Exception
    {
        int wrongCrypto = 3;
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE, 0,
                PROTOCOL_VERSION, wrongCrypto,
                agentId, entityPrivateKey, null);

        protocolHandler.handshake();
    }

    @Test(expected = AgentVerificationException.class)
    public void initiateHandshakeResponse_InvalidAgentId() throws Exception
    {
        UUID invalidAgentId = UUID.randomUUID();
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                invalidAgentId, entityPrivateKey, null);

        protocolHandler.handshake();
    }

    @Test(expected = MessageVerificationException.class)
    public void initiateHandshakeResponse_InvalidChallengeResponse()
            throws Exception
    {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                agentId, EncryptionKeyPair.generate().getPrivateKey(), null);

        protocolHandler.handshake();
    }

    @Test
    public void ackRequest() throws Exception
    {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                agentId, entityPrivateKey, null);

        // validate what is sent is what is produced by the writer
        byte[] expectedRequest = new byte[] { 0, 1, 2, 3, 4 };

        when(outerEnvelopeWriter.encryptPayload(any(), any()))
                .thenReturn(expectedRequest);

        protocolHandler.handshake();

        verify(dataChannel).send(expectedRequest);
    }

    @Test(expected = InvalidPayloadSizeException.class)
    public void ackHandshakeResponse_InvalidPayloadSize() throws Exception
    {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                agentId, entityPrivateKey, null);

        int invalidPayloadSize = 999;
        when(outerEnvelopeReader.decryptHeader(any(), any()))
                .thenReturn(invalidPayloadSize);
        protocolHandler.handshake();
    }

    @Test(expected = InvalidRequestIdException.class)
    public void ackHandshakeResponse_InvalidRequestId() throws Exception
    {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                agentId, entityPrivateKey, null);

        int badAckRequestId = 999;
        when(outerEnvelopeReader.decryptPayload(any(), any(), any()))
                .thenReturn(createAckDecryptedPayload(badAckRequestId, 0));

        protocolHandler.handshake();
    }

    @Test(expected = ErrorStatusException.class)
    public void ackHandshakeResponse_UnsuccessfulStatus() throws Exception
    {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION,
                agentId, entityPrivateKey, null);

        when(outerEnvelopeReader.decryptPayload(any(), any(), any()))
                .thenReturn(createAckDecryptedPayload(
                        UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_ACKNOWLEDGE, -1));

        protocolHandler.handshake();
    }

    @Test(expected = IOException.class)
    public void submit_missingTransactionId() throws Exception {
        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(DUMMY_CERTIFICATE_TYPE);
        builder.addUUID(Field.ARTIFACT_ID, DUMMY_ARTIFACT_ID);

        Certificate dummyCertificate = builder.emit();

        protocolHandler.submit(dummyCertificate);
    }

    @Test(expected = IOException.class)
    public void submit_missingArtifactId() throws Exception {
        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(DUMMY_CERTIFICATE_TYPE);
        builder.addUUID(Field.CERTIFICATE_ID, DUMMY_CERTIFICATE_ID);

        Certificate dummyCertificate = builder.emit();

        protocolHandler.submit(dummyCertificate);
    }

    @Test
    public void submit_happyPath() throws Exception {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION, agentId,
                entityPrivateKey, null);

        // when the handshake is invoked
        protocolHandler.handshake();

        byte[] sharedSecret ={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 };
        stubDataChannel((int)UNAUTH_PROTOCOL_REQ_ID_TRANSACTION_SUBMIT, 0, 0, null,
                        sharedSecret, null);

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(DUMMY_CERTIFICATE_TYPE);
        builder.addUUID(Field.CERTIFICATE_ID, DUMMY_CERTIFICATE_ID);
        builder.addUUID(Field.ARTIFACT_ID, DUMMY_ARTIFACT_ID);

        protocolHandler.submit(builder.emit());

        // then there should have been three round trips
        verify(dataChannel, times(3)).send(Mockito.any());
        // headers are 5 bytes each
        verify(dataChannel, times(3)).recv(5);
        // init response body
        verify(dataChannel, times(1)).recv(164);
        // ack and submit HMAC
        verify(dataChannel, times(2)).recv(32);
        // ack and submit response bodies
        verify(dataChannel, times(2)).recv(12);
        verifyNoMoreInteractions(dataChannel);

    }

    @Test(expected = ConnectException.class)
    public void submit_noConnection() throws Exception {

        CertificateBuilder builder =
            CertificateBuilder.createCertificateBuilder(DUMMY_CERTIFICATE_TYPE);
        builder.addUUID(Field.CERTIFICATE_ID, DUMMY_CERTIFICATE_ID);
        builder.addUUID(Field.ARTIFACT_ID, DUMMY_ARTIFACT_ID);

        protocolHandler.submit(builder.emit());
    }

    @Test
    public void getLatestBlockId_happyPath() throws Exception {
        stubDataChannelForHandshake(IPC_DATA_TYPE_DATA_PACKET,
                UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_INITIATE,0,
                PROTOCOL_VERSION, CRYPTO_SUITE_VERSION, agentId,
                entityPrivateKey, null);

        // when the handshake is invoked
        protocolHandler.handshake();

        byte[] sharedSecret ={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 };
        stubDataChannelWithMacPayload(
            (int)UNAUTH_PROTOCOL_REQ_ID_LATEST_BLOCK_ID_GET, 0, 0,
            UuidUtil.getBytesFromUUID(LATEST_BLOCK_UUID), sharedSecret, null);

        UUID id = protocolHandler.getLatestBlockId();

        // the returned UUID matches the payload.
        assertEquals(id, LATEST_BLOCK_UUID);

        // then there should have been three round trips
        verify(dataChannel, times(3)).send(Mockito.any());
        // headers are 5 bytes each
        verify(dataChannel, times(3)).recv(5);
        // init response body
        verify(dataChannel, times(1)).recv(164);
        // ack HMAC
        verify(dataChannel, times(1)).recv(32);
        // ack response body
        verify(dataChannel, times(1)).recv(12);
        // latest id response body
        verify(dataChannel, times(1)).recv(12 + 16 + 32);
        verifyNoMoreInteractions(dataChannel);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getLatestBlockId_noConnection() throws Exception {
        protocolHandler.getLatestBlockId();
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getBlockById_noConnection() throws Exception {
        UUID EXPECTED_BLOCK_ID = 
            UUID.fromString("1649a0e4-5360-48a5-9915-c1ceac7bc88d");

        Optional<Certificate> blockCert =
            protocolHandler.getBlockById(EXPECTED_BLOCK_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getNextBlockId_noConnection() throws Exception {
        UUID EXPECTED_BLOCK_ID = 
            UUID.fromString("2ea35d68-ae58-41bf-95fb-26b284ec6825");

        Optional<UUID> nextId =
            protocolHandler.getNextBlockId(EXPECTED_BLOCK_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getPrevBlockId_noConnection() throws Exception {
        UUID EXPECTED_BLOCK_ID = 
            UUID.fromString("29ed5398-6fbd-48d4-910d-cc65c2e265a6");

        Optional<UUID> prevId =
            protocolHandler.getPrevBlockId(EXPECTED_BLOCK_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getTransactionById_noConnection() throws Exception {
        UUID EXPECTED_TXN_ID = 
            UUID.fromString("a2643f7a-9eca-4f07-9732-f8fe3c86d011");

        Optional<Certificate> txnCert =
            protocolHandler.getTransactionById(EXPECTED_TXN_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getTransactionNextId_noConnection() throws Exception {
        UUID EXPECTED_TXN_ID = 
            UUID.fromString("5d91a253-9367-4de1-94d2-7ea406511b6c");

        Optional<UUID> nextId =
            protocolHandler.getTransactionNextId(EXPECTED_TXN_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getTransactionPreviousId_noConnection() throws Exception {
        UUID EXPECTED_TXN_ID = 
            UUID.fromString("eb80d8f5-079c-4969-af3c-8689667ba415");

        Optional<UUID> prevId =
            protocolHandler.getTransactionPreviousId(EXPECTED_TXN_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getTransactionBlockId_noConnection() throws Exception {
        UUID EXPECTED_TXN_ID = 
            UUID.fromString("1b061f2b-668f-44e6-ab93-8d949b66dea0");

        Optional<UUID> blockId =
            protocolHandler.getTransactionBlockId(EXPECTED_TXN_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getArtifactFirstTxnId_noConnection() throws Exception {
        UUID EXPECTED_ARTIFACT_ID = 
            UUID.fromString("71f9ad6c-726f-4db7-addb-615a8c98b0b4");

        Optional<UUID> txnId =
            protocolHandler.getArtifactFirstTxnId(EXPECTED_ARTIFACT_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getArtifactLastTxnId_noConnection() throws Exception {
        UUID EXPECTED_ARTIFACT_ID = 
            UUID.fromString("b2b6ba4e-c438-4819-b917-0ca8d314a73d");

        Optional<UUID> txnId =
            protocolHandler.getArtifactLastTxnId(EXPECTED_ARTIFACT_ID);
    }

    /* Throw a ConnectException if not connected. */
    @Test(expected = ConnectException.class)
    public void getBlockIdByBlockHeight_noConnection() throws Exception {
        long EXPECTED_HEIGHT = 117;

        Optional<UUID> blockId =
            protocolHandler.getBlockIdByBlockHeight(EXPECTED_HEIGHT);
    }

    /* HELPER METHODS AND UTILITIES BELOW THIS LINE */

    private void stubDataChannelForHandshake(
            byte packetType, long requestId, int status, long protocolVersion,
            long cryptoSuiteVersion, UUID agentId,
            EncryptionPrivateKey entityPrivateKey, byte[] hmacOverride)
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
        byte[] ackResponseTail = new byte[12];
        when(dataChannel.recv(12)).thenReturn(ackResponseTail);


        // stub the decryption for the ack header
        when(outerEnvelopeReader.decryptHeader(any(), any()))
                .thenReturn(12);

        // set up the HMAC in the ack response
        byte[] hmacVal = null;

        if (null == hmacOverride) {
            HMAC hmac = new HMAC(sharedSecret);
            byte[][] hmacParts = new byte[2][];
            hmacParts[0] = ackResponseHeader;
            hmacParts[1] = ackResponseTail;
            hmacVal = hmac.createHMACShort(hmacParts);
        } else {
            hmacVal = hmacOverride;
        }

        when(dataChannel.recv(32))
            .thenReturn(hmacVal);

        byte[] ackResponse = new byte[hmacVal.length + ackResponseTail.length];
        System.arraycopy(hmacVal, 0, ackResponse, 0, hmacVal.length);
        System.arraycopy(
            ackResponseTail, 0, ackResponse, hmacVal.length, ackResponseTail.length);

        // stub the decryption of the ack payload
        when(outerEnvelopeReader.decryptPayload(sharedSecret, ackResponseHeader,
                                                ackResponse))
            .thenReturn(
                    createAckDecryptedPayload(
                            UNAUTH_PROTOCOL_REQ_ID_HANDSHAKE_ACKNOWLEDGE, 0));

    }

    private void stubDataChannel(
            int requestId, int status, int offset, byte[] payload,
            byte[] sharedSecret, byte[] hmacOverride)
        throws IOException
    {
        int payloadLength = 0;
        if (null != payload)
            payloadLength = payload.length;

        // return the response header
        byte[] authedResponseHeader =
            createAuthedResponseHeader(sharedSecret, payloadLength + 12);
        when(dataChannel.recv(5))
                .thenReturn(authedResponseHeader);

        // build the payload
        byte[] authedResponsePayload =
            createAuthedResponsePayload(
                sharedSecret, requestId, status, offset, payload);
        when(dataChannel.recv(authedResponsePayload.length))
            .thenReturn(authedResponsePayload);

        // compute the MAC.
        byte[] MAC = null;
        if (null == hmacOverride) {
            MAC = createMAC(sharedSecret, authedResponseHeader,
                            authedResponsePayload);
        } else {
            MAC = hmacOverride;
        }

        // compute the combined MAC and payload.
        byte[] combinedMacPayload =
            new byte[MAC.length + authedResponsePayload.length];
        System.arraycopy(MAC, 0, combinedMacPayload, 0, MAC.length);
        System.arraycopy(
            authedResponsePayload, 0, combinedMacPayload, MAC.length,
            authedResponsePayload.length);

        // return the MAC
        when(dataChannel.recv(32))
            .thenReturn(MAC);

        // stub the decryption for the ack header
        when(outerEnvelopeReader.decryptHeader(any(), any()))
                .thenReturn(12);

        // stub the decryption of the payload
        when(outerEnvelopeReader.decryptPayload(
                    any(), eq(authedResponseHeader), eq(combinedMacPayload)))
            .thenReturn(
                    createUnencryptedPayload(
                        requestId, status, offset, payload));

    }

    private void stubDataChannelWithMacPayload(
            int requestId, int status, int offset, byte[] payload,
            byte[] sharedSecret, byte[] hmacOverride)
        throws IOException
    {
        int payloadLength = 0;
        if (null != payload)
            payloadLength = payload.length;

        // return the response header
        byte[] authedResponseHeader =
            createAuthedResponseHeader(sharedSecret, payloadLength + 12);
        when(dataChannel.recv(5))
                .thenReturn(authedResponseHeader);

        // build the payload
        byte[] authedResponsePayload =
            createAuthedResponsePayload(
                sharedSecret, requestId, status, offset, payload);

        // compute the MAC.
        byte[] MAC = null;
        if (null == hmacOverride) {
            MAC = createMAC(sharedSecret, authedResponseHeader,
                            authedResponsePayload);
        } else {
            MAC = hmacOverride;
        }

        // compute the combined MAC and payload.
        byte[] combinedMacPayload =
            new byte[MAC.length + authedResponsePayload.length];
        System.arraycopy(MAC, 0, combinedMacPayload, 0, MAC.length);
        System.arraycopy(
            authedResponsePayload, 0, combinedMacPayload, MAC.length,
            authedResponsePayload.length);

        // return the combined MAC payload
        when(dataChannel.recv(12 + payload.length + 32))
            .thenReturn(combinedMacPayload);

        // stub the decryption for the ack header
        when(outerEnvelopeReader.decryptHeader(any(), any()))
                .thenReturn(12 + payload.length);

        // stub the decryption of the payload
        when(outerEnvelopeReader.decryptPayload(
                    any(), eq(authedResponseHeader), eq(combinedMacPayload)))
            .thenReturn(
                    createUnencryptedPayload(
                        requestId, status, offset, payload));

    }

    private byte[] createUnencryptedPayload(
                int requestId, int status, int offset, byte[] payload) {

        // compute payload length
        int payloadLength = 0;
        if (null != payload) {
            payloadLength = payload.length;
        }

        //create unencrypted payload.
        byte[] pl = new byte[12 + payloadLength];
        System.arraycopy(ByteUtil.htonl(requestId), 0, pl, 0, 4);
        System.arraycopy(ByteUtil.htonl(status), 0, pl, 4, 4);
        System.arraycopy(ByteUtil.htonl(offset), 0, pl, 8, 4);
        if (null != payload) {
            System.arraycopy(payload, 0, pl, 12, payload.length);
        }

        return pl;
    }

    private byte[] createUnencryptedHeader(int length) {
        byte[] hdr = new byte[5];
        hdr[0] = 0x30; // IPC_DATA_TYPE_AUTHED_PACKET

        // response size
        System.arraycopy(ByteUtil.htonl(length), 0, hdr, 1, 4);

        return hdr;
    }

    private byte[] createMAC(byte[] sharedSecret, byte[] authedResponseHeader,
                             byte[] authedResponsePayload) {

        return new byte[32];
    }

    private byte[] createAuthedResponseHeader(byte[] sharedSecret, int length) {
        return createUnencryptedHeader(length);
    }

    private byte[] createAuthedResponsePayload(
                byte[] sharedSecret, int requestId, int status, int offset,
                byte[] payload) {

        return createUnencryptedPayload(requestId, status, offset, payload);
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
            long requestId, int status, long proto, long crypto, UUID agentdId,
            EncryptionPrivateKey entityPrivateKey)
    {
        SecureRandom sr = new SecureRandom();

        byte[] response = new byte[164];

        // request ID
        System.arraycopy(ByteUtil.htonl((int)requestId), 0, response, 0, 4);

        // status
        System.arraycopy(ByteUtil.htonl(status), 0, response, 8, 4);

        // protocol
        System.arraycopy(ByteUtil.htonl((int)proto), 0, response, 12, 4);

        // crypto version
        System.arraycopy(ByteUtil.htonl((int)crypto), 0, response, 16, 4);

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
                clientKeyNonce);

        HMAC hmac = new HMAC(sharedSecret);
        byte[][] hmacParts = new byte[2][];
        hmacParts[0] = Arrays.copyOfRange(response, 0, 132);
        hmacParts[1] = clientChallengeNonce;
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

    private byte[] createAckDecryptedPayload(long requestId, int status)
    {
        byte[] decryptedPayload = new byte[12];

        // set the request ID
        System.arraycopy(ByteUtil.htonl((int)requestId), 0, decryptedPayload, 0, 4);

        // set the return status
        System.arraycopy(ByteUtil.htonl(status), 0, decryptedPayload, 4, 4);

        return decryptedPayload;
    }
}
