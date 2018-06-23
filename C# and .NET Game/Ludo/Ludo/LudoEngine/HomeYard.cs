using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ludo.Engine
{
    public class HomeYard
    {
        public TokenColor TokenColor { get; }
        public Stack<Token> Tokens { get { return _tokens; } }
        public int TokenCount { get { return _tokens.Count; } }
        public static readonly int StartingTokenCount = 4;
        public bool IsEmpty() => _tokens.Count == 0;
        public bool IsFull() => _tokens.Count == StartingTokenCount;           

        private Stack<Token> _tokens;

        public HomeYard(TokenColor tokenColor)
        {
            if (tokenColor == TokenColor.None)
            {
                throw new InvalidTokenColorException("A homeyard needs a color");
            }
                        
            TokenColor = tokenColor;
            _tokens = new Stack<Token>();   
        }

        public void Push(Token token)
        {
            if (token.TokenColor != TokenColor)
            {
                throw new InvalidTokenColorException("Token needs to be same token color as homeyard");
            }
            _tokens.Push(token);
        }

        public void Push(Stack<Token> tokens)
        {
            _tokens = tokens;
        }

        public Token Take()
        {
            return _tokens.Pop();
        }
    }
}
