using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LudoEngine
{
    class Program
    {
        static void Main(string[] args)
        {            
            //testToss(new LudoEngine(2));
            //testSquares(new LudoEngine(2));
            //testHomeYards(new LudoEngine(2));
            testMoveOut(new LudoEngine(2));
            
            Console.ReadKey();
        }

        private static void testToss(ILudoEngine ludoEngine)
        {
            for (int i = 0; i < 10; i++)
            {
                Console.Write("Player " + ludoEngine.CurrentPlayer + " rolls dice: ");
                int result = ludoEngine.TossDice();
                Console.WriteLine(result);
                Console.WriteLine("This means new current player is: Player " + ludoEngine.CurrentPlayer);
                Console.WriteLine();
            }            
        }

        private static void testSquares(ILudoEngine ludoEngine)
        {
            Square[] squares = ludoEngine.Squares;
            for(int i = 0; i < squares.Length; i++)
            {
                Console.WriteLine((i + 1) + ": " + squares[i].TokenColor);
            }            
        }

        private static void testHomeYards(ILudoEngine ludoEngine)
        {
            HomeYard[] homeYards = ludoEngine.HomeYards;
            for(int i = 0; i < homeYards.Length; i++)
            {
                Console.WriteLine("Home yard " + (i + 1) + ": " + homeYards[i].TokenColor);
                Console.WriteLine("Is full: " + homeYards[i].IsFull());
            }
        }

        private static void testMoveOut(ILudoEngine ludoEngine)
        {
            while(true)
            {
                int result;
                do
                {
                    result = ludoEngine.TossDice();
                } while (result != 6);
                Console.WriteLine("Player " + ludoEngine.CurrentPlayer + " finally got a 6");
                Console.WriteLine("Before moving out: " + ludoEngine.Squares[(ludoEngine.CurrentPlayer - 1) * 10].Occupied);
                Console.WriteLine("Moving out token...");
                ludoEngine.MoveOutToken();
                Console.WriteLine("After moving out: " + ludoEngine.Squares[(ludoEngine.CurrentPlayer - 1) * 10].Occupied);
                Console.ReadKey();
            }
            

            //int currentPlayer = ludoEngine.CurrentPlayer;
            //Console.WriteLine("Player " + ludoEngine.CurrentPlayer + "'s start position occupied: " + ludoEngine.Squares[i*10].Occupied);
            
            //    Console.WriteLine("Moving out player " + ludoEngine.CurrentPlayer + "'s token");
            //    ludoEngine.MoveOutToken();
            
            //Console.WriteLine("Player " + ludoEngine.CurrentPlayer + "'s start position occupied: " + ludoEngine.Squares[i*10].Occupied);
            //Console.WriteLine("It is now player " + ludoEngine.CurrentPlayer + "'s turn");
            //Console.WriteLine();            
        }
    }
}
