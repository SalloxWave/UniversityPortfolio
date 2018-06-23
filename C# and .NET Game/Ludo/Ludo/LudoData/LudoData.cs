using System;
using System.Collections.Generic;
using System.Linq;
using Ludo.Engine;
using System.Xml;
using System.IO;

namespace Ludo.Data
{
    namespace alexmickeversion
    {
        public class LudoData : ILudoData
        {
            private const string DataPath = "../../Resources/";
            private const string FileName = "ludoGameState.xml";
            private const string FullPath = DataPath + FileName;

            private XmlDocument xmlDoc;
            private IEnumerable<XmlNode> data;

            public LudoData()
            {
                initXmlDocument();                
            }

            private void initXmlDocument()
            {
                xmlDoc = new XmlDocument();
                //Load xml file only if it exists
                if (Exists)
                {
                    try
                    {
                        xmlDoc.Load(FullPath);
                        data = xmlDoc.SelectSingleNode("LudoGameState").ChildNodes.Cast<XmlNode>();
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("Error occured when trying to load xml document: {0}", e.Message);
                    }
                }
            }

            #region Properties
            public bool Exists
            {
                get
                {
                    return File.Exists(Path.GetFullPath(FullPath));
                }
            }

            public int CurrentPlayer
            {
                get
                {
                    return int.Parse(data.First(n => n.Name == "CurrentPlayer").InnerText);
                }                
            }

            public List<int> DiceHistory
            {
                get
                {   
                    return data.First(n => n.Name == "DiceHistory").ChildNodes.Cast<XmlNode>()
                        .Select(n => int.Parse(n.InnerText)).ToList();
                }
            }

            public bool DiceTossed
            {
                get
                {
                    return bool.Parse(data.First(n => n.Name == "DiceTossed").InnerText);
                }
            }

            public bool GameOver
            {
                get
                {
                    return bool.Parse(data.First(n => n.Name == "GameOver").InnerText);
                }
            }

            public HomeYard[] HomeYards
            {
                get
                {
                    HomeYard[] homeYards = new HomeYard[PlayerCount];
                    XmlNodeList homeYardsNode = data.First(n => n.Name == "HomeYards").ChildNodes;
                    for(int i = 0; i < homeYardsNode.Count; i++)
                    {
                        XmlNode homeYardNode = homeYardsNode.Item(i);
                        TokenColor tokenColor = (TokenColor)Enum.Parse(typeof(TokenColor), homeYardNode["TokenColor"].InnerText);
                        HomeYard homeYard = new HomeYard(tokenColor);

                        XmlNodeList tokens = homeYardNode["Tokens"].ChildNodes;
                        foreach(XmlNode tokenNode in tokens)    
                            homeYard.Push(new Token(tokenColor, int.Parse(tokenNode["TokenID"].InnerText)));

                        homeYards[i] = homeYard;
                    }
                    return homeYards;       
                }
            }

            public int PlayerCount
            {
                get
                {
                    return int.Parse(data.First(n => n.Name == "PlayerCount").InnerText);
                }
            }

            public Player[] Players
            {
                get
                {
                    Player[] players = new Player[PlayerCount];
                    XmlNodeList playersNode = data.First(n => n.Name == "Players").ChildNodes;
                    for (int i = 0; i < playersNode.Count; i++)
                    {
                        XmlNode playerNode = playersNode.Item(i);
                        TokenColor tokenColor = (TokenColor)Enum.Parse(typeof(TokenColor), playerNode["TokenColor"].InnerText);
                        Player player = new Player(tokenColor);
                        player.Winner = bool.Parse(playerNode["Winner"].InnerText);
                        player.FinishedTokens = int.Parse(playerNode["FinishedTokens"].InnerText);                        

                        players[i] = player;
                    }
                    return players;
                }
            }

