using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Ludo.Engine;

namespace Ludo.Data
{
    public interface ILudoData
    {
        void SaveState(ILudoEngine ludoEngine);
        bool Exists { get; }

        int PlayerCount { get; }
        int CurrentPlayer { get; }
        bool DiceTossed { get; }
        bool GameOver { get; }
        int[] Winners { get; }
        Square[] Squares { get; }
        HomeYard[] HomeYards { get; }        
        Player[] Players { get; }
        List<int> DiceHistory { get; }        
    }
}
