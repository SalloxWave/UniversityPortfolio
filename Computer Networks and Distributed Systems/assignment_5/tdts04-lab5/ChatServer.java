import ChatApp.*;          // The package containing our stubs. 
import org.omg.CosNaming.*;// HelloServer will use the naming service. 
import org.omg.CosNaming.NamingContextPackage.*; // ..for exceptions. 
import org.omg.CORBA.*;    // All CORBA applications need these classes. 
import org.omg.PortableServer.*;   
import org.omg.PortableServer.POA;
import java.util.*;

class User {
	private String name;
	
	public ChatCallback clientobj;

	public User(String name, ChatCallback clientobj)
	{
		this.name = name;
		this.clientobj = clientobj;
	}

	public String getName()
	{
		return name;
	}
}

class ChatImpl extends ChatPOA {
    private ORB orb;

	private List<User> users = new ArrayList<User>();

	//Init a new gomoku game
	private Gomoku gomoku = new Gomoku();

  public void setORB(ORB orb_val) 
  {
    orb = orb_val;
  }
	
	public void gomoku(ChatCallback callobj, String color)
	{
		//Client needs to be registered to join a Gomoku game
		if (!isRegistered(callobj))
		{
			callobj.callback("Error: You must join to play. Use command: 'join <username>' to join");
			return;
		}
		//Client must have a valid color to join a Gomoku game
		else if (!gomoku.validColor(color))
		{
			callobj.callback("Error: You can't join with color '" + color + "', please use 'x' or 'o'");
			return;
		}
		//Client can't join if client is already playing
		else if(gomoku.isActivePlayer(getUserByCallObject(callobj).getName()))
		{
			callobj.callback("Error: You are already in a Gomoku game session");
			return;
		}
		//Add a player to the game with specified username and team color
		gomoku.addPlayer(getUserByCallObject(callobj).getName(), color.charAt(0));

		callobj.callback("You entered a Gomoku game session with color '" + color + "'");		
		//Update gameboard only for joined player
		updateGameBoard(callobj);
	}

	public void add(ChatCallback callobj, int x, int y) {
		//Client can't add a tile if not in a game session
		if (!gomoku.isActivePlayer(getUserByCallObject(callobj).getName()))
		{
			callobj.callback("Error: You need to join to play the game. Use command: 'join <username>' to join");
			return;
		}
		//If position of tile is invalid
		else if(!gomoku.canAddTile(x, y))
		{
			callobj.callback("Error: You can't add tile on tile " + "[" + (x+1) + "," + (y+1) + "]");
			return;
		}
		//Add tile by specified username and cordinates
		gomoku.addTile(getUserByCallObject(callobj).getName(), x, y);

		//Print gameboard to all users since a new tile has been updated
		for (User user : users)
		{
			updateGameBoard(user.clientobj);
		}

		//Print gameboard to server by specifying callobject to null.
		//(I know it's ugly fix but whatever, it's at least better then 
		//overriding method with another parameter,
		//unfortunately Java doesn't support default paremters')
		updateGameBoard(null);

		//A team has won
		if (gomoku.decideWinner())
		{	
			//Broadcast winning message to all players
			broadcastMessage(gomoku.getWinningMessage(), null);
			//Reset the Gomoku game
			gomoku.reset();
		}
	}

	private void updateGameBoard(ChatCallback callobj) {
		char[][] gameBoard = gomoku.getGameBoard();
		String strGameBoard = "";
		//Go through whole gameboard and get row by row
		for (int x = 0; x < gomoku.NUM_OF_X_TILES; x++)
    {
      for(int y = 0; y < gomoku.NUM_OF_Y_TILES; y++)
      {
        strGameBoard+=Character.toString(gameBoard[x][y]) + " ";
      }
      strGameBoard+="\n";
    }
		//Print gameboard for either client or server
		if (callobj == null) { System.out.println(strGameBoard); }
		else { callobj.callback(strGameBoard); }
	}

