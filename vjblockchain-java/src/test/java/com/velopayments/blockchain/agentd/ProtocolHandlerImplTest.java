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
//    @Test
//    public void getBlockById() throws IOException {
//
//        // given a valid request
//        long requestId = 5L;
//        UUID blockId = UUID.randomUUID();
//        byte[] certBytes = "some block bytes".getBytes();
//
//        // set up the response
//        int n = stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                ApiMethod.GET_BLOCK_BY_ID, requestId,0,
//                createBlockResponsePayload(blockId, certBytes)));
//
//        // when the API is invoked
//        Optional<Certificate> maybeCert = protocolHandler.getBlockById(blockId);
//
//        verifyChannelInteractions(n);
//
//        // and the returned value should be correct
//        assertThat(maybeCert.isPresent(), is(true));
//        assertThat(maybeCert.get().toByteArray(), is(certBytes));
//    }
//
//    @Test
//    public void getBlockById_doesNotExist() throws IOException {
//
//        // given a valid request
//        long requestId = 5L;
//        UUID blockId = UUID.randomUUID();
//
//        // set up the response
//        int n = stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                ApiMethod.GET_BLOCK_BY_ID, requestId,0,
//                createBlockResponsePayload(blockId, null)));
//
//        // when the API is invoked
//        Optional<Certificate> maybeCert = protocolHandler.getBlockById(blockId);
//
//        verifyChannelInteractions(n);
//
//        // and the returned value should be correct
//        assertThat(maybeCert.isPresent(), is(false));
//    }
//
//    @Test
//    public void getBlockIdByBlockHeight() throws IOException {
//
//        long requestId = 5L;
//        long blockHeight = 17L;
//        UUID blockId = UUID.randomUUID();
//
//        // set up the response
//        stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                ApiMethod.GET_BLOCK_ID_BY_BLOCK_HEIGHT, requestId,0,
//                UuidUtil.getBytesFromUUID(blockId)));
//
//        // when the API is invoked
//        Optional<UUID> apiResponse = protocolHandler.getBlockIdByBlockHeight(blockHeight);
//
//        // the returned value should be correct
//        assertThat(apiResponse.get(), is(blockId));
//    }
//
//    @Test
//    public void getLatestBlockId() throws IOException {
//
//        // given a valid request
//        long requestId = 5L;
//        UUID respUuid = UUID.randomUUID();
//
//        // set up the response
//        int n = stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                ApiMethod.GET_LATEST_BLOCK_ID, requestId,0,
//                UuidUtil.getBytesFromUUID(respUuid)));
//
//        // when the API is invoked
//        UUID apiResponse = protocolHandler.getLatestBlockId();
//
//        verifyChannelInteractions(n);
//
//        // and the returned value should be correct
//        assertThat(apiResponse, is(respUuid));
//    }
//
//    @Test
//    public void getTransactionById() throws IOException {
//        // given a valid request
//        long requestId = 15L;
//        UUID transactionId = UUID.randomUUID();
//        byte[] certBytes = "some transaction bytes".getBytes();
//
//        // set up the response
//        int n = stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                ApiMethod.GET_TXN_BY_ID, requestId,0,
//                createTransactionResponsePayload(transactionId, certBytes)));
//
//        // when the API is invoked
//        Optional<Certificate> maybeCert = protocolHandler.getTransactionById(transactionId);
//
//        verifyChannelInteractions(n);
//
//        // and the returned value should be correct
//        assertThat(maybeCert.isPresent(), is(true));
//        assertThat(maybeCert.get().toByteArray(), is(certBytes));
//    }
//
//    @Test
//    public void getTransactionById_doesNotExist() throws IOException {
//        // given a valid request
//        long requestId = 15L;
//        UUID transactionId = UUID.randomUUID();
//
//        // set up the response
//        int n = stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                ApiMethod.GET_TXN_BY_ID, requestId,0,
//                createTransactionResponsePayload(transactionId, null)));
//
//        // when the API is invoked
//        Optional<Certificate> maybeCert = protocolHandler.getTransactionById(transactionId);
//
//        verifyChannelInteractions(n);
//
//        // and the returned value should be correct
//        assertThat(maybeCert.isPresent(), is(false));
//    }
//
//    @Test
//    public void sendAndReceiveUUID() throws IOException {
//
//        // given a valid request
//        ApiMethod apiMethod = ApiMethod.GET_PREV_BLOCK_ID;
//        long requestId = 3L;
//        UUID reqUuid = UUID.randomUUID();
//        UUID respUuid = UUID.randomUUID();
//
//        // set up the response
//        int n = stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                apiMethod, requestId,0,
//                UuidUtil.getBytesFromUUID(respUuid)));
//
//        // when the API is invoked
//        Optional<UUID> apiResponse = protocolHandler.sendAndReceiveUUID(
//                apiMethod, reqUuid);
//
//        verifyChannelInteractions(n);
//
//        // and the returned value should be correct
//        assertThat(apiResponse.get(), is(respUuid));
//    }
//
//    @Test
//    public void submit() throws IOException {
//
//        // given a valid request
//        ApiMethod apiMethod = ApiMethod.SUBMIT;
//        long requestId = 25L;
//        UUID transactionId = UUID.randomUUID();
//        UUID artifactId = UUID.randomUUID();
//        Certificate certificate = CertificateBuilder
//                .createCertificateBuilder(CertificateType.TRANSACTION)
//                .addUUID(Field.CERTIFICATE_ID, transactionId)
//                .addUUID(Field.ARTIFACT_ID, artifactId)
//                .emit();
//
//        // set up the response
//        int n = stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                apiMethod, requestId,0,
//                new byte[0]));
//
//        // when the API is invoked
//        protocolHandler.submit(certificate);
//
//        verifyChannelInteractions(n);
//
//        // then no errors should be thrown
//    }
//
//    @Test(expected = OperationFailureException.class)
//    public void submitFailure() throws IOException {
//        // given a valid request
//        ApiMethod apiMethod = ApiMethod.SUBMIT;
//        long requestId = 25L;
//        UUID transactionId = UUID.randomUUID();
//        UUID artifactId = UUID.randomUUID();
//        Certificate certificate = CertificateBuilder
//                .createCertificateBuilder(CertificateType.TRANSACTION)
//                .addUUID(Field.CERTIFICATE_ID, transactionId)
//                .addUUID(Field.ARTIFACT_ID, artifactId)
//                .emit();
//
//        // set up the response
//        stubChannel(createResponse(entityPrivateKey.getRawBytes(),
//                apiMethod, requestId,1, // <-- failure
//                new byte[0]));
//
//        // when the API is invoked
//        protocolHandler.submit(certificate);
//    }
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
