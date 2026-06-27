using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace ChessC
{
    partial class ChessUI
    {
        [DllImport("C:\\Users\\kzibs\\Desktop\\repos\\chess_c\\ProgramC\\ChessProgram.dll", CallingConvention = CallingConvention.Cdecl)] static extern Moves MovesToPlay(int x, int y);
        [DllImport("C:\\Users\\kzibs\\Desktop\\repos\\chess_c\\ProgramC\\ChessProgram.dll", CallingConvention = CallingConvention.Cdecl)] static extern void CheckMovePlayed(int x, int y, int nx, int ny, bool isTake);
        [DllImport("C:\\Users\\kzibs\\Desktop\\repos\\chess_c\\ProgramC\\ChessProgram.dll", CallingConvention = CallingConvention.Cdecl)] static extern void Initializing();
        public Tuple<int[,], int[], char, int> MovesToPlayIn(int[] PiecePosition)
        {
            int x = PiecePosition[0];
            int y = PiecePosition[1];
            Moves PieceMoves = MovesToPlay(x, y);
            int[,] AllMoves = new int[PieceMoves.MovesNumber, 2];
            int j = 0;
            for(int i = 0; i < PieceMoves.MovesNumber;i++)
            {
                AllMoves[i, 0] = PieceMoves.ArrMoves[j++];
                AllMoves[i, 1] = PieceMoves.ArrMoves[j++];
            }
            int[] TakenMoves = new int[PieceMoves.TakenMovesNumber];
            j = 0;
            for (int i = 0; i < PieceMoves.TakenMovesNumber; i++)
            {
                TakenMoves[i] = PieceMoves.ArrTakenMoves[i];
            }
            int index = PieceMoves.PieceIndex;
            char color = PieceMoves.PieceColor;
            Tuple<int[,], int[], char, int> TuplePiece = new Tuple<int[,], int[], char, int>(AllMoves, TakenMoves, color, index);
            return TuplePiece;
        }
        public void CheckingMovePlayed(int[] WhereItWas, int[] WhereItIs, bool isTake)
        {
            int x = WhereItWas[0];
            int y = WhereItWas[1];
            int nx = WhereItIs[0];
            int ny = WhereItIs[1];
            CheckMovePlayed(x, y, nx, ny, isTake);
        }

        public void InitializingChess()
        {
            Initializing();
        }
    }

    [StructLayout(LayoutKind.Sequential)] struct Moves
    {
        public char PieceColor;
        public int PieceIndex;
        public int MovesNumber;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 54)] public int[] ArrMoves;
        public int TakenMovesNumber;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)] public int[] ArrTakenMoves;
    }
}
