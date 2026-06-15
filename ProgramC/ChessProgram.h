#ifndef CHESSPROGRAM_H
#define CHESSPROGRAM_H
#define EXPORT __declspec(dllexport)
#include <stdbool.h>

typedef struct _Piece {
    bool isProtected; // protected by another piece or not (true or false)
    bool ability; // can move or not (true or false)
    char type; // K, Q, R, B, N, P, E
    int position[2]; // {y, x}
    char color; // w, b, d(for default piece)
    int side; // 1 or -1 or 0(for default piece)
    int numMoves; // number of moves that can be played by the piece
    int count; // number of moves played by the piece
    int moves[27][2]; // the moves that can play
    void (*Movement)(struct _Piece* piece);
}Piece, * PtrPiece;

typedef struct {
    int x;
    int y;
}Position;

typedef struct _Player
{
    Piece pieces[16]; // array of the current pieces of the player
}Player, * PtrPlayer;

typedef struct _TupleMoves {
    int length;
    int* InLength;
    int*** ArrMoves;
}TupleMoves, * PtrTupleMoves;

typedef struct _Moves
{
    int MovesNumber;
    int MovesToP[27][2];
}Moves;


extern Player WPlayer;
extern Player BPlayer;
extern Piece WKing, WQueen, WRook1, WRook2, WBishop1, WBishop2, WKnight1, WKnight2, WPawn1, WPawn2, WPawn3, WPawn4, WPawn5, WPawn6, WPawn7, WPawn8;
extern Piece BKing, BQueen, BRook1, BRook2, BBishop1, BBishop2, BKnight1, BKnight2, BPawn1, BPawn2, BPawn3, BPawn4, BPawn5, BPawn6, BPawn7, BPawn8;
extern Piece DefaultPiece;
extern PtrPiece board[8][8];
extern int SelectedPieceIndex[2];

EXPORT void Initializing(void);
bool SafeSquare(int x, int y);
void CheckIsProtected(PtrPiece piece);
int** PawnPossibleMoves(PtrPiece pawn);
void PawnPlayedMoves(PtrPiece pawn);
TupleMoves BishopPossibleMoves(PtrPiece bishop, int* PtrInLength, int*** PtrArrMoves);
void BishopPlayedMoves(PtrPiece bishop);
int** KingPossibleMoves(PtrPiece king);
void KingPlayedMoves(PtrPiece king);
TupleMoves QueenPossibleMoves(PtrPiece queen, int* PtrInLength, int*** PtrArrMoves);
void QueenPlayedMoves(PtrPiece queen);
TupleMoves RookPossibleMoves(PtrPiece rook, int* PtrInLength, int*** PtrArrMoves);
void RookPlayedMoves(PtrPiece rook);
int** KnightPossibleMoves(PtrPiece knight);
void KnightPlayedMoves(PtrPiece knight);
EXPORT Moves MovesToPlay(int x, int y);
EXPORT void CheckMovePlayed(int x, int y);

#endif