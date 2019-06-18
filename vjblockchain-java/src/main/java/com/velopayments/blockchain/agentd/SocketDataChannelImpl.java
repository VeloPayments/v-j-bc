package com.velopayments.blockchain.agentd;

import javax.net.SocketFactory;
import java.io.*;
import java.net.Socket;

public class SocketDataChannelImpl implements DataChannel {

    private String host;
    private Integer port;
    private SocketFactory socketFactory;

    private Socket socket;
    private DataOutputStream out;
    private DataInputStream in;

    /**
     * Constructor method to create a disconnected channel.
     *
     * @param host          the host to establish a remote connection to
     * @param port          the port number to establish a remote connection to
     * @param socketFactory a factory capable of creating a socket
     *
     * @throws IOException
     */
    public SocketDataChannelImpl(String host, Integer port,
                                 SocketFactory socketFactory) {
        this.host = host;
        this.port = port;
        this.socketFactory = socketFactory;
    }


    @Override
    public void connect() throws  IOException {
        socket = socketFactory.createSocket(host, port);
        out = new DataOutputStream(socket.getOutputStream());
        in = new DataInputStream(socket.getInputStream());
    }

    @Override
    public void close() throws IOException {
        in.close();
        out.close();
        socket.close();
    }

    @Override
    public void send(byte[] message) throws IOException {
        out.write(message);
    }


    @Override
    public byte[] recv(int nBytes) throws IOException {
        byte[] receivedBytes = new byte[nBytes];

        in.readFully(receivedBytes);

        return receivedBytes;
    }

}
