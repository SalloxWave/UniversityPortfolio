using System;
using System.Collections.Generic;
using System.Linq;

namespace LudoEngine
{
    public enum TokenColor
    {
        None = 0,
        Red = 1,
        Green = 2,
        Blue = 3,
        Yellow = 4
    }

    public class LudoEngine : ILudoEngine
    { 
        public static readonly int MaximumPlayers = 4;
        private static readonly int homeYardSize = MaximumPlayers;
        private static readonly int cycleCount = homeYardSize * 10;
        private static readonly int squareCount = homeYardSize * 14;
        public Square[] Squares { get { return _squares; } }
        public HomeYard[] HomeYards { get { return _homeYards; } }
        public int CurrentPlayer { get { return _currentPlayer + 1; } }
        public bool GameOver { get { return _players.All(p => p.Winner); } }
        
        private Player[] _players;
        private int[] _winners;
        private HomeYard[] _homeYards;
        private Square[] _squares;
        private Dice _dice;
        private List<int> _diceHistory;
        private int _currentPlayer;
        private int _numberOfSteps;

        public LudoEngine(int playerCount)
        {
            if (playerCount < 2 || playerCount > MaximumPlayers)
            {
                throw new InvalidOperationException("You need 2 - " + MaximumPlayers + " players to play Ludo");
            }

            _players = new Player[playerCount];
            for (int i = 0; i < playerCount; i++)
            {
                _players[i] = new Player((TokenColor)(i + 1));                
            }
            initHomeYard();
            initSquares();
            _winners = new int[playerCount];            
            _dice = new Dice();
            _diceHistory = new List<int>();
            _currentPlayer = 0;
            _numberOfSteps = -1;
        }

        public int TossDice()
        {
            int result = _dice.Toss();
            _diceHistory.Add(result);

            if (GameOver) return result;

            _players[_currentPlayer].CanTossDice = false;

            bool tooManySix = _diceHistory.Count(h => h == 6) >= 3;

            //Can play if player has a token in play
            _players[_currentPlayer].CanMove = !_homeYards[_currentPlayer].IsFull();

            //Can move out if result is 6
            _players[_currentPlayer].CanMoveOut = result == 6;                
                        
            //Player can't do anything or too many tosses with 6 as result in a row
            if (!_players[_currentPlayer].CanPlay || tooManySix)
                nextTurn();

            return result;
        }

        public void MoveOutToken()
        {
            if (GameOver) return;

            if (!_players[_currentPlayer].CanMoveOut) return;

            //Get where to start depending on current player's token color
            int pos = getStartPositionByColor(_players[_currentPlayer].TokenColor);

            if (_squares[pos].Occupied)
            {
                if (_squares[pos].Token.TokenColor != _players[_currentPlayer].TokenColor)
                {
                    _homeYards[((int)_squares[pos].TokenColor) - 1].Push(_squares[pos].PopToken());                    
                }
                else
                {
                    _players[_currentPlayer].CanMoveOut = false;                                                           
                    return;
                }
            }

            //Move token from homeyard to squares
            _squares[pos].Token = _homeYards[_currentPlayer].Take();

            nextTurn();
        }

        public void MoveToken(int squareIndex)
        {
            if (squareIndex >= squareCount)
            {
                string message = "Square index " + squareIndex + " is out of range";
                throw new IndexOutOfRangeException(message);
            }

            if (GameOver) return;
            if (!_players[_currentPlayer].CanMove) return;

            if (_numberOfSteps == -1)
                _numberOfSteps = _diceHistory.Last();

            Token activeToken = _squares[squareIndex].Token;
            activeToken.StepCount += _numberOfSteps;

            //Step one at a time and look for token color with same token color
            //If found, step to position behind your own token
            for (int i = 1; i <= _numberOfSteps; i++)
            {
                if(_squares[squareIndex+i].Occupied && _squares[squareIndex+i].Token.TokenColor == _players[_currentPlayer].TokenColor)
                {
                    _numberOfSteps = i - 1;
                    MoveToken(squareIndex);
                    return;
                }
            }

            int goalStepCount = cycleCount + (homeYardSize / 4) + 1;
            if (activeToken.StepCount >= goalStepCount)
            {
                int goal = (activeToken.StepCount - goalStepCount) % 2;
                //Successfully entered goal
                if (goal == 0)
                {
                    _players[_currentPlayer].FinishedTokens++;
                    _squares[squareIndex].PopToken();
                    return;
                }
                //Step to position before goal
                else { _numberOfSteps = goalStepCount - squareIndex - 1; }
            }

            int newPos;
            //Will go inside colored squares
            if (activeToken.StepCount >= cycleCount)
                newPos = activeToken.StepCount + (_currentPlayer * homeYardSize);
            else            
                newPos = (_numberOfSteps + squareIndex) % cycleCount;

            //Empty square
            if (!_squares[newPos].Occupied)
            {               
                _squares[newPos].Token = activeToken;
                _squares[squareIndex].PopToken();

                nextTurn();
            }
            //Occupied, different color
            else if (activeToken.TokenColor != _squares[newPos].Token.TokenColor)
            {
                //Push enemy token to its homeyard
                int index = (int)_squares[newPos].Token.TokenColor - 1;
                Token enemy = _squares[newPos].PopToken();
                enemy.StepCount = 0;
                _homeYards[index].Push(enemy);

                //Move token forward
                _squares[newPos].Token = activeToken;
                _squares[squareIndex].PopToken();

                nextTurn();
            }
            //Occupied, same color
            //else
            //{
            //    //Check between 
            //    --_numberOfSteps;
            //    MoveToken(squareIndex);
            //}
            _numberOfSteps = -1;
        }

        private void nextTurn()
        {
            if (GameOver) return;
            if (playAgain()) return;
            
            //Go to next turn if current player already won
            if (_players[_currentPlayer].Winner)
            {
                nextTurn();
                return;
            }                
            
            //Reset player
            _players[_currentPlayer].CanMoveOut = false;
            _players[_currentPlayer].CanMove = false;
            _players[_currentPlayer].CanTossDice = true;

            //Next player in array
            if (_currentPlayer + 1 == _players.Length)
                _currentPlayer = 0;
            else
                _currentPlayer++;            

            _diceHistory.Clear();
        }

        private int getStartPositionByColor(TokenColor tokenColor)
        {
            if (tokenColor == TokenColor.None)
            {
                throw new InvalidOperationException("Can't get start position when color is none");
            }

            //Color - 1 times a quarter lap in the square
            return (((int)tokenColor) - 1) * (cycleCount / 4);
        }
        
        private bool playAgain()
        {
            //Not too many six and last result was a six
            return _diceHistory.Count(h => h == 6) < 3 && _diceHistory.Last() == 6;
        }

        private void initHomeYard()
        {
            _homeYards = new HomeYard[homeYardSize];
            for (int i = 0; i < homeYardSize; i++)
            {
                TokenColor homeYardColor = (TokenColor)(i + 1);
                _homeYards[i] = new HomeYard(homeYardColor);
                for (int j = 0; j < HomeYard.TokenCount; j++)
                {
                    int id = j + (i * HomeYard.TokenCount + 1);
                    _homeYards[i].Push(new Token(homeYardColor, id));                    
                }
            }
        }

        private void initSquares()
        {
            _squares = new Square[squareCount];

            //Create colorless squares
            for(int i = 0; i < cycleCount; i++)
            {
                _squares[i] = new Square();
            }

            //Create colored squares
            for(int i = cycleCount; i < squareCount; i++)
            {
                _squares[i] = new Square((TokenColor)(((i - cycleCount)/4) + 1));
            }
        }
    }
}
