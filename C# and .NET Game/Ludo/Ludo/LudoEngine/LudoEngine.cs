using System;
using System.Collections.Generic;
using System.Linq;
using Ludo.Data;

namespace Ludo.Engine
{
    namespace alexmickeversion
    {
        public class LudoEngine : ILudoEngine
        {
            public static readonly int MaximumPlayers = 4;
            private static readonly int homeYardSize = MaximumPlayers;
            private static readonly int cycleCount = homeYardSize * 10;
            private static readonly int squareCount = homeYardSize * 14;
            private static readonly int goalStepCount = cycleCount + 5;
            public Square[] Squares { get { return _squares; } }
            public HomeYard[] HomeYards { get { return _homeYards; } }
            public int CurrentPlayer { get { return _currentPlayer + 1; } }
            public int[] Winners { get { return _winners.Where(p => p != 0).ToArray(); } }
            public bool GameOver { get { return _players.All(p => p.Winner); } }
            public List<int> DiceHistory { get { return _diceHistory; } }
            public bool DiceTossed { get { return _diceTossed; } }
            public int PlayerCount { get { return _playerCount; } }
            public Player[] Players { get { return _players; } }         

            private Player[] _players;
            private List<int> _winners;
            private HomeYard[] _homeYards;
            private Square[] _squares;
            private Dice _dice;
            private List<int> _diceHistory;
            private int _currentPlayer;
            private bool _diceTossed;
            private ILudoRules _ludoRules;
            private ILudoData _ludoData;
            private int _playerCount;

            public LudoEngine(ILudoRules ludoRules, ILudoData ludoData, int playerCount = 4)
            {
                _ludoRules = ludoRules;
                _ludoData = ludoData;
                if (ludoData.Exists)
                {
                    initFromData(ludoData);
                    return;
                }

                if (playerCount < 2 || playerCount > MaximumPlayers)
                {
                    throw new InvalidPlayerCountException("You need 2 - " + MaximumPlayers + " players to play Ludo");
                }
                _playerCount = playerCount;
                _winners = new List<int>();
                initPlayers();
                initHomeYards();
                initSquares();
                _dice = new Dice();
                _diceHistory = new List<int>();
                _currentPlayer = 0;
                _diceTossed = false;
            }

            private void initFromData(ILudoData ludoData)
            {
                _ludoData = ludoData;
                
                _players = _ludoData.Players;
                _homeYards = _ludoData.HomeYards;
                _squares = _ludoData.Squares;
                _diceHistory = ludoData.DiceHistory;
                _diceTossed = ludoData.DiceTossed;
                _playerCount = _ludoData.PlayerCount;
                _currentPlayer = _ludoData.CurrentPlayer - 1;
                _winners = _ludoData.Winners.ToList();
                
                
                _dice = new Dice();
            }

            public int TossDice()
            {
                if (_diceTossed)
                {
                    throw new DiceTossException("Current player has already tossed dice");
                }

                int result = _dice.Toss();
                _diceHistory.Add(result);

                if (_ludoRules.GameOver(_players))
                {
                    throw new GameOverException("Can't toss dice since the game is over");
                }

                _diceTossed = true;

                int tokensInPlay = HomeYard.StartingTokenCount - _homeYards[_currentPlayer].TokenCount - _players[_currentPlayer].FinishedTokens;
                //Player can't do anything or too many tosses with 6 as result in a row
                if (tokensInPlay == 0 && result != 6 || tooManySix())
                    nextTurn();

                _ludoData.SaveState(this);

                return result;
            }

            public bool MoveOutToken(TokenColor tokenColor)
            {
                bool canMoveOut = false;
                if (_ludoRules.GameOver(_players))
                {
                    throw new GameOverException("Can't move out token since game is over");
                }
                if (tokenColor != _players[_currentPlayer].TokenColor)
                {
                    throw new InvalidTokenColorException("Current player's color is not same as specified token color");
                }
                if (!_diceTossed) return false;
                if (_homeYards[_currentPlayer].IsEmpty()) return false;

                //Get where to start depending on current player's token color
                int pos = getStartPositionByColor(_players[_currentPlayer].TokenColor);

                if (_ludoRules.CanMoveOutToken(_squares, pos, _diceHistory.Last(), _players[_currentPlayer].TokenColor))
                {
                    if (_ludoRules.WillPush(_squares, pos, _players[_currentPlayer].TokenColor))
                    {
                        pushToken(pos);
                    }
                    //Move token from homeyard to squares
                    Token token = _homeYards[_currentPlayer].Take();
                    token.StepCount = 1;
                    placeToken(token, pos);                    
                    nextTurn();
                    
                    canMoveOut = true;
                }

                _ludoData.SaveState(this);

                return canMoveOut;
            }

