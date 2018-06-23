using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Ludo.Engine;
using System.Text.RegularExpressions;

namespace Ludo
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class LudoGUI : Window
    {
        private Rectangle[] _homeYards;
        private Rectangle[] _squares;
        private Button _dice;
        private Label _lblCurrentPlayer;        
        private Label _lblError;
        private string _errorMessage; 
        
        private const double _gameboardWidthProc = 0.45;
        private double _gameboardX;
        private double _gameboardY;
        private double _gameboardWidth;
        private double _gameboardHeight;

        private double _squareSideLength;
        private double _squareMargin;
        private double _homeYardLength;
        private double _tokenDiameter;
        private double _homeYardTokenMargin;

        private double _screenWidth;
        private double _screenHeight;        

        private int _diceResult;        
        private ILudoEngine _engine;

        public LudoGUI(ILudoEngine engine)
        {
            _engine = engine;            
            InitializeComponent();

            _screenWidth = SystemParameters.PrimaryScreenWidth;
            _screenHeight = SystemParameters.WorkArea.Bottom;

            if (_engine.DiceHistory.Count != 0)
                _diceResult = _engine.DiceHistory.Last();            
            
            initGameboard();
            updateGraphics();
        }

        private void initGameboard()
        {
            _gameboardWidth = _screenWidth * _gameboardWidthProc;
            _gameboardHeight = _gameboardWidth;
            _gameboardX = _screenWidth / 2.0 - _gameboardWidth / 2.0;
            _gameboardY = _screenHeight / 2.0 - _gameboardHeight / 2.0;

            _squareSideLength = _gameboardHeight / 11.0;
            _squareMargin = _squareSideLength / 6.0;
            _homeYardTokenMargin = _squareMargin;
            _squareMargin = 0;
            _homeYardLength = _squareSideLength * 4 + _squareMargin * 3;
            _tokenDiameter = _squareSideLength;
        }        
        
        private void updateGraphics()
        {   
            //Clear all components
            mainCanvas.Children.Clear();
            gameOverCanvas.Children.Clear();        

            //Draw updated components
            updateHomeYards();        
            updateSquares();
            updateDice();
            updateCurrentPlayer();
            updateErrorMessage();
            updateWinnerResult();
            if (_engine.GameOver)
                updateGameOver();
            _errorMessage = "";
            _dice.Focus();
        }

        private void updateHomeYards()
        {
            _homeYards = new Rectangle[_engine.HomeYards.Length];
            double leftX = _gameboardX;
            double rightX = _gameboardX + _homeYardLength + _squareSideLength * 3 + _squareMargin * 4;
            double topY = _gameboardY;
            double botY = _gameboardY + _homeYardLength + _squareSideLength * 3 + _squareMargin * 4;
            createHomeYard(TokenColor.Red, _homeYardLength, _homeYardLength, new Thickness(leftX, topY, 0, 0));
            createHomeYard(TokenColor.Green, _homeYardLength, _homeYardLength, new Thickness(rightX, topY, 0, 0));
            createHomeYard(TokenColor.Yellow, _homeYardLength, _homeYardLength, new Thickness(leftX, botY, 0, 0));
            createHomeYard(TokenColor.Blue, _homeYardLength, _homeYardLength, new Thickness(rightX, botY, 0, 0));

            for (int i = 0; i < _homeYards.Length; i++)
            {
                mainCanvas.Children.Add(_homeYards[i]);
            }

            HomeYard[] homeYards = _engine.HomeYards;

            for (int i = 0; i < homeYards.Length; i++)
            {
                double toptop = _homeYards[i].Margin.Top + _tokenDiameter - _homeYardTokenMargin * 2.0;
                double topbot = _homeYards[i].Margin.Top + _homeYards[i].Height - _tokenDiameter * 2.0 + _homeYardTokenMargin * 2.0;
                double leftleft = _homeYards[i].Margin.Left + _tokenDiameter - _homeYardTokenMargin * 2.0;
                double leftRight = _homeYards[i].Margin.Left + _homeYards[i].Width - _tokenDiameter * 2.0 + _homeYardTokenMargin * 2.0;

                double top;
                double left;
                for (int j = 0; j < homeYards[i].TokenCount; j++)
                {
                    top = j / 2 == 0 ? toptop : topbot;
                    left = j % 2 == 0 ? leftleft : leftRight;
                    createToken(homeYards[i].TokenColor, _tokenDiameter, _tokenDiameter, new Thickness(left, top, 0, 0));
                }
            }
        }

        private void createHomeYard(TokenColor tokenColor, double width, double height, Thickness margin)
        {
            Rectangle homeYard = new Rectangle();
            homeYard.Fill = getColorByTokenColor(tokenColor, false);
            homeYard.Width = width;
            homeYard.Height = height;
            homeYard.Margin = margin;
            homeYard.Name = tokenColor.ToString();
            homeYard.MouseDown += HomeYard_MouseDown;
            _homeYards[((int)tokenColor) - 1] = homeYard;
        }

        private void createToken(TokenColor tokenColor, double width, double height, Thickness margin)
        {
            Ellipse token = new Ellipse();
            token.Fill = getColorByTokenColor(tokenColor, true);
            token.Width = width;
            token.Height = height;
            token.Margin = margin;
            token.Name = tokenColor.ToString();
            token.MouseDown += HomeYard_MouseDown;
            mainCanvas.Children.Add(token);
        }

        private void updateSquares()
        {
            //Pattern for painting out squares
            //1
            //r u r d r d | l d l u l u
            //4 4 2 4 4 2 | 4 4 2 4 4 1

            Square[] squares = _engine.Squares;
            _squares = new Rectangle[squares.Length];

            int squareWidth = ((squares.Length / _engine.HomeYards.Length) - _engine.HomeYards.Length) / 2;
            int id = 0;
            double x = (int)_gameboardX;
            double y = (int)_gameboardY + _homeYardLength + _squareMargin;
            double steps = _squareSideLength + _squareMargin;

            addSquare(squares[id].TokenColor, _squareSideLength, _squareSideLength, new Thickness(x, y, 0, 0), id++);

            //r u r d r d
            //4 4 2 4 4 2
            Direction dir = new Direction("right");
            for (int i = 0; i < 12; i++)
            {
                if (i / 6 == 0)
                {
                    //r u r d r d
                    if (i % 2 == 0) dir = new Direction("right");
                    else if (i == 1) dir = new Direction("up");
                    else dir = new Direction("down");
                }
                else
                {
                    //l d l u l u
                    if (i % 2 == 0) dir = new Direction("left");
                    else if (i == 7) dir = new Direction("down");
                    else dir = new Direction("up");
                }

                //4 4 2 4 4 2 | 4 4 2 4 4 1
                int count;
                if (i == 11)
                    count = (squareWidth - 1) / 4;
                else if ((i + 1) % 3 == 0)
                    count = (squareWidth - 1) / 2;
                else
                    count = squareWidth - 1;

                for (int j = 0; j < count; j++)
                {
                    dir.Step(ref x, ref y, steps);
                    addSquare(squares[id].TokenColor, _squareSideLength, _squareSideLength, new Thickness(x, y, 0, 0), id++);
                }
            }

            //Add colored squares
            int counter = 0;
            for (int i = 0; i < _engine.HomeYards.Length; i++)
            {
                int dirNum = counter / 4;
                switch (dirNum)
                {
                    case 0: dir = new Direction("right"); break;
                    case 1: dir = new Direction("down"); break;
                    case 2: dir = new Direction("left"); break;
                    case 3: dir = new Direction("up"); break;
                }

                if (counter / 4 != 0)
                {
                    //Go to one step past beginning
                    dir.Reverse().Step(ref x, ref y, steps * 5);
                }

                for (int j = 0; j < squareWidth - 1; j++)
                {
                    dir.Step(ref x, ref y, steps);
                    addSquare(squares[id].TokenColor, _squareSideLength, _squareSideLength, new Thickness(x, y, 0, 0), id++);
                    counter++;
                }

                //Go to middle
                dir.Step(ref x, ref y, steps);
            }
        }

        private void addSquare(TokenColor tokenColor, double width, double height, Thickness margin, int squareIndex)
        {
            Rectangle square = new Rectangle();
            square.Fill = getColorByTokenColor(tokenColor, false);
            square.Width = width;
            square.Height = height;
            square.Margin = margin;
            square.Name = "Square" + squareIndex.ToString();
            square.MouseDown += Square_MouseDown;
            _squares[squareIndex] = square;
            mainCanvas.Children.Add(square);

            if (_engine.Squares[squareIndex].Occupied)
            {
                Ellipse token = new Ellipse();
                token.Width = _tokenDiameter;
                token.Height = _tokenDiameter;
                token.Margin = margin;
                token.Fill = getColorByTokenColor(_engine.Squares[squareIndex].Token.TokenColor, true);
                token.Name = "Token" + squareIndex.ToString();
                token.MouseDown += Square_MouseDown;
                mainCanvas.Children.Add(token);
            }
        }

        private void updateDice()
        {
            _dice = new Button();
            _dice.Margin = _homeYards[_engine.CurrentPlayer - 1].Margin;            
            _dice.Width = _tokenDiameter;
            _dice.Height = _tokenDiameter;
            _dice.Content = _diceResult == 0 ? "ROLL!" : _diceResult.ToString();
            _dice.Click += Dice_Click;
            mainCanvas.Children.Add(_dice);
        }

        private void updateCurrentPlayer()
        {
            TokenColor playerColor = (TokenColor)_engine.CurrentPlayer;
            _lblCurrentPlayer = new Label();

            TextBlock txtBlockDesc = new TextBlock();
            txtBlockDesc.Text = "Current Player: ";            

            TextBlock txtBlockClr = new TextBlock();
            txtBlockClr.Background = getColorByTokenColor(playerColor, false);
            txtBlockClr.Text = playerColor.ToString();

            StackPanel panel = new StackPanel();
            panel.Orientation = Orientation.Horizontal;
            panel.Children.Add(txtBlockDesc);
            panel.Children.Add(txtBlockClr);

            _lblCurrentPlayer.FontSize = 24;
            _lblCurrentPlayer.Content = panel;
            _lblCurrentPlayer.Margin = new Thickness(_gameboardX + _homeYardLength, _gameboardY - 50, 0, 0);
            mainCanvas.Children.Add(_lblCurrentPlayer);

            Rectangle marker = new Rectangle();
            marker.Fill = Brushes.Black;
            marker.Width = _homeYards[_engine.CurrentPlayer - 1].Width + _squareMargin;
            marker.Height = _homeYards[_engine.CurrentPlayer - 1].Height + _squareMargin;
            marker.Margin = _homeYards[_engine.CurrentPlayer - 1].Margin;
            Panel.SetZIndex(marker, -1);
            mainCanvas.Children.Add(marker);
        }

        private void updateErrorMessage()
        {
            _lblError = new Label();
            TextBlock txtBlock = new TextBlock();
            txtBlock.Text = _errorMessage;
            txtBlock.TextWrapping = TextWrapping.WrapWithOverflow;
            _lblError.Content = txtBlock;
            _lblError.MaxWidth = _screenWidth - (_gameboardX + _gameboardWidth);
            _lblError.FontSize = 24;
            _lblError.Margin = new Thickness(_homeYards[1].Margin.Left + _homeYardLength, _homeYards[1].Margin.Top + _homeYardLength, 0, 0);
            mainCanvas.Children.Add(_lblError);
        }

        private void updateWinnerResult()
        {   
            int[] winners = _engine.Winners;
            
            for (int i = 0; i < winners.Length; i++)
            {
                Label lbl = new Label();
                lbl.FontSize = 24;
                lbl.Content = "#" + (i+1);
                lbl.Measure(new Size(double.PositiveInfinity, double.PositiveInfinity));
                Rectangle homeYard = _homeYards[winners[i] - 1];
                lbl.Margin = new Thickness(homeYard.Margin.Left + _homeYardLength - lbl.DesiredSize.Width, homeYard.Margin.Top, 0, 0);
                mainCanvas.Children.Add(lbl);
            }
        }

        private void updateGameOver()
        {
            Button btnRestart = new Button();
            btnRestart.FontSize = 24;
            btnRestart.Content = "Restart!";
            btnRestart.Width = _gameboardWidth / 4;
            btnRestart.Height = _gameboardHeight / 6;
            btnRestart.Margin = new Thickness(_gameboardX + _gameboardWidth / 2 - btnRestart.Width / 2,
                                               _gameboardY + _gameboardHeight / 2 - btnRestart.Height / 2, 0, 0);
            btnRestart.Click += BtnRestart_Click;
            gameOverCanvas.Children.Add(btnRestart);

            Label lblGameOver = new Label();
            lblGameOver.FontSize = 48;
            lblGameOver.Content = "Game Over!";
            lblGameOver.Measure(new Size(double.PositiveInfinity, double.PositiveInfinity));
            lblGameOver.Margin = new Thickness(_gameboardX + _gameboardWidth / 2 - lblGameOver.DesiredSize.Width / 2,
                                               btnRestart.Margin.Top - lblGameOver.DesiredSize.Height, 0, 0);
            gameOverCanvas.Children.Add(lblGameOver);
            mainCanvas.IsEnabled = false;
        }

        private SolidColorBrush getColorByTokenColor(TokenColor tColor, bool isToken)
        {
            switch (tColor)
            {
                case TokenColor.Red:
                    return isToken ? Brushes.DarkRed : Brushes.Red;
                case TokenColor.Green:
                    return isToken ? Brushes.DarkOliveGreen : Brushes.Green;
                case TokenColor.Blue:
                    return isToken ? Brushes.DarkBlue : Brushes.Blue;
                case TokenColor.Yellow:
                    return isToken ? Brushes.Gold : Brushes.Yellow;
                default:
                    return Brushes.White;
            }
        }

        #region Events
        private void Square_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Shape square = (Shape)sender;
            int id = int.Parse(Regex.Match(square.Name, "\\d+").Value);            
            try
            {
                _engine.MoveToken(id);
                updateGraphics();
            }
            catch(InvalidOperationException)
            {
                _errorMessage = "Square doesn't contain a token";
                updateGraphics();
            }
            catch(InvalidTokenColorException)
            {
                _squares[id].Fill = Brushes.Red;
                _errorMessage = "Must move your own token";
                updateErrorMessage();
            }            
        }

        private void HomeYard_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Shape homeYard = (Shape)sender;
            bool result;
            try
            {
                result = _engine.MoveOutToken((TokenColor)Enum.Parse(typeof(TokenColor), homeYard.Name));
                if (result == false)
                {
                    _errorMessage = "You can't move out your token";                    
                }
                updateGraphics();                
            }
            catch(InvalidTokenColorException)
            {
                _errorMessage = "This is not your own homeyard";                
            }
        }


        private void Dice_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                _diceResult = _engine.TossDice();
            }
            catch (DiceTossException)
            {
                _errorMessage = "You have already tossed the dice!";
            }
            updateGraphics();
        }

        private void BtnRestart_Click(object sender, RoutedEventArgs e)
        {
            _engine.Restart();
            mainCanvas.IsEnabled = true;
            _diceResult = 0;
            updateGraphics();
        }
        #endregion

    }
}
