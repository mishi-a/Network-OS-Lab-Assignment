import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Properties;
import java.util.Scanner;
import java.util.Date;



class htmlRequestHandler implements Runnable
{
	private Socket client ;
	private String serverText;
	
	//constructor
	htmlRequestHandler(Socket client,String serverText)
	{
		this.client = client;
		this.serverText = serverText;
	}
	public htmlRequestHandler() {

	}
        
        //overriding run
	public void run()
	{
		try
	        {
			//Increasing active thread count by 1
			MultiThreadedHttpServer.threadCount++;
			DataOutputStream printStream = new DataOutputStream(client.getOutputStream());
			InputStreamReader inputReader = new InputStreamReader(client.getInputStream());
			BufferedReader bufferReader = new BufferedReader(inputReader);
			
 			//Reading Request from client
			String msg = bufferReader.readLine();
			System.out.print("Request message : "+msg + "\n");
			String[] tokens = msg.split("[ /]");
			if(tokens[2].length() == 0)
			{
				tokens[2] = this.serverText;
			}
			if(tokens[2].equals("HttpServer.java") || tokens[2].equals("MultiThreadedHttpServer.java"))
        		{
           			tokens[2] = "xyz";
       			}
			String data = "";
			FileInputStream fi = null;
			try
		        {
				String val =""+ System.getProperty("user.dir");
				File file = new File(val+"/"+tokens[2]);
				fi= new FileInputStream(file);
			} 
			catch (Exception e) 
			{
				System.out.println("no file");
				printStream.write(("HTTP/1.1 404 Not Found\r\n\r\n").getBytes());
				printStream.close();
				inputReader.close();
				bufferReader.close();
				Thread.sleep(10000);
				MultiThreadedHttpServer.threadCount--;
				return;
			}	
			printStream.write(("HTTP/1.1 200 OK\r\n").getBytes());
			if(tokens[2].contains("jpg"))
				printStream.write(("Content-Type: image/jpeg\r\n\r\n").getBytes());
			else
				printStream.write(("Content-Type: text/html\r\n\r\n").getBytes());
			//printStream.write(("Content-Length: " + data.length()+"\n\n").getBytes());;
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
			client.close();
			//make thread sleep for 10s so that we can see effect of max allowed thread
			Thread.sleep(10000);
			MultiThreadedHttpServer.threadCount--;
		}
		catch (Exception e) 
		{
			// TODO: handle exception
		}
	}
}

public class MultiThreadedHttpServer
{
	public static int threadCount = 0;
	public static void main(String[] args) 
	{
		ServerSocket server = null;
		int port = 1234;
		Scanner in = new Scanner(System.in);
		Properties prop = new Properties();
                
        //loading Config.config file
		String val =""+ System.getProperty("user.dir");
		String configFile = val+"/Config.config";
		try 
                {
			InputStream is = new FileInputStream(configFile);
			prop.load(is);
		} 
		catch (FileNotFoundException e1) 
		{
                        System.out.println("Config File not found");
			e1.printStackTrace();
			return;
		}
		catch (IOException e)
	        {
			e.printStackTrace();
			return;
		}

		System.out.println("Press any key to start the server........");
		in.next();
		try
		{
                        //ServerSocket at port 1234 created
			server = new ServerSocket(port);
			System.out.println("Server is started at port "+port);
		}
		catch (IOException e)
		{
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
				System.out.println("client request accepted at port "+port);
			}
			catch (IOException e)
			{
				System.out.println("ERROR : client request can't be accepted at port "+port);  
				e.printStackTrace();
				return;
			}
			String ip = prop.getProperty("blockIp");
			String[] ipTokens = ip.split(",");
			String clientIp = client.getRemoteSocketAddress().toString();
			clientIp = clientIp.substring(1, clientIp.indexOf(':'));
			System.out.println("Client ip is : "+clientIp);
			boolean block = false;

                        //Checking if Client ip is block
			for(int i=0;i<ipTokens.length;i++)
			{
				if(clientIp.equals(ipTokens[i]))
				{
					block = true;
					break;
				}
			}
			int allowedThread = Integer.parseInt(prop.getProperty("max_concurrent_thread"));
			String defaultPath = prop.getProperty("defaultFile");
			String[] fle = defaultPath.split("/");
			defaultPath = fle[1];
			System.out.println("Totthread = "+threadCount);

                        //If valid connection then create thread and process
			if(threadCount < allowedThread && block == false)
			{
				new Thread(new htmlRequestHandler(client, defaultPath)).start();	
			}
			else if(block == true)
			{
				//Case of blocked IP
				System.out.println("This Ip is Blocked");
				try 
				{
					DataOutputStream printStream = new DataOutputStream(client.getOutputStream());
					printStream.write(("HTTP/1.1 403.6 IP address rejected\r\n\r\n").getBytes());
					printStream.close();
					client.close();
				} 
				catch (IOException e)
			        {
					e.printStackTrace();
					return;
				}
				
			}
			else
			{
				System.out.println("Allowed Thread limit reached");
                                //Condition for thread limit reached
				try 
				{
					DataOutputStream printStream = new DataOutputStream(client.getOutputStream());
					Date today = new Date();
					printStream.write(("HTTP/1.1 403 Access Denied\r\n"+"Connection: close\r\n\r\n").getBytes());
					printStream.close();
					client.close();
				}
			        catch (IOException e)
			        {
					e.printStackTrace();
					return;
				}	
			}
		}
	}
}


