package com.velopayments.blockchain.agentd;


public interface RemoteAgent {


    void send(byte[] request);

    byte[] recv(int n);

}
