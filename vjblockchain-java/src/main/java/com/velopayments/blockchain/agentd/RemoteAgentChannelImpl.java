package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.client.RemoteAgentConfiguration;

import javax.net.SocketFactory;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class RemoteAgentChannelImpl implements RemoteAgentChannel {

    private RemoteAgentConfiguration config;
    private SocketFactory socketFactory;
    private Socket socket;

    public RemoteAgentChannelImpl(RemoteAgentConfiguration config, SocketFactory socketFactory) {
        this.config = config;
        this.socketFactory = socketFactory;
    }


    @Override
    public void connect() throws  IOException {
        this.socket = socketFactory.createSocket(config.getHost(), config.getPort());
    }

    @Override
    public void close() throws IOException {
        this.socket.close();
    }

    /**
     * Synchronously send raw bytes to the socket
     *
     * @param message     bytes to send to socket
     *
     * @throws IOException
     */
    @Override
    public void send(byte[] message) throws IOException {
        try (OutputStream out = socket.getOutputStream())
        {
            out.write(message);
        }
    }

    /**
     * Synchronously read <n> bytes from the socket.
     *
     * @param n   the number of bytes to read
     *
     * @return    bytes read
     *
     * @throws IOException
     */
    @Override
    public byte[] recv(int n) throws IOException {

        byte[] response = new byte[n];

        try (DataInputStream in = new DataInputStream(socket.getInputStream()))
        {
            in.readFully(response);
        }

        return response;
    }


}