	public void join(ChatCallback callobj, String userName) {
		//If user with specified username already is registered
		if (nameTaken(userName))
		{
			callobj.callback("Error: user " + userName + " is already an active chatter");
			return;
		}
		//Client has already joined
		else if(isRegistered(callobj))
		{
			callobj.callback("Error: you are already joined as user '" + getUserByCallObject(callobj).getName() + "'");
			return;
		}
		callobj.callback("Welcome " + userName + "!");
		//Add new client with specified name and specified call object
		users.add(new User(userName, callobj));

		//Broadcast message joining message to all clients except joined client (callobj)
		broadcastMessage(getUserByCallObject(callobj).getName() + " joined", callobj);

		System.out.println(userName + " joined the server");
	}

	public void leave(ChatCallback callobj) {
		//Client can't leave if client hasn't joined yet
		if (!isRegistered(callobj))
		{
			callobj.callback("Error: You need to join to leave. Use command: 'join <username>' to join");
			return;
		}
		callobj.callback("Goodbye " + getUserByCallObject(callobj).getName());

		//Broadcast message to all users except the one leaving (callobj)
		broadcastMessage(getUserByCallObject(callobj).getName() + " left", callobj);

		System.out.println(getUserByCallObject(callobj).getName() + " left the server");
		//Remove user from the server
		users.remove(getUserByCallObject(callobj));
	}

	public void post(ChatCallback callobj, String message) {
		//Client can't post if not joined
		if(!isRegistered(callobj))
		{
			callobj.callback("Error: You need to join to post. Use command: 'join <username>' to join");
			return;
		}
		//Broadcast the message to all users (exception is null means there's not exception)
		broadcastMessage(getUserByCallObject(callobj).getName() + " said: " + message, null);
	}

	public void list(ChatCallback callobj) {
		String listMsg="There are " + users.size() + " users currently registered:";
		//Add all names of registered users
		for (User user : users) 
		{
			listMsg+="\n" + user.getName();
		}
		//Print list to only client called the method
		callobj.callback(listMsg);
	}

	private User getUserByCallObject(ChatCallback callobj) {
		//Look which user the callobj is belonging to (null if not any user)
		for (User user : users) 
		{
			if (user.clientobj.equals(callobj))
			{
				return user;
			}
		}
		return null;
	}

	//Set exception to null if you don't want any exceptions
	private void broadcastMessage(String message, ChatCallback exception) {
		for (User user : users) 
		{
			//To not broadcoast to specified user
			if (!user.clientobj.equals(exception))
			{	
				user.clientobj.callback(message);
			}
		}
	}

	private boolean nameTaken(String userName) {
		//Loop for user with same name as specified name
		for (User user : users) 
		{
			if (user.getName().equals(userName))
			{
				return true;
			}
		}
		return false;
	}

	private boolean isRegistered(ChatCallback callobj) {
		//Check if callobject belongs to any of the users
		for (User user : users) 
		{
			if (user.clientobj.equals(callobj))
			{
				return true;
			}
		}
		return false;
	}
}

public class ChatServer {
    public static void main(String args[]) {
	try { 
	    // create and initialize the ORB
	    ORB orb = ORB.init(args, null); 

	    // create servant (impl) and register it with the ORB
	    ChatImpl chatImpl = new ChatImpl();
	    chatImpl.setORB(orb); 

	    // get reference to rootpoa & activate the POAManager
	    POA rootpoa = 
		POAHelper.narrow(orb.resolve_initial_references("RootPOA"));  
	    rootpoa.the_POAManager().activate(); 

	    // get the root naming context
	    org.omg.CORBA.Object objRef = 
		           orb.resolve_initial_references("NameService");
	    NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

	    // obtain object reference from the servant (impl)
	    org.omg.CORBA.Object ref = 
		rootpoa.servant_to_reference(chatImpl);
	    Chat cref = ChatHelper.narrow(ref);

	    // bind the object reference in naming
	    String name = "Chat";
	    NameComponent path[] = ncRef.to_name(name);
	    ncRef.rebind(path, cref);

	    // Application code goes below
	    System.out.println("ChatServer ready and waiting ...");
	    
	    // wait for invocations from clients
	    orb.run();
	}
	    
	catch(Exception e) {
	    System.err.println("ERROR : " + e);
	    e.printStackTrace(System.out);
	}
	System.out.println("ChatServer Exiting ...");
    }
}
