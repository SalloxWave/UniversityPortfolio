import ChatApp.*;          // The package containing our stubs
import org.omg.CosNaming.*;// HelloClient will use the naming service.
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;    // All CORBA applications need these classes.
import org.omg.PortableServer.*;   
import org.omg.PortableServer.POA;

import java.util.*;
import java.util.Scanner;

 
class ChatCallbackImpl extends ChatCallbackPOA {
    private ORB orb;

    public void setORB(ORB orb_val) {
        orb = orb_val;
    }

    public void callback(String notification)
    {
        System.out.println(notification);
    }
}

public class ChatClient {
    static Chat chatImpl;
    
    public static void main(String args[])
    {
		try 
		{
			// create and initialize the ORB
			ORB orb = ORB.init(args, null);

			// create servant (impl) and register it with the ORB
			ChatCallbackImpl chatCallbackImpl = new ChatCallbackImpl();
			chatCallbackImpl.setORB(orb);

			// get reference to RootPOA and activate the POAManager
			POA rootpoa = 
			POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
				rootpoa.the_POAManager().activate();
			
			// get the root naming context 
			org.omg.CORBA.Object objRef = 
			orb.resolve_initial_references("NameService");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
			
			// resolve the object reference in naming
			String name = "Chat";
			chatImpl = ChatHelper.narrow(ncRef.resolve_str(name));
			
			// obtain callback reference for registration w/ server
			org.omg.CORBA.Object ref = 
			rootpoa.servant_to_reference(chatCallbackImpl);
			ChatCallback cref = ChatCallbackHelper.narrow(ref);
			
			// Application code goes below
			System.out.println("Welcome to the chat server!");
			System.out.println("Please input command...");

			boolean quit = false;
			Scanner scanner = new Scanner(System.in);

			while(!quit)
			{	
				//Get user input
				String input = scanner.nextLine();

				//First word is command, rest is argument
				String command = input.split(" ")[0];				
				String arguments = input.substring(input.indexOf(" ")+1, input.length());

				//Look which command was used
				if (command.equals("join"))
				{
					chatImpl.join(cref, arguments);
				}
				else if(command.equals("leave"))
				{
					chatImpl.leave(cref);
				}
				else if(command.equals("list"))
				{
					chatImpl.list(cref);
				}
				else if(command.equals("post"))
				{
					chatImpl.post(cref, arguments);
				}
				else if(command.equals("help"))
				{
					String errorMsg = "Available commands:\n";
					errorMsg+="join <username>\nleave\nlist\npost <message>\nquit\ngomoku <color>\nadd <x> <y>";
					System.out.println(errorMsg);
				}
				else if(command.equals("quit"))
				{
					//Leave user, then quit
					chatImpl.leave(cref);
					quit = true;
				}
				else if(command.equals("gomoku"))
				{
					chatImpl.gomoku(cref, arguments);
				}
				else if(command.equals("add") || command.equals("a"))
				{
					try
					{
						//Get first and second argument of the arguments
						int x = Integer.parseInt(arguments.split(" ")[0]);
						int y = Integer.parseInt(arguments.split(" ")[1]);
						//Subtract 1 to get correct coordinates 
						//(correct in the means of how a coordinate system is represented)
						chatImpl.add(cref, x-1, y-1);
					}
					catch(Exception e)
					{
						System.out.println(arguments + " are not valid coordinates");
					}
				}
				else
				{
					System.out.println("\n'" + command + "' is not a valid command\nUse command 'help' for a list of available commands");
				}
			}
		}
	 	catch(Exception e)
		{
	    	System.out.println("ERROR : " + e);
	    	e.printStackTrace(System.out);
		}
    }
}
