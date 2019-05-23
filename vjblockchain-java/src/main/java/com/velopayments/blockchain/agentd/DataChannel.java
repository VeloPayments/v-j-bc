package com.velopayments.blockchain.agentd;


import java.io.IOException;

/**
 * Class representing a data channel between client and server
 */
public interface DataChannel {

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
     * Sends a message through the channel
     *
     * @param message     bytes to send
     *
     * @throws IOException
     */
    void send(byte[] message) throws IOException;

    /**
     * Read a message from the channel
     *
     * @return    bytes read
     *
     * @throws IOException
     */
    byte[] recv() throws IOException;

}
