package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.crypt.EncryptionKeyPair;
import com.velopayments.blockchain.crypt.GenericStreamCipher;
import com.velopayments.blockchain.util.UuidUtil;
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

public class GetNextBlockIdTest {

    GetNextBlockId getNextBlockId;

    RemoteAgentChannel remoteAgentChannel;

    EncryptionKeyPair encryptionKeyPair;

    @Before
    public void setup() {

        remoteAgentChannel = Mockito.mock(RemoteAgentChannel.class);

        encryptionKeyPair = EncryptionKeyPair.generate();

        getNextBlockId = new GetNextBlockId(remoteAgentChannel);
    }

    @Test
    public void getNextBlockId() throws IOException {

        // given a valid request
        long requestId = 3L;
        UUID blockId = UUID.randomUUID();
        UUID nextBlockId = UUID.randomUUID();

        // set up the response
        Mockito.when(remoteAgentChannel.recv(GetNextBlockId.RESPONSE_SIZE))
                .thenReturn(createAgentdResponse(encryptionKeyPair.getPrivateKey().getRawBytes(),
                        ApiMethod.GET_NEXT_BLOCK_ID,requestId,0,
                        UuidUtil.getBytesFromUUID(nextBlockId)));

        // when the API is invoked
        Optional<UUID> apiResponse = getNextBlockId.getNextBlockId(
                encryptionKeyPair.getPublicKey(),
                encryptionKeyPair.getPrivateKey(),
                requestId, blockId);


        // then there should have been one round trip
        verify(remoteAgentChannel, times(1)).send(Mockito.any());
        verify(remoteAgentChannel, times(1)).recv(GetNextBlockId.RESPONSE_SIZE);
        verifyNoMoreInteractions(remoteAgentChannel);

        // and the returned value should be correct
        assertThat(apiResponse.get(), is(nextBlockId));
    }
}