            public Square[] Squares
            {
                get
                {
                    XmlNodeList squaresNode = data.First(n => n.Name == "Squares").ChildNodes;
                    Square[] squares = new Square[squaresNode.Count];
                    for (int i = 0; i < squaresNode.Count; i++)
                    {
                        XmlNode squareNode = squaresNode.Item(i);
                        TokenColor tokenColor = (TokenColor)Enum.Parse(typeof(TokenColor), squareNode["TokenColor"].InnerText);
                        Square square = new Square(tokenColor);

                        XmlNode tokenNode = squareNode["Token"];
                        if (tokenNode.HasChildNodes)
                        {
                            TokenColor tokenTokenColor = (TokenColor)Enum.Parse(typeof(TokenColor), tokenNode["TokenColor"].InnerText);
                            Token token = new Token(tokenTokenColor, int.Parse(tokenNode["TokenID"].InnerText));
                            token.StepCount = int.Parse(tokenNode["StepCount"].InnerText);
                            square.Token = token;
                        }                     

                        squares[i] = square;
                    }
                    return squares;
                }
            }

            public int[] Winners
            {
                get
                {
                    return data.First(n => n.Name == "Winners").ChildNodes.Cast<XmlNode>()
                        .Select(n => int.Parse(n.InnerText)).ToArray();
                }
            }
            #endregion

