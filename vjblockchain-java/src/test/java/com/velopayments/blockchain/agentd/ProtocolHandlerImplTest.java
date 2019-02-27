package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.cert.Certificate;
import com.velopayments.blockchain.cert.CertificateBuilder;
import com.velopayments.blockchain.cert.CertificateType;
import com.velopayments.blockchain.cert.Field;
import com.velopayments.blockchain.client.RemoteAgentConfiguration;
import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.util.UuidUtil;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

import java.io.IOException;
import java.util.Arrays;
import java.util.Optional;
import java.util.UUID;

import static org.mockito.Mockito.*;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;
import static com.velopayments.blockchain.agentd.TestUtil.createAgentdResponse;
import static com.velopayments.blockchain.agentd.TestUtil.createBlockResponsePayload;
import static com.velopayments.blockchain.agentd.TestUtil.createTransactionResponsePayload;
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
    public void getBlockById() throws IOException {

        // given a valid request
        long requestId = 5L;
        UUID blockId = UUID.randomUUID();
        byte[] certBytes = "some block bytes".getBytes();

        // set up the response
        int n = stubChannel(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                ApiMethod.GET_BLOCK_BY_ID, requestId,0,
                createBlockResponsePayload(blockId, certBytes)));

        // when the API is invoked
        Optional<Certificate> maybeCert = protocolHandler.getBlockById(blockId);

        verifyChannelInteractions(n);

        // and the returned value should be correct
        assertThat(maybeCert.isPresent(), is(true));
        assertThat(maybeCert.get().toByteArray(), is(certBytes));
    }

    @Test
    public void getBlockById_doesNotExist() throws IOException {

        // given a valid request
        long requestId = 5L;
        UUID blockId = UUID.randomUUID();

        // set up the response
        int n = stubChannel(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                ApiMethod.GET_BLOCK_BY_ID, requestId,0,
                createBlockResponsePayload(blockId, null)));

        // when the API is invoked
        Optional<Certificate> maybeCert = protocolHandler.getBlockById(blockId);

        verifyChannelInteractions(n);

        // and the returned value should be correct
        assertThat(maybeCert.isPresent(), is(false));
    }

    @Test
    public void getBlockIdByBlockHeight() throws IOException {

        long requestId = 5L;
        long blockHeight = 17L;
        UUID blockId = UUID.randomUUID();

        // set up the response
        stubChannel(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                ApiMethod.GET_BLOCK_ID_BY_BLOCK_HEIGHT, requestId,0,
                UuidUtil.getBytesFromUUID(blockId)));

        // when the API is invoked
        Optional<UUID> apiResponse = protocolHandler.getBlockIdByBlockHeight(blockHeight);

        // the returned value should be correct
        assertThat(apiResponse.get(), is(blockId));
    }

    @Test
    public void getLatestBlockId() throws IOException {

        // given a valid request
        long requestId = 5L;
        UUID respUuid = UUID.randomUUID();

        // set up the response
        int n = stubChannel(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                ApiMethod.GET_LATEST_BLOCK_ID, requestId,0,
                UuidUtil.getBytesFromUUID(respUuid)));

        // when the API is invoked
        UUID apiResponse = protocolHandler.getLatestBlockId();

        verifyChannelInteractions(n);

        // and the returned value should be correct
        assertThat(apiResponse, is(respUuid));
    }

    @Test
    public void getTransactionById() throws IOException {
        // given a valid request
        long requestId = 15L;
        UUID transactionId = UUID.randomUUID();
        byte[] certBytes = "some transaction bytes".getBytes();

        // set up the response
        int n = stubChannel(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                ApiMethod.GET_TXN_BY_ID, requestId,0,
                createTransactionResponsePayload(transactionId, certBytes)));

        // when the API is invoked
        Optional<Certificate> maybeCert = protocolHandler.getTransactionById(transactionId);

        verifyChannelInteractions(n);

        // and the returned value should be correct
        assertThat(maybeCert.isPresent(), is(true));
        assertThat(maybeCert.get().toByteArray(), is(certBytes));
    }

    @Test
    public void getTransactionById_doesNotExist() throws IOException {
        // given a valid request
        long requestId = 15L;
        UUID transactionId = UUID.randomUUID();

        // set up the response
        int n = stubChannel(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                ApiMethod.GET_TXN_BY_ID, requestId,0,
                createTransactionResponsePayload(transactionId, null)));

        // when the API is invoked
        Optional<Certificate> maybeCert = protocolHandler.getTransactionById(transactionId);

        verifyChannelInteractions(n);

        // and the returned value should be correct
        assertThat(maybeCert.isPresent(), is(false));
    }

    @Test
    public void sendAndReceiveUUID() throws IOException {

        // given a valid request
        ApiMethod apiMethod = ApiMethod.GET_PREV_BLOCK_ID;
        long requestId = 3L;
        UUID reqUuid = UUID.randomUUID();
        UUID respUuid = UUID.randomUUID();

        // set up the response
        int n = stubChannel(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                apiMethod, requestId,0,
                UuidUtil.getBytesFromUUID(respUuid)));

        // when the API is invoked
        Optional<UUID> apiResponse = protocolHandler.sendAndReceiveUUID(
                apiMethod, reqUuid);

        verifyChannelInteractions(n);

        // and the returned value should be correct
        assertThat(apiResponse.get(), is(respUuid));
    }

    @Test
    public void submit() throws IOException {

        // given a valid request
        ApiMethod apiMethod = ApiMethod.SUBMIT;
        long requestId = 25L;
        UUID transactionId = UUID.randomUUID();
        UUID artifactId = UUID.randomUUID();
        Certificate certificate = CertificateBuilder
                .createCertificateBuilder(CertificateType.TRANSACTION)
                .addUUID(Field.CERTIFICATE_ID, transactionId)
                .addUUID(Field.ARTIFACT_ID, artifactId)
                .emit();

        // set up the response
        int n = stubChannel(createAgentdResponse(config.getEntityPrivateKey().getRawBytes(),
                apiMethod, requestId,0,
                new byte[0]));

        // when the API is invoked
        protocolHandler.submit(certificate);

        verifyChannelInteractions(n);

        // then no errors should be thrown
    }

    private int stubChannel(byte[] responseBytes) throws IOException {
        Mockito.when(remoteAgentChannel.recv(5))
                .thenReturn(Arrays.copyOfRange(responseBytes, 0, 5));

        Mockito.when(remoteAgentChannel.recv(responseBytes.length - 5))
                .thenReturn(Arrays.copyOfRange(responseBytes, 5, responseBytes.length));

        return responseBytes.length;
    }

    private void verifyChannelInteractions(int numBytes) throws IOException {
        verify(remoteAgentChannel, times(1)).send(Mockito.any());
        verify(remoteAgentChannel, times(1)).recv(5);
        verify(remoteAgentChannel, times(1)).recv(numBytes - 5);
        verifyNoMoreInteractions(remoteAgentChannel);
    }
}
