using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace ChessC
{
    partial class ChessUI
    {
        [DllImport("C:\\Users\\kzibs\\Desktop\\repos\\chess_c\\ProgramC\\ChessProgram.dll", CallingConvention = CallingConvention.Cdecl)] static extern Moves MovesToPlay(int x, int y);
        [DllImport("C:\\Users\\kzibs\\Desktop\\repos\\chess_c\\ProgramC\\ChessProgram.dll", CallingConvention = CallingConvention.Cdecl)] static extern void CheckMovePlayed(int x, int y);
        [DllImport("C:\\Users\\kzibs\\Desktop\\repos\\chess_c\\ProgramC\\ChessProgram.dll", CallingConvention = CallingConvention.Cdecl)] static extern void Initializing();
        public int[,] MovesToPlayIn(int[] PiecePosition)
        {
            int x = PiecePosition[0];
            int y = PiecePosition[1];
            Moves PieceMoves = MovesToPlay(x, y);
            int[,] Moves = new int[PieceMoves.MovesNumber, 2];
            for(int i = 0; i < PieceMoves.MovesNumber;)
            {
                Moves[i, 0] = PieceMoves.ArrMoves[i++];
                Moves[i, 1] = PieceMoves.ArrMoves[i++];
            }
            return Moves;
        }
        public void CheckingMovePlayed(int[] WhereItWas, int[] WhereItIs)
        {
            int x = WhereItWas[0];
            int y = WhereItWas[1];
            CheckMovePlayed(x, y);
        }

        public void InitializingChess()
        {
            Initializing();
        }
    }

    [StructLayout(LayoutKind.Sequential)] struct Moves
    {
        public int MovesNumber;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 54)]  public int[] ArrMoves;
    }
}
