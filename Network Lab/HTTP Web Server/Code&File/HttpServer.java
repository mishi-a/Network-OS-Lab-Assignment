import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class HttpServer
{
  public static void main(String[] args)
  {
	  ServerSocket server = null;
	  int port = 1234;
	  Scanner in = new Scanner(System.in);
	  System.out.println("Press any key to start the server");
	  in.next();
	  try
	  {
		server = new ServerSocket(port);
		System.out.println("Server is started at port "+port);
	  }
	  catch (IOException e)
	  {
		// TODO Auto-generated catch block
		System.out.println("Error: Server with port "+port +" cannot be created");  
		e.printStackTrace();
		return;
	  }
	  
	  while(true)
	  {
		  Socket client = null;
		  try
		  {
			client = server.accept();
			String clientIp = client.getRemoteSocketAddress().toString();
			clientIp = clientIp.substring(1, clientIp.indexOf(':'));
			System.out.println("client request accepted at port "+port +" and ClienIp is "+clientIp);
		  }
		  catch (IOException e)
		  {
			// TODO Auto-generated catch block
			System.out.println("ERROR : client request can't be accepted at port "+port);  
			e.printStackTrace();
			return;
		  }
		  
		  try {
			processClientHtmlRequest(client);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	  }
  }

  private static void processClientHtmlRequest(Socket client) throws IOException
  {
	// TODO Auto-generated method stub
	DataOutputStream printStream = new DataOutputStream(client.getOutputStream());
	InputStreamReader inputReader = new InputStreamReader(client.getInputStream());
	BufferedReader bufferReader = new BufferedReader(inputReader);
	
	String msg = bufferReader.readLine();
	System.out.print("Request from client :" + msg+"\n");
	String[] tokens = msg.split("[ /]");
	String data = "";
        if(tokens[2].equals("HttpServer.java") || tokens[2].equals("MultiThreadedHttpServer.java"))
        {
           tokens[2] = "xyz";
        }
	FileInputStream fi = null; 
	try {
		String val =""+ System.getProperty("user.dir");
		//System.out.println(val);
		File file = new File(val+"/"+tokens[2]);
		fi= new FileInputStream(file);
	} catch (Exception e) {
		// TODO: handle exception
		System.out.println("no file");
		printStream.write(("HTTP/1.0 404 Not Found\r\n\r\n").getBytes());
		printStream.close();
		inputReader.close();
		bufferReader.close();
                System.out.println("connection close for this client\n");
		return;
	}
	printStream.write(("HTTP/1.1 200 OK\r\n").getBytes());
	if(tokens[2].contains("jpg"))
		printStream.write(("Content-Type: image/jpeg\r\n\r\n").getBytes());
	else
		printStream.write(("Content-Type: text/html\r\n\r\n").getBytes());
	//printStream.println("Content-Length: " + data.length());
	int totalByte;
	byte[] buffer = new byte[2048];
	while((totalByte = fi.read(buffer))!=-1)
	{
		printStream.write(buffer,0,totalByte);
	}
	printStream.flush();
	printStream.close();
	inputReader.close();
	bufferReader.close();
        System.out.println("connection close for this client\n");
  }
}

