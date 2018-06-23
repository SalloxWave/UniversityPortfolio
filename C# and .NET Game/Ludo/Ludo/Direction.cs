using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ludo
{
    public class Direction
    {
        private string _dir;
        public Direction(string dir)
        {
            _dir = dir;
        }

        //public static readonly Direction
        //    right = new Direction("right");

        public void Step(ref int x, ref int y, int stepCount = 1)
        {
            switch(_dir)
            {
                case "left":
                    x -= stepCount;
                    break;
                case "up":
                    y -= stepCount;
                    break;
                case "right":
                    x += stepCount;
                    break;
                case "down":
                    y += stepCount;
                    break;
            }
        }

        public void Step(ref double x, ref double y, double stepCount = 1)
        {
            switch (_dir)
            {
                case "left":
                    x -= stepCount;
                    break;
                case "up":
                    y -= stepCount;
                    break;
                case "right":
                    x += stepCount;
                    break;
                case "down":
                    y += stepCount;
                    break;
            }
        }

        public Direction Reverse()
        {
            switch (_dir)
            {
                case "left":
                    return new Direction("right");                    
                case "up":
                    return new Direction("down");
                case "right":
                    return new Direction("left");
                case "down":
                    return new Direction("up");
            }
            return this;
        }
    }
}
