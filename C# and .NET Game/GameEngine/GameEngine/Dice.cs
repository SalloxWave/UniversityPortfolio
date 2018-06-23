using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LudoEngine
{
    public class Dice
    {
        private int _sides;
        private Random random;

        public Dice(int sides = 6)
        {
            _sides = sides;
            random = new Random();
        }

        public int Toss()
        {            
            int result = random.Next(1, _sides + 1);            
            return result;
        }        
    }
}
