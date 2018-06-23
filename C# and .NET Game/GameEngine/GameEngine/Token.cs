using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace LudoEngine
{
    public class Token
    {
        public TokenColor TokenColor { get; }
        public int TokenID { get; }
        public int StepCount { get; set; } = 0;

        public Token(TokenColor tokenColor, int tokenID)
        {
            if (tokenColor == TokenColor.None)
            {
                throw new InvalidOperationException("A token needs a color");
            }

            if (tokenID < 1 || tokenID > HomeYard.TokenCount *  LudoEngine.MaximumPlayers)
            {
                throw new InvalidOperationException("Token id can't be higher than total amount of tokens or less than 1");
            }

            TokenColor = tokenColor;
            TokenID = tokenID;
        }
    }
}
