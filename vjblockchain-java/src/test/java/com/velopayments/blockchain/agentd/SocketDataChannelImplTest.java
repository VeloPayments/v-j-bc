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


public class SocketDataChannelImplTest {

    SocketDataChannelImpl socketDataChannel;

    Socket socket;

    ByteArrayInputStream bis;
    ByteArrayOutputStream bos;

    @Before
    public void setup() throws  Exception {

        // create a configuration
        String host = "localhost";
        int port = 999;

        // set up a mock socket
        socket = mock(Socket.class);

        bis = mock(ByteArrayInputStream.class);
        bos = new ByteArrayOutputStream();

        when(socket.getInputStream()).thenReturn(bis);
        when(socket.getOutputStream()).thenReturn(bos);


        // configure a socket factory to return the mock socket
        SocketFactory socketFactory = mock(SocketFactory.class);
        when(socketFactory.createSocket(host, port)).thenReturn(socket);

        // create the data channel under test
        socketDataChannel = new SocketDataChannelImpl(host, port, socketFactory);
        socketDataChannel.connect();
    }


    @Test
    public void send() throws Exception {

        // given a message to send
        byte[] payload = "Hello!".getBytes();

        // when the message is sent
        socketDataChannel.send(payload);

        // the message should be written to the socket
        byte[] message = bos.toByteArray();
        assertThat(message, is(payload));
    }

    @Test
    public void recv() throws Exception {
        // TODO
    }
}
