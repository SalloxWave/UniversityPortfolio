import java.util.*;

class Direction {
    public String direction;
    public Direction(String dir)
    {
        direction = dir;
    }
    public int[] step(int x, int y)
    {
        //Step specified direction based on position
        if (direction.equals("left")) { return new int[]{x-1, y}; }
        else if (direction.equals("up")) { return new int[]{x, y-1}; }
        else if (direction.equals("right")) { return new int[]{x+1, y}; }
        else if (direction.equals("down")) { return new int[]{x, y+1}; }
        else if (direction.equals("upleft")) { return new int[]{x-1, y-1}; }
        else if (direction.equals("upright")) { return new int[]{x+1, y-1}; }
        else if (direction.equals("downleft")) { return new int[]{x-1, y+1}; }
        else if (direction.equals("downright")) { return new int[]{x+1, y+1}; }
        else { return new int[]{x, y}; }
    }
}

class Gomoku {
    public final int NUM_OF_X_TILES = 8;
    public final int NUM_OF_Y_TILES = 8;

    private char[][] gameBoard = new char[NUM_OF_X_TILES][NUM_OF_Y_TILES];

    //Map where the key is the player and the color is the value
    private HashMap<String, Character> players = new HashMap<String, Character>();

    private final char emptySpace = '¤';

    private char winningColor = emptySpace;

    public Gomoku() {
        //Reset game by filling board with empty spaces, resetting winner
        //and removing all players
        reset();
    }

    public char[][] getGameBoard() {
        return gameBoard;
    }

    public HashMap<String, Character> getActivePlayers() {
        return players;
    }

    public void addPlayer(String playerName, char color) {
        //Create new pair with player name and the color
        players.put(playerName, color);
        System.out.println("Gomoku: " + playerName + " joined as color " + color);
    }

    public void addTile(String playerName, int x, int y) {
        //Assign specified position to user's color
        gameBoard[x][y] = players.get(playerName);
    }

    public String getWinningMessage() {
        String title = "Team " + winningColor + " has won!";
        String winners = "\nWinners in team " + winningColor + ":";
        String losers = "";
        //Decide who is the loser based on the winner
        switch (winningColor) 
        {
            case 'x':
                losers = "\nLosers in team o:";
                break;
            case 'o':
                losers = "\nLosers in team x:";
            default:
                break;
        }
        
        //Go through all players
        for (Map.Entry<String, Character> player : players.entrySet()) 
        {
            //Player is a winner
            if (player.getValue() == winningColor)
            {
                winners+="\n" + player.getKey();
            }
            //Player is a loser
            else
            {
                losers+="\n" + player.getKey();
            }
        }
        return title + winners + losers;
    }

    public boolean decideWinner() {
        boolean left = false;
        boolean right = false;
        boolean up = false;
        boolean down = false;
        for (int x = 0; x < NUM_OF_X_TILES; x++)
        {
            for(int y = 0; y < NUM_OF_Y_TILES; y++)
            {
                //Get color of tile
                char color = gameBoard[x][y];

                //A tile was found
                if (color != emptySpace)
                {                    
                    //Check possible directions for tile to get five in a row in                    
                    if (x >= 4) { left = true; }                    
                    if (x <= (NUM_OF_X_TILES - 5)) { right = true; }                    
                    if (y >= 4) { up = true; }                    
                    if (y <= (NUM_OF_Y_TILES - 5)) { down = true; }

                    //Create list of possible directions to get five in a row in
                    List<Direction> directions = new ArrayList<Direction>();
                    if (left) { directions.add(new Direction("left")); }
                    if (right) { directions.add(new Direction("right")); }
                    if (up) { directions.add(new Direction("up")); }
                    if (down) { directions.add(new Direction("down")); }
                    if (up && left) { directions.add(new Direction("upleft")); }
                    if (up && right) { directions.add(new Direction("upright")); }
                    if (down && left) { directions.add(new Direction("downleft")); }
                    if (down && right) { directions.add(new Direction("downright")); }
           
                    //Go through possible directions
                    for (Direction dir : directions) 
                    {
                        //Check if there's five in a row of the specified color...
                        //...at the specified position...
                        //...in the specified direction
                        if (fiveInARow(color, x, y, dir))
                        {
                            //Set winner and return a team has won
                            winningColor = color;
                            return true;
                        }
                    }
                }
                //Reset possible directions for next tile
                left = false;
                right = false;
                up = false;
                down = false;
            }
        }
        return false;
    }

    private boolean fiveInARow(char color, int x, int y, Direction dir) {
        //Start at specified position
        int[] steps = new int[] {x, y};
        for (int i = 0; i < 5; i++)
        {   
            //Check if there's not a correct color on the new tile
            if (gameBoard[steps[0]][steps[1]] != color)
            {
                return false;
            }
            //Step in direction based on position
            steps = dir.step(steps[0], steps[1]);
        }
        return true;
    }

    public boolean validColor(String color) {
        //Valid colors are x or o
        return color.equals("x") || color.equals("o");
    }

    public boolean validColor(char color) {
        //Valid colors are x or o
        return color == 'x' || color == 'o';
    }

    public boolean canAddTile(int x, int y) {
        //x-cordinate is outside of board
        if (x < 0 || x >= NUM_OF_X_TILES)
        {
            return false;
        }
        //y-cordinate is outside of board
        else if (y < 0 || y >= NUM_OF_Y_TILES)
        {
            return false;
        }
        //Space is not occupied (0) means the tile can be added
        return gameBoard[x][y] == emptySpace;
    }

    public boolean isActivePlayer(String userName) {
        return players.containsKey(userName);
    }    

    public void reset() {
        //Fill gameboard with empty spaces (0)
        for (int x = 0; x < NUM_OF_X_TILES; x++)
        {
            for(int y = 0; y < NUM_OF_Y_TILES; y++)
            {
                gameBoard[x][y] = emptySpace;
            }
        }
        //Remove all players
        players.clear();
        winningColor = emptySpace;
    }
}
