import javax.swing.*;

public class RouterNode {
  private int myID;
  private GuiTextArea myGUI;
  private RouterSimulator sim;
  private int[] costs = new int[RouterSimulator.NUM_NODES];
  // Possible routes to take represented by ID of nodes
  private int[] routes = new int[RouterSimulator.NUM_NODES];
  // Node ID connected to cost
  private int[][] vectors = new int[RouterSimulator.NUM_NODES][RouterSimulator.NUM_NODES];

  boolean poisonReverse = false;

  //--------------------------------------------------
  public RouterNode(int ID, RouterSimulator sim, int[] costs) {
    myID = ID;
    this.sim = sim;
    myGUI =new GuiTextArea("  Output window for Router #"+ ID + "  ");

    // Assign your own costs.
    System.arraycopy(costs, 0, this.costs, 0, RouterSimulator.NUM_NODES);

    // Go through amount of nodes on network.
    for(int i = 0; i < RouterSimulator.NUM_NODES; i++)
    {
      // Add the node to the network.
      routes[i] = i;      
      for(int j = 0; j < RouterSimulator.NUM_NODES; j++) 
      {      
        // Insert your own costs in neighbour table.
        // INFINITY to non-neighbours.
        if( i == myID )
        {
          vectors[i][j] = costs[j]; 
        }
        else
        {
          vectors[i][j] = RouterSimulator.INFINITY;
        }
      }
    }
    sendToNeighbours();
  }
  //--------------------------------------------------
  private void sendToNeighbours() {
    int[] sendCosts = new int[RouterSimulator.NUM_NODES];
    
    // go through all *nodes*
    for (int neighbour = 0; neighbour <RouterSimulator.NUM_NODES; neighbour++)
    {
			// make sure we only go through *neighbours*
      if ( neighbour != myID && costs[neighbour] != RouterSimulator.INFINITY)
      {
		  	// copy vectors from current node in case we want to alter them.
        System.arraycopy(vectors[myID], 0, sendCosts, 0, RouterSimulator.NUM_NODES); 

        if ( poisonReverse )
        {
          // loop through all possible destinations.
          for (int dest = 0; dest < RouterSimulator.NUM_NODES; dest++)
          {
						// if our route to destination takes us through the 
						//  neighbour we're on right now, send our cost to 
						//  destination to infinity.
            if ( routes[dest] == neighbour && dest != neighbour )
            {
              sendCosts[dest] = RouterSimulator.INFINITY;
            }
          }
        }
				// send current packet.
        RouterPacket packet = new RouterPacket(myID, neighbour, sendCosts);
        sendUpdate(packet);
      }
    }
  }

  //--------------------------------------------------
  private boolean modifyCost() {
    boolean costChanged = false;

    // Go through all nodes on the network
    //  to find the cheapest path to the destination.
    for (int destination = 0; destination < RouterSimulator.NUM_NODES; destination++)
    {
			// you're not supposed to go to yourself
      if ( destination != myID )
      {
				// set cheapest route to initial cost
        int cheapest = costs[destination];
        // set route to current destination
        int route = destination;
        // go through possible routes to destination
        for (int neighbour = 0; neighbour < RouterSimulator.NUM_NODES; neighbour++)
        {
					// route is not to yourself nor a non-neighbour
          if ( neighbour != myID && costs[neighbour] != RouterSimulator.INFINITY )
          {
						// calculate cost based on cost to neighbour and 
						//  neighbours cost to destination
            int altCost = costs[neighbour] + vectors[neighbour][destination];
            // check if the possible path is cheaper than current cheapest path
            if (altCost < cheapest)
            {
              cheapest = altCost;
              route = neighbour;
            }
          }
        }

				// if a route has changed, update vectors' route and cost
        if (cheapest != vectors[myID][destination])
        {
          vectors[myID][destination] = cheapest;
          routes[destination] = route;
          costChanged = true;
        }
      }
    }
    // A cost has been modified or not
    return costChanged;
  }

  //--------------------------------------------------
  public void recvUpdate(RouterPacket pkt) {  
    // overwrite the old vectors from the source with new ones.
    System.arraycopy(pkt.mincost, 0, vectors[pkt.sourceid], 0, RouterSimulator.NUM_NODES);

		// check if anything has changed.
    if ( modifyCost() )
    {
      sendToNeighbours();
    }
  }
  
  //--------------------------------------------------
  private void sendUpdate(RouterPacket pkt) {
    sim.toLayer2(pkt);
  }

  //--------------------------------------------------
  public void printDistanceTable() {
    // oof. trust us. it works. minimize and move on.
	  myGUI.println(" Current state for " + myID +
			"  at time " + sim.getClocktime());

    myGUI.println();

    myGUI.println(" Distancetable:");
    String strHeader = F.format(" dest |", 10);
    for (int i = 0; i < RouterSimulator.NUM_NODES; i++)
    {
      strHeader+= F.format(Integer.toString(i), 6);
    }
    myGUI.println(strHeader);
    myGUI.println(new String(new char[strHeader.length()]).replace("\0", "-"));

    // Setup printing for neighbours
    for (int i = 0; i < RouterSimulator.NUM_NODES; i++)
    {
        String str = F.format("nbr ", 5);
        str+=F.format(Integer.toString(i), 3);
        str+=F.format("|", 2);
        for (int j = 0; j < RouterSimulator.NUM_NODES; j++)
        {
          str+=F.format(Integer.toString(vectors[i][j]), 6);
        }        
        myGUI.println(str);
    }
    myGUI.println();
    myGUI.println(" Our distance vector and routes:");

    strHeader = F.format(" dest |", 10);
    for (int i = 0; i < RouterSimulator.NUM_NODES; i++)
    {
      strHeader+= F.format(Integer.toString(i), 6);
    }
    myGUI.println(strHeader);
    // Print a line
    myGUI.println(new String(new char[strHeader.length()]).replace("\0", "-"));

    // Print routes double woohoo
    String strRoutes = F.format("route", 6);
    strRoutes+= F.format("|", 4);
    for (int i = 0; i < RouterSimulator.NUM_NODES; i++)
    {      
      if ( i == myID )
      {
        strRoutes+=F.format("-", 6);
      }
      else
      {
        strRoutes+=F.format(routes[i], 6);
      }
    }
    myGUI.println(strRoutes);

    // Print costs woohoo
    String strCosts = F.format("cost", 5);
    strCosts+= F.format("|", 5);
    for (int i = 0; i < RouterSimulator.NUM_NODES; i++)
    {
       strCosts+= F.format(Integer.toString(vectors[myID][i]), 6);
    }
    myGUI.println(strCosts);

    myGUI.println();
  }

  //--------------------------------------------------
  public void updateLinkCost(int dest, int newcost) {
    // alter the cost to the affected node.
    // only time it's allowed to alter the variable 'costs'.
    costs[dest] = newcost;

    if ( modifyCost() )
    {
      sendToNeighbours();
    }
  }
}
