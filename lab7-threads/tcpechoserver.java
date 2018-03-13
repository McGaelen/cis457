import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;

class tcpechoserver {
    public static void main(String args[]) {
        Console cons = System.console();
        int portNum = Integer.parseInt(cons.readLine("What port number? "));

        String message = "";
        try {
            ServerSocketChannel c = ServerSocketChannel.open();
            c.bind(new InetSocketAddress(portNum));
            SocketChannel sc = c.accept();
            TcpServerThread t = new TcpServerThread(sc);
            t.start();

            while (true) {
                message = cons.readLine("Enter your message: ");
                if (message.equals("quit")) {
                    break;
                }
                ByteBuffer buf = ByteBuffer.wrap(message.getBytes());
                sc.write(buf);
            }
            sc.close();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}

class TcpServerThread extends Thread {
    SocketChannel sc;
    Console cons;

    TcpServerThread(SocketChannel sc) {
        this.sc = sc;
        cons = System.console();
    }

    public void run() {
        String recieved;
        try {
            while (true) {
                ByteBuffer buffer = ByteBuffer.allocate(4096);
                sc.read(buffer);
                buffer.flip();
                byte[] a = new byte[buffer.remaining()];
                buffer.get(a);
                recieved = new String(a);
                System.out.println("Got from client: " + recieved);
            }
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
