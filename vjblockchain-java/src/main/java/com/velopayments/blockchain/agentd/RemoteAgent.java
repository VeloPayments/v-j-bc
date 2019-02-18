package com.velopayments.blockchain.agentd;

import java.io.IOException;

public interface RemoteAgent {


    void send(byte[] request);

    byte[] recv(int n);

}
