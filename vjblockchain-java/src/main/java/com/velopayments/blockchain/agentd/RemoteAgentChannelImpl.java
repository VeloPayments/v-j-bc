package com.velopayments.blockchain.agentd;

import com.velopayments.blockchain.util.ByteUtil;

import javax.net.SocketFactory;
import java.io.*;
import java.net.Socket;
import java.util.Arrays;

public class RemoteAgentChannelImpl implements RemoteAgentChannel {

    private String host;
    private Integer port;
    private SocketFactory socketFactory;
    private Socket socket;

    private DataOutputStream out;
    private DataInputStream in;

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
        // TODO: temporary hack -- this probably belongs in the protocol handler
        byte[] data_packet = new byte[message.length + 5];
        data_packet[0] = 0x20; // IPC_DATA_PACKET

        System.arraycopy(ByteUtil.htonl(message.length), 0, data_packet, 1, 4);
        System.arraycopy(message, 0, data_packet, 5, message.length);

        out.write(data_packet);
    }

    @Override
    public byte[] recv(int n) throws IOException {
        byte[] response = new byte[n+5];  // TODO: temporary hack
        int r = in.read(response);
        if (r == -1) {
            throw new RuntimeException("InputStream at eof");
        }
        if (r != n+5) {
            throw new RuntimeException("Expected " + n+5 + " bytes but read " + r);
        }
        //in.readFully(response);
        //return response;

        return Arrays.copyOfRange(response, 5, n);
    }


}
