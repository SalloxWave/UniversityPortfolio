using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ludo.Engine.alexmickeversion
{
    public class LudoRules : ILudoRules
    {
        public bool GameOver(Player[] players)
        {            
            return players.All(p => p.Winner);            
        }

        public int NewPosition(Square[] gameBoard, int startPos, int stepCount, int cycleCount)
        {
            int pos = startPos;
            Token activeToken = gameBoard[startPos].Token;
            int stepsTaken = activeToken.StepCount;

            // Remove uncoloured squares and divide by number of players to get number of coloured squares per player
            int goalDistance = (gameBoard.Length - cycleCount)/4 + 1;
            
            for (int i = 1; i <= stepCount; i++)
            {
                int oldPos = pos;

                //Step to next position
                pos+=1;

                // Check if we go from an uncoloured to a coloured square
                if (stepsTaken + i - 1 <= cycleCount && (stepsTaken + i) > cycleCount)
                {
                    pos = ((int)activeToken.TokenColor - 1) * 4 + cycleCount;

                    // If we try to go to the first coloured square and it's occupied, 
                    // set token to the previous uncoloured square
                    if (gameBoard[pos].Occupied)
                        return oldPos;                    
                }

                // Check if we go out of the uncoloured square bounds without
                //having walked a complete lap around the board
                if (stepsTaken + i < cycleCount && pos >= cycleCount)
                {
                    pos = pos % cycleCount;
                }

                //Already passed goal
                if (stepsTaken + i >= cycleCount + goalDistance) break;

                //Your own token is in the way
                if (gameBoard[pos].Occupied && gameBoard[pos].Token.TokenColor == gameBoard[startPos].Token.TokenColor)
                {                    
                    pos -= 1;
                    // If we tried to go from the last uncoloured square index to the first, but it was occupied, 
                    // return to last uncoloured square
                    if (pos < 0)
                    {
                        pos = cycleCount - 1;
                    }
                    return pos;
                }
            }

            // If we go past the goal square
            if (stepsTaken + stepCount >= cycleCount + goalDistance)
            {
                // Check if amount of steps result going into goal
                if (((stepsTaken + stepCount) % (cycleCount + goalDistance)) % 2 == 0)
                    return pos;                    
                else
                    return pos - 1;                    
            }

            return pos;           
        }

        public bool WillPush(Square[] gameBoard, int targetPosition, TokenColor playerTokenColor)
        {
            if (!gameBoard[targetPosition].Occupied)
                return false;
            //Don't push your own token
            return gameBoard[targetPosition].Token.TokenColor != playerTokenColor;
        }

        public bool CanMoveOutToken(Square[] gameBoard, int insertPosition, int diceResult, TokenColor playerTokenColor)
        {
            return diceResult == 6 && 
                (!gameBoard[insertPosition].Occupied || gameBoard[insertPosition].Token.TokenColor != playerTokenColor);
        }

        public bool HasWon(int finishedTokens, int tokenCount)
        {
            return finishedTokens == tokenCount;  
        }

        public bool InGoal(int tokenStepCount, int goalPosition)
        {
            return tokenStepCount == goalPosition;            
        }
    }
}
