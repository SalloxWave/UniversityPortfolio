using System;

namespace Ludo.Engine
{
    public class InvalidTokenColorException : Exception
    {
        public InvalidTokenColorException() { }

        public InvalidTokenColorException(string message)
        : base(message) { }

        public InvalidTokenColorException(string message, Exception inner)
        : base(message, inner) { }
    }

    public class InvalidPlayerCountException : Exception
    {
        public InvalidPlayerCountException() { }

        public InvalidPlayerCountException(string message)
        : base(message) { }

        public InvalidPlayerCountException(string message, Exception inner)
        : base(message, inner) { }
    }

    public class DiceTossException : Exception
    {
        public DiceTossException() { }

        public DiceTossException(string message)
        : base(message) { }

        public DiceTossException(string message, Exception inner)
        : base(message, inner) { }
    }

    public class GameOverException : Exception
    {
        public GameOverException() { }

        public GameOverException(string message)
        : base(message) { }

        public GameOverException(string message, Exception inner)
        : base(message, inner) { }
    }
}
