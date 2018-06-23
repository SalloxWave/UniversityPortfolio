using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ludo.Engine
{
    public class Square
    {
        public TokenColor TokenColor { get; set; }        
        public bool Occupied { get { return _token != null; } }
        public Token Token { get { return _token; } set
            {
                if (TokenColor != TokenColor.None && value.TokenColor != TokenColor)
                {
                    throw new InvalidTokenColorException("Can't set token with token color not the same as the square");
                }
                _token = value;
            } }

        private Token _token;

        public Square()
        {
            _token = null;
            TokenColor = TokenColor.None;
        }

        public Square(TokenColor tokenColor)
        {
            _token = null;
            TokenColor = tokenColor;
        }

        public Token PopToken()
        {
            Token tmp = _token;
            _token = null;
            return tmp;
        }
    }
}