            public void SaveState(ILudoEngine ludoEngine)
            {
                if (ludoEngine.GameOver)
                {
                    File.Delete(Path.GetFullPath(FullPath));
                    return;
                }

                //Remove all child elements in order to update new ones
                xmlDoc.RemoveAll();

                //Xml declaration
                XmlDeclaration xmlDecl = xmlDoc.CreateXmlDeclaration("1.0", "UTF-8", null);
                xmlDoc.AppendChild(xmlDecl);

                XmlNode ludoGameStateNode = xmlDoc.CreateElement("LudoGameState");
                //Player count
                XmlNode playerCountNode = xmlDoc.CreateElement("PlayerCount");
                playerCountNode.AppendChild(xmlDoc.CreateTextNode(ludoEngine.PlayerCount.ToString()));
                ludoGameStateNode.AppendChild(playerCountNode);

                //Current player
                XmlNode currentPlayerNode = xmlDoc.CreateElement("CurrentPlayer");
                currentPlayerNode.AppendChild(xmlDoc.CreateTextNode(ludoEngine.CurrentPlayer.ToString()));
                ludoGameStateNode.AppendChild(currentPlayerNode);

                //Dice tossed
                XmlNode diceTossedNode = xmlDoc.CreateElement("DiceTossed");
                diceTossedNode.AppendChild(xmlDoc.CreateTextNode(ludoEngine.DiceTossed.ToString()));
                ludoGameStateNode.AppendChild(diceTossedNode);

                //Game over
                XmlNode gameOverNode = xmlDoc.CreateElement("GameOver");
                gameOverNode.AppendChild(xmlDoc.CreateTextNode(ludoEngine.GameOver.ToString()));
                ludoGameStateNode.AppendChild(gameOverNode);

                //Dice history
                XmlNode diceHistoryNode = xmlDoc.CreateElement("DiceHistory");
                foreach(int result in ludoEngine.DiceHistory)
                {
                    XmlNode resultNode = xmlDoc.CreateElement("DiceResult");
                    resultNode.AppendChild(xmlDoc.CreateTextNode(result.ToString()));
                    diceHistoryNode.AppendChild(resultNode);
                }
                ludoGameStateNode.AppendChild(diceHistoryNode);

                //Winners
                XmlNode winnersNode = xmlDoc.CreateElement("Winners");
                foreach (int winner in ludoEngine.Winners)
                {
                    XmlNode winnerNode = xmlDoc.CreateElement("Winner");
                    winnerNode.AppendChild(xmlDoc.CreateTextNode(winner.ToString()));
                    winnersNode.AppendChild(winnerNode);
                }
                ludoGameStateNode.AppendChild(winnersNode);

                //Squares
                XmlNode squaresNode = xmlDoc.CreateElement("Squares");
                foreach (Square square in ludoEngine.Squares)
                {
                    XmlNode squareNode = xmlDoc.CreateElement("Square");
                    XmlNode colorNode = xmlDoc.CreateElement("TokenColor");
                    colorNode.AppendChild(xmlDoc.CreateTextNode(square.TokenColor.ToString()));
                    squareNode.AppendChild(colorNode);
                    XmlNode tokenNode = xmlDoc.CreateElement("Token");
                    if (square.Occupied)
                    {
                        XmlNode tokenColorNode = xmlDoc.CreateElement("TokenColor");
                        tokenColorNode.AppendChild(xmlDoc.CreateTextNode(square.Token.TokenColor.ToString()));
                        tokenNode.AppendChild(tokenColorNode);
                        XmlNode tokenIDNode = xmlDoc.CreateElement("TokenID");
                        tokenIDNode.AppendChild(xmlDoc.CreateTextNode(square.Token.TokenID.ToString()));
                        tokenNode.AppendChild(tokenIDNode);
                        XmlNode stepCountNode = xmlDoc.CreateElement("StepCount");
                        stepCountNode.AppendChild(xmlDoc.CreateTextNode(square.Token.StepCount.ToString()));
                        tokenNode.AppendChild(stepCountNode);
                    }
                    squareNode.AppendChild(tokenNode);
                    squaresNode.AppendChild(squareNode);
                }
                ludoGameStateNode.AppendChild(squaresNode);

                //Home yards
                XmlNode homeYardsNode = xmlDoc.CreateElement("HomeYards");
                foreach (HomeYard homeYard in ludoEngine.HomeYards)
                {
                    XmlNode homeYardNode = xmlDoc.CreateElement("HomeYard");
                    XmlNode colorNode = xmlDoc.CreateElement("TokenColor");
                    colorNode.AppendChild(xmlDoc.CreateTextNode(homeYard.TokenColor.ToString()));
                    homeYardNode.AppendChild(colorNode);
                    XmlNode tokensNode = xmlDoc.CreateElement("Tokens");
                    foreach (Token token in homeYard.Tokens)
                    {
                        XmlNode tokenNode = xmlDoc.CreateElement("Token");
                        XmlNode tokenIDNode = xmlDoc.CreateElement("TokenID");
                        tokenIDNode.AppendChild(xmlDoc.CreateTextNode(token.TokenID.ToString()));
                        tokenNode.AppendChild(tokenIDNode);
                        tokensNode.AppendChild(tokenNode);
                    }
                    homeYardNode.AppendChild(tokensNode);
                    homeYardsNode.AppendChild(homeYardNode);
                }
                ludoGameStateNode.AppendChild(homeYardsNode);

                //Players
                XmlNode playersNode = xmlDoc.CreateElement("Players");
                foreach (Player player in ludoEngine.Players)
                {
                    XmlNode playerNode = xmlDoc.CreateElement("Player");
                    XmlNode colorNode = xmlDoc.CreateElement("TokenColor");
                    colorNode.AppendChild(xmlDoc.CreateTextNode(player.TokenColor.ToString()));
                    playerNode.AppendChild(colorNode);
                    XmlNode winnerNode = xmlDoc.CreateElement("Winner");
                    winnerNode.AppendChild(xmlDoc.CreateTextNode(player.Winner.ToString()));
                    playerNode.AppendChild(winnerNode);
                    XmlNode finishedTokensNode = xmlDoc.CreateElement("FinishedTokens");
                    finishedTokensNode.AppendChild(xmlDoc.CreateTextNode(player.FinishedTokens.ToString()));
                    playerNode.AppendChild(finishedTokensNode);
                    playersNode.AppendChild(playerNode);
                }
                ludoGameStateNode.AppendChild(playersNode);

                //Add ludo game state node
                xmlDoc.AppendChild(ludoGameStateNode);
                xmlDoc.Save(FullPath);                  
            }

            public static void Delete()
            {
                File.Delete(Path.GetFullPath(FullPath));
            }
        }
    }    
}
