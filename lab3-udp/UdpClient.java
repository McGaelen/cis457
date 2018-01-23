import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;
import java.util.*;

class UdpClient {
	public static void main(String args[]) {
		Console cons = System.console();
		int portNum = Integer.parseInt(cons.readLine("What port number? "));
		String ipAddr = cons.readLine("What IP Address? ");

		try {
			DatagramChannel dg = DatagramChannel.open();

			String m = cons.readLine("Enter your mesage: ");
			ByteBuffer buf = ByteBuffer.wrap(m.getBytes());
			dg.send(buf, new InetSocketAddress(ipAddr, portNum));
			ByteBuffer receiveBuf = ByteBuffer.allocate(4096);
			dg.receive(receiveBuf);
			String s = new String(receiveBuf.array());
			System.out.println("Received from server: " + s);
			dg.close();
		} catch (IOException e) {
			System.out.println("error");
		}
	}
}
