package com.velopayments.blockchain.agentd;

import javax.net.SocketFactory;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class RemoteAgentImpl implements RemoteAgent {

    private RemoteAgentConfiguration config;
    private SocketFactory socketFactory;

    public RemoteAgentImpl(RemoteAgentConfiguration config,SocketFactory socketFactory) {
        this.config = config;
        this.socketFactory = socketFactory;
    }


    /**
     * Synchronously send raw bytes to the socket
     *
     * @param message     bytes to send to socket
     * @throws AgentdIOException
     */
    @Override
    public void send(byte[] message) {

        try (Socket socket = socketFactory.createSocket(config.getHost(), config.getPort());
             OutputStream out = socket.getOutputStream())
        {
            out.write(message);
        } catch (IOException e) {
            throw new AgentdIOException("Error sending to agentd", e);
        }
    }

    /**
     * Synchronously read <n> bytes from the socket.
     *
     * @param n   the number of bytes to read
     * @return    bytes read
     */
    @Override
    public byte[] recv(int n) {

        byte[] response = new byte[n];

        try (Socket socket = socketFactory.createSocket(config.getHost(), config.getPort());
             DataInputStream in = new DataInputStream(socket.getInputStream()))
        {
            in.readFully(response);
        } catch (IOException e) {
            throw new AgentdIOException("Error receiving from agentd", e);
        }

        return response;
    }


}