            public bool MoveToken(int squareIndex)
            {
                if (squareIndex >= squareCount)
                {
                    string message = "Square index " + squareIndex + " is out of range";
                    throw new IndexOutOfRangeException(message);
                }

                if (_ludoRules.GameOver(_players))
                {
                    throw new GameOverException("Can't move token when game is over");
                }
                if (!_diceTossed) return false;
                if (_homeYards[_currentPlayer].IsFull()) return false;
                if (!_squares[squareIndex].Occupied)
                {
                    throw new InvalidOperationException("Can't move since there's no token on specified square");
                }
                if (_squares[squareIndex].Token.TokenColor != _players[_currentPlayer].TokenColor)
                {
                    throw new InvalidTokenColorException("Must move your own token");
                }

                int steps = _diceHistory.Last();

                //Uncomment to quicker go to goal, for testing
                //if (steps + _squares[squareIndex].Token.StepCount < cycleCount)
                //{
                //    steps = 35;
                //}

                Token activeToken = _squares[squareIndex].Token;
                int newPosition = _ludoRules.NewPosition(_squares, squareIndex, steps, cycleCount);

                activeToken.StepCount += countStepCount(activeToken.StepCount, squareIndex, newPosition);

                if (_ludoRules.InGoal(activeToken.StepCount, goalStepCount))
                {
                    _squares[squareIndex].PopToken();
                    _players[_currentPlayer].FinishedTokens++;                    
                    if (_ludoRules.HasWon(_players[_currentPlayer].FinishedTokens, HomeYard.StartingTokenCount))
                    {
                        _players[_currentPlayer].Winner = true;
                        _winners.Add(CurrentPlayer);
                    }
                    nextTurn();
                    _ludoData.SaveState(this);
                    return true;
                }

                _squares[squareIndex].PopToken();

                if (_ludoRules.WillPush(_squares, newPosition, _players[_currentPlayer].TokenColor))
                {
                    pushToken(newPosition);
                }
                //_squares[squareIndex].PopToken();
                placeToken(activeToken, newPosition);                

                //Next turn if player should not play again
                if (!playAgain() || _players[_currentPlayer].Winner) nextTurn();
                else clearDice();  //Make player able to roll dice and play again

                _ludoData.SaveState(this);
                return true;
            }

            public void Restart()
            {
                _players = new Player[_playerCount];
                _winners = new List<int>();
                initPlayers();
                initHomeYards();
                initSquares();
                _diceHistory.Clear();
                _currentPlayer = 0;
                _diceTossed = false;
            }

            #region Private methods
            private void clearDice()
            {
                _diceHistory.Clear();
                _diceTossed = false;
            }

            private int countStepCount(int prevStepCount, int startPos, int newPosition)
            {
                //New position is inside colored square, meaning different step count than new position
                if (startPos < cycleCount && newPosition > cycleCount)                    
                    return (cycleCount - prevStepCount) + (newPosition - cycleCount - (_currentPlayer * 4)) + 1;                  
                else if (newPosition < startPos)
                    return cycleCount - (startPos - newPosition);
                else
                    return newPosition - startPos;
            }

            private void pushToken(int pos)
            {
                _homeYards[((int)_squares[pos].Token.TokenColor) - 1].Push(_squares[pos].PopToken());
            }

            private void placeToken(Token token, int position)
            {
                _squares[position].Token = token;
            }

            private void nextTurn()
            {
                if (_ludoRules.GameOver(_players)) return;

                _diceTossed = false;                

                //Next player in array
                if (_currentPlayer + 1 == _players.Length)
                    _currentPlayer = 0;
                else
                    _currentPlayer++;

                //Go to next turn if current player already won
                if (_players[_currentPlayer].Winner)
                {
                    nextTurn();
                    return;
                }

                _diceHistory.Clear();
            }

            private int getStartPositionByColor(TokenColor tokenColor)
            {
                if (tokenColor == TokenColor.None)
                {
                    throw new InvalidTokenColorException("Can't get start position when color is none");
                }

                //Color - 1 times a quarter lap in the square
                return (((int)tokenColor) - 1) * (cycleCount / 4);
            }

            private bool playAgain()
            {
                //Not too many six and last result was a six
                return !tooManySix() && _diceHistory.Last() == 6;
            }

            private bool tooManySix()
            {
                return _diceHistory.Count(h => h == 6) >= 3;
            }

            private void initHomeYards()
            {
                _homeYards = new HomeYard[homeYardSize];
                for (int i = 0; i < homeYardSize; i++)
                {
                    TokenColor homeYardColor = (TokenColor)(i + 1);
                    _homeYards[i] = new HomeYard(homeYardColor);
                    for (int j = 0; j < HomeYard.StartingTokenCount; j++)
                    {
                        int id = j + (i * HomeYard.StartingTokenCount + 1);
                        _homeYards[i].Push(new Token(homeYardColor, id));
                    }
                }
            }

            private void initSquares()
            {
                _squares = new Square[squareCount];

                //Create colorless squares
                for (int i = 0; i < cycleCount; i++)
                {
                    _squares[i] = new Square();
                }

                //Create colored squares
                for (int i = cycleCount; i < squareCount; i++)
                {
                    _squares[i] = new Square((TokenColor)(((i - cycleCount) / 4) + 1));
                }
            }

            private void initPlayers()
            {
                _players = new Player[_playerCount];
                for (int i = 0; i < _playerCount; i++)
                {
                    _players[i] = new Player((TokenColor)(i + 1));
                }
            }
            #endregion
        }
    }
}
