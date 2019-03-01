package com.velopayments.blockchain.agentd;

import javax.net.SocketFactory;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class RemoteAgentChannelImpl implements RemoteAgentChannel {

    private String host;
    private Integer port;
    private SocketFactory socketFactory;
    private Socket socket;

    /**
     * Constructor method to create an unconnected channel.
     *
     * @param host          the host to establish a remote connection to
     * @param port          the port number to establish a remote connection to
     * @param socketFactory a factory capable of creating a socket
     * @throws IOException
     */
    public RemoteAgentChannelImpl(String host, Integer port,
                                  SocketFactory socketFactory) {
        this.host = host;
        this.port = port;
        this.socketFactory = socketFactory;
    }


    @Override
    public void connect() throws  IOException {
        this.socket = socketFactory.createSocket(host, port);
    }

    @Override
    public void close() throws IOException {
        this.socket.close();
    }

    @Override
    public void send(byte[] message) throws IOException {
        try (OutputStream out = socket.getOutputStream())
        {
            out.write(message);
        }
    }

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
