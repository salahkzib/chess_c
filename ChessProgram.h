#ifndef CHESSPROGRAM_H
#define CHESSPROGRAM_H
#define EXPORT __declspec(dllexport)

typedef struct _Piece {
    bool isProtected; // protected by another piece or not (true or false)
    bool ability; // can move or not (true or false)
    char type; // K, Q, R, B, N, P, E
    int position[2]; // {y, x}
    char color; // w, b, d(for default piece)
    int side; // 1 or -1 or 0(for default piece)
    int numMoves; // number of moves that can play
    int count; // number of moves played by the piece
    int moves[27][2]; // the moves that can play
}Piece, * PtrPiece;


typedef struct _Player
{
    Piece pieces[16]; // array of the current pieces of the player
}Player, * PtrPlayer;

typedef struct _TupleMoves {
    int length;
    int* ArrMoves;
}TupleMoves, * PtrTupleMoves;

Player WPlayer;
Player BPlayer;
Piece WKing, WQueen, WRook1, WRook2, WBishop1, WBishop2, WKnight1, WKnight2, WPawn1, WPawn2, WPawn3, WPawn4, WPawn5, WPawn6, WPawn7, WPawn8;
Piece BKing, BQueen, BRook1, BRook2, BBishop1, BBishop2, BKnight1, BKnight2, BPawn1, BPawn2, BPawn3, BPawn4, BPawn5, BPawn6, BPawn7, BPawn8;
Piece DefaultPiece;
PtrPiece board[8][8];

EXPORT void Initializing(void);
int* PawnPossibleMoves(PtrPiece pawn);
void PawnPlayedMoves(PtrPiece pawn);
int* KingPossibleMoves(PtrPiece king);
void KingPlayedMoves(PtrPiece king);
int* QueenPossibleMoves(PtrPiece queen);
void QueenPlayedMoves(PtrPiece queen);
int* RookPossibleMoves(PtrPiece rook);
void RookPlayedMoves(PtrPiece rook);
int* BishoPossibleMoves(PtrPiece bishop);
void BishopPlayedMoves(PtrPiece bishop);
int* NightPossibleMoves(PtrPiece night);
void NightPlayedMoves(PtrPiece night);
EXPORT TupleMoves MovesToPlay(PtrPiece piece);
EXPORT void CheckMovePlayed(PtrPiece piece, int move[2]);


#endif