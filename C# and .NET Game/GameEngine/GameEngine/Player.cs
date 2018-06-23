using System;
using System.Collections;

namespace LudoEngine
{
    public class Player
    {
        public string Name { get; } = "Unnamed";
        public TokenColor TokenColor { get; }
        public bool Winner { get { return FinishedTokens == HomeYard.TokenCount; } }
        public int FinishedTokens { get; set; } = 0;
        public bool CanMoveOut { get; set; } = false;
        public bool CanMove { get; set; } = false;
        public bool CanPlay { get { return (CanMoveOut || CanMove) && !Winner; } }
        public bool CanTossDice { get; set; } = true;

        private Token[] _tokens;
        private HomeYard _homeYard;

        public Player(TokenColor tokenColor)
        {
            if (tokenColor == TokenColor.None)
            {
                throw new InvalidOperationException("A player needs a color");
            }           
            TokenColor = tokenColor;
            _tokens = new Token[HomeYard.TokenCount];
        }

        public Player(TokenColor tokenColor, string name) :this(tokenColor)
        {
            Name = name;
        }
    }
}