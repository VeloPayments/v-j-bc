package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.util.ByteUtil;

import javax.net.SocketFactory;
import java.io.*;
import java.net.Socket;

public class SocketDataChannelImpl implements DataChannel {

    private static final byte IPC_DATA_TYPE_DATA_PACKET = 0x20;

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
    public void send(byte[] payload) throws IOException {

        byte[] message = new byte[payload.length + 5];

        // set the type
        message[0] = IPC_DATA_TYPE_DATA_PACKET;

        // set the message length
        byte[] length = ByteUtil.htonl(payload.length);
        System.arraycopy(length, 0, message, 1, 4);

        // copy the payload into the output buffer
        System.arraycopy(payload, 0, message, 5, payload.length);

        out.write(message);
    }

    @Override
    public byte[] recv() throws IOException {

        // verify the type is correct
        byte msgType = in.readByte();
        if (IPC_DATA_TYPE_DATA_PACKET != msgType) {
            throw new RuntimeException("Incorrect message type"); // TODO
        }

        // get the length of the payload
        byte[] msgLengthBytes = new byte[4];
        in.readFully(msgLengthBytes);
        int msgLength = (int)ByteUtil.ntohl(msgLengthBytes);

        // read the rest of the message
        byte[] payload = new byte[msgLength];
        in.readFully(payload);

        return payload;
    }

}
