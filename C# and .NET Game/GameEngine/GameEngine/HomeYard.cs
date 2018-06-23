using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LudoEngine
{
    public class HomeYard
    {
        public TokenColor TokenColor { get; }
        public static readonly int TokenCount = 4;

        private Stack<Token> _tokens;

        public HomeYard(TokenColor tokenColor)
        {
            if (tokenColor == TokenColor.None)
            {
                throw new InvalidOperationException("A homeyard needs a color");
            }
                        
            TokenColor = tokenColor;
            _tokens = new Stack<Token>();   
        }

        public void Push(Token token)
        {
            //ERROR: if token doesn't have same color as homeyard
            if (token.TokenColor != TokenColor)
            {
                throw new InvalidOperationException("Token needs to be same token color as homeyard");
            }
            _tokens.Push(token);
        }

        public Token Take()
        {
            return _tokens.Pop();
        }

        public bool IsEmpty()
        {
            return _tokens.Count == 0;
        }

        public bool IsFull()
        {
            return _tokens.Count == TokenCount;
        }
    }
}
