using System;
using System.Collections;

namespace Ludo.Engine
{
    public class Player
    {
        public string Name { get; } = "Unnamed";
        public TokenColor TokenColor { get; }
        public bool Winner { get; set; }
        public int FinishedTokens { get; set; } = 0;

        public Player(TokenColor tokenColor)
        {
            if (tokenColor == TokenColor.None)
            {
                throw new InvalidTokenColorException("A player needs a color");
            }           
            TokenColor = tokenColor; 
        }

        public Player(TokenColor tokenColor, string name) :this(tokenColor)
        {
            Name = name;
        }
    }
}