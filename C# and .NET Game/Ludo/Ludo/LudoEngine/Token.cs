using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Ludo.Engine
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
                throw new InvalidTokenColorException("A token needs a color");
            }

            if (tokenID < 1 || tokenID > HomeYard.StartingTokenCount* alexmickeversion.LudoEngine.MaximumPlayers)
            {
                throw new InvalidOperationException("Token id can't be higher than total amount of tokens or less than 1");
            }

            TokenColor = tokenColor;
            TokenID = tokenID;
        }
    }
}
