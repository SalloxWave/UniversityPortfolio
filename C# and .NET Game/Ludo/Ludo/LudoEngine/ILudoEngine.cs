using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ludo.Engine
{
    /// <summary>
    /// Represents the logic of a Ludo game
    /// </summary>
    public interface ILudoEngine
    {
        bool GameOver { get; }
        Square[] Squares { get; }
        HomeYard[] HomeYards { get; }
        Player[] Players { get; }
        List<int> DiceHistory { get; }
        bool DiceTossed { get; }
        int PlayerCount { get; }
        /// <summary>
        /// All players who have won where first index represents player that won first
        /// </summary>
        int[] Winners { get; }
        /// <summary>
        /// Whoose turn it is where for example 1 is player one
        /// </summary>
        int CurrentPlayer { get; }
        
        /// <summary>
        /// Toss dice to decide if current player can play or move out
        /// </summary>
        /// <returns>Result of the dice throw</returns>
        int TossDice();

        /// <summary>
        /// Move out current player's token from the home yard
        /// </summary>
        /// <param name="tokenColor">Token color of homeyard to move out from</param>
        /// <returns>If the move out was a success</returns>
        /// <exception cref="InvalidTokenColorException"></exception>
        bool MoveOutToken(TokenColor tokenColor);

        /// <summary>
        /// Move current player's token according to last dice result and handle collisions
        /// </summary>
        /// <param name="squareIndex">Index of square where token to move is located</param>
        /// <returns>If moving the token was successfull</returns>
        bool MoveToken(int squareIndex);

        /// <summary>
        /// Resets the game to be able to play a new game
        /// </summary>
        void Restart();
    }
}
