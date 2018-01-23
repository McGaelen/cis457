import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;
import java.util.*;

class UdpServer {
	public static void main(String args[]) {
		int portNum = Integer.parseInt(System.console().readLine("What port number? "));

		try {
			DatagramChannel c = DatagramChannel.open();
			c.bind(new InetSocketAddress(portNum));
			while (true) {
				ByteBuffer buffer = ByteBuffer.allocate(4096);
				SocketAddress clientaddr = c.receive(buffer);
				String message = new String(buffer.array());
				System.out.println(message);
				buffer.flip();
				c.send(buffer, clientaddr);
			}
		} catch (IOException e) {
			System.out.println("error");
		}
	}
}
