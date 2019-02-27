package com.velopayments.blockchain.agentd;

import org.junit.Before;
import org.junit.Test;

import javax.net.SocketFactory;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.net.Socket;

import static org.mockito.Mockito.*;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;


public class RemoteAgentChannelImplTest {

    RemoteAgentChannelImpl remoteAgentChannel;

    Socket socket;

    @Before
    public void setup() throws  Exception {

        // create a configuration
        String host = "localhost";
        int port = 999;

        // set up a mock socket
        socket = mock(Socket.class);
        SocketFactory socketFactory = mock(SocketFactory.class);
        when(socketFactory.createSocket(host, port)).thenReturn(socket);

        // create the remote agent under test
        remoteAgentChannel = new RemoteAgentChannelImpl(host, port, socketFactory);
        remoteAgentChannel.connect();
    }


    @Test
    public void send() throws Exception {

        // given a remote agent with a socket connection
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        when(socket.getOutputStream()).thenReturn(bos);

        // ... and a message to send
        byte[] message = "Hello!".getBytes();

        // when the message is sent
        remoteAgentChannel.send(message);

        // the bytes should be written to the socket
        assertThat(bos.toByteArray(), is(message));
    }

    @Test
    public void recv() throws Exception {

        // given a remote agent with a socket connection
        byte[] message = "This is a test.".getBytes();
        ByteArrayInputStream bis = new ByteArrayInputStream(message);
        when(socket.getInputStream()).thenReturn(bis);

        // when recv is invoked
        byte[] incoming = remoteAgentChannel.recv(message.length);

        // the message should be received
        assertThat(incoming, is(message));
    }
}
