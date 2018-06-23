using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LudoEngine
{
    /// <summary>
    /// Represents the logic of a Ludo game
    /// </summary>
    public interface ILudoEngine
    {
        Square[] Squares { get; }
        HomeYard[] HomeYards { get; }
        /// <summary>
        /// Whoose turn it is where for example 1 is player one
        /// </summary>
        int CurrentPlayer { get; }

        bool GameOver { get; }
        /// <summary>
        /// Toss dice to decide if current player can play or move out
        /// </summary>
        /// <returns>Result of the dice throw</returns>
        int TossDice();

        /// <summary>
        /// Move out current player's token from the home yard
        /// </summary>
        void MoveOutToken();

        /// <summary>
        /// Move current player's token according to last dice result and handle collisions
        /// </summary>
        /// <param name="squareIndex">Index of square where token to move is located</param>
        void MoveToken(int squareIndex);
    }
}
