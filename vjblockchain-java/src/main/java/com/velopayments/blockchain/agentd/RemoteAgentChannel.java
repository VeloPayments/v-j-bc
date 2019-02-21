package com.velopayments.blockchain.agentd;


import java.io.IOException;

public interface RemoteAgentChannel {

    void connect() throws IOException;

    void close() throws IOException;

    void send(byte[] request) throws IOException;

    byte[] recv(int n) throws IOException;

}
