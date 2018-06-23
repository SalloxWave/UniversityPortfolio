using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ludo.Engine
{
    public interface ILudoRules
    {
        bool GameOver(Player[] players);
        int NewPosition(Square[] gameBoard, int startPos, int stepCount, int cycleCount);
        bool CanMoveOutToken(Square[] gameBoard, int insertPosition, int diceResult, TokenColor playerTokenColor);
        bool WillPush(Square[] gameBoard, int targetPosition, TokenColor playerTokenColor);
        bool InGoal(int tokenStepCount, int goalPosition);
        bool HasWon(int finishedTokens, int tokenCount);        
    }
}
