using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameEngine
{
    public class InvalidTokenColorException : Exception
    {
        public override string Message { get { return customMessage; } }
        private string customMessage;
        private const string baseMessage = "Invalid token color:";
        public InvalidTokenColorException()
        {
            
        }

        public InvalidTokenColorException(string message)
        : base(message)
        {
        }

        public InvalidTokenColorException(string message, Exception inner)
        : base(message, inner)
    {
        }
    }
}
