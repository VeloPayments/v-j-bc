package com.velopayments.blockchain.agentd;


import java.io.IOException;

/**
 * Socket based communication channel between client and remote agent.
 */
public interface RemoteAgentChannel {

    /**
     * Establishes the channel's connection to the remote endpoint.
     *
     * @throws IOException
     */
    void connect() throws IOException;

    /**
     * Closes this channel, making it unavailable for further use.
     *
     * @throws IOException
     */
    void close() throws IOException;

    /**
     * Sends raw bytes to the socket
     *
     * @param message     bytes to send to socket
     *
     * @throws IOException
     */
    void send(byte[] message) throws IOException;

    /**
     * Reads {@code n} bytes from the socket.
     *
     * @param n   the number of bytes to read
     *
     * @return    bytes read
     *
     * @throws IOException
     */
    byte[] recv(int n) throws IOException;

}
