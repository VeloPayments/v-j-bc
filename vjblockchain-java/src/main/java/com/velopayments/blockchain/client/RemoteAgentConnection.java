package com.velopayments.blockchain.client;

import com.velopayments.blockchain.agentd.GetNextBlockId;
import com.velopayments.blockchain.agentd.Handshake;
import com.velopayments.blockchain.agentd.RemoteAgentChannel;
import com.velopayments.blockchain.agentd.RemoteAgentChannelImpl;
import com.velopayments.blockchain.cert.Certificate;

import javax.net.SocketFactory;
import java.io.IOException;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.CompletableFuture;

public class RemoteAgentConnection implements VelochainConnection {

    private RemoteAgentConfiguration config;
    private RemoteAgentChannel remoteAgentChannel;

    public RemoteAgentConnection(RemoteAgentConfiguration config,
                                 SocketFactory socketFactory) {
        this.config = config;
        this.remoteAgentChannel = new RemoteAgentChannelImpl(config, socketFactory);
    }

    @Override
    public void commitTransactions() throws IOException {

    }

    @Override
    public void cancelTransactions() throws IOException {

    }

    public void connect() throws IOException {
        remoteAgentChannel.connect();
        Handshake handshake = new Handshake(config, remoteAgentChannel);
        handshake.initiate();
        handshake.acknowledge();
    }

    @Override
    public void close() throws IOException {

        /* commit outstanding transactions. */
        commitTransactions();

        remoteAgentChannel.close();
    }

    @Override
    public CompletableFuture<TransactionStatus> submit(Certificate transaction) throws IOException {
        return null;
    }

    @Override
    public UUID getLatestBlockId() throws IOException {
        return null;
    }

    @Override
    public Optional<UUID> getNextBlockId(UUID blockId) throws IOException {
        GetNextBlockId getNextBlockId = new GetNextBlockId(remoteAgentChannel);
        // TODO: keys, requestId
        return getNextBlockId.getNextBlockId(null, null, 0L, blockId);
    }

    @Override
    public Optional<UUID> getPrevBlockId(UUID blockId) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<UUID> getTransactionBlockId(UUID txnId) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<Certificate> getBlockById(UUID blockId) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<UUID> getBlockIdByBlockHeight(long height) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<Certificate> getTransactionById(UUID txnId) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<UUID> getFirstTransactionIdForArtifactById(UUID artifactId) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<UUID> getLastTransactionIdForArtifactById(UUID artifactId) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<UUID> getLastBlockIdForArtifactById(UUID artifactId) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<UUID> getPreviousTransactionIdForTransactionById(UUID txnId) throws IOException {
        return Optional.empty();
    }

    @Override
    public Optional<UUID> getNextTransactionIdForTransactionById(UUID txnId) throws IOException {
        return Optional.empty();
    }
}
