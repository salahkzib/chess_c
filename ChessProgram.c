#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ChessProgram.h"


DefaultPiece = { false, 'E', {-1, -1}, 'd', 0 };


EXPORT void Initializing(void)
{
    WKing = (Piece){ false, true, 'K', {0, 4}, 'w', 1, 0, 0 };
    WQueen = (Piece){ false, true, 'Q', {0, 3}, 'w', 1, 0, 0 };
    WRook1 = (Piece){ false, true, 'R', {0, 0}, 'w', 1, 0, 0 };
    WRook2 = (Piece){ false, true, 'R', {0, 7}, 'w', 1, 0, 0 };
    WBishop1 = (Piece){ false, true, 'B', {0, 2}, 'w', 1, 0, 0 };
    WBishop2 = (Piece){ false, true, 'B', {0, 5}, 'w', 1, 0, 0 };
    WKnight1 = (Piece){ false, true, 'N', {0, 1}, 'w', 1, 0, 0 };
    WKnight2 = (Piece){ false, true, 'N', {0, 6}, 'w', 1, 0, 0 };
    WPawn1 = (Piece){ false, true, 'P', {1, 0}, 'w', 1, 0, 0 };
    WPawn2 = (Piece){ false, true, 'P', {1, 1}, 'w', 1, 0, 0 };
    WPawn3 = (Piece){ false, true, 'P', {1, 2}, 'w', 1, 0, 0 };
    WPawn4 = (Piece){ false, true, 'P', {1, 3}, 'w', 1, 0, 0 };
    WPawn5 = (Piece){ false, true, 'P', {1, 4}, 'w', 1, 0, 0 };
    WPawn6 = (Piece){ false, true, 'P', {1, 5}, 'w', 1, 0, 0 };
    WPawn7 = (Piece){ false, true, 'P', {1, 6}, 'w', 1, 0, 0 };
    WPawn8 = (Piece){ false, true, 'P', {1, 7}, 'w', 1, 0, 0 };

    WPlayer.pieces[0] = WKing;
    WPlayer.pieces[1] = WQueen;
    WPlayer.pieces[2] = WRook1;
    WPlayer.pieces[3] = WRook2;
    WPlayer.pieces[4] = WBishop1;
    WPlayer.pieces[5] = WBishop2;
    WPlayer.pieces[6] = WKnight1;
    WPlayer.pieces[7] = WKnight2;
    WPlayer.pieces[8] = WPawn1;
    WPlayer.pieces[9] = WPawn2;
    WPlayer.pieces[10] = WPawn3;
    WPlayer.pieces[11] = WPawn4;
    WPlayer.pieces[12] = WPawn5;
    WPlayer.pieces[13] = WPawn6;
    WPlayer.pieces[14] = WPawn7;
    WPlayer.pieces[15] = WPawn8;

    BKing = (Piece){ false, true, 'K', {7, 4}, 'b', -1, 0, 0 };
    BQueen = (Piece){ false, true, 'Q', {7, 3}, 'b', -1, 0, 0 };
    BRook1 = (Piece){ false, true, 'R', {7, 0}, 'b', -1, 0, 0 };
    BRook2 = (Piece){ false, true, 'R', {7, 7}, 'b', -1, 0, 0 };
    BBishop1 = (Piece){ false, true, 'B', {7, 2}, 'b', -1, 0, 0 };
    BBishop2 = (Piece){ false, true, 'B', {7, 5}, 'b', -1, 0, 0 };
    BKnight1 = (Piece){ false, true, 'N', {7, 1}, 'b', -1, 0, 0 };
    BKnight2 = (Piece){ false, true, 'N', {7, 6}, 'b', -1, 0, 0 };
    BPawn1 = (Piece){ false, true, 'P', {6, 0}, 'b', -1, 0, 0 };
    BPawn2 = (Piece){ false, true, 'P', {6, 1}, 'b', -1, 0, 0 };
    BPawn3 = (Piece){ false, true, 'P', {6, 2}, 'b', -1, 0, 0 };
    BPawn4 = (Piece){ false, true, 'P', {6, 3}, 'b', -1, 0, 0 };
    BPawn5 = (Piece){ false, true, 'P', {6, 4}, 'b', -1, 0, 0 };
    BPawn6 = (Piece){ false, true, 'P', {6, 5}, 'b', -1, 0, 0 };
    BPawn7 = (Piece){ false, true, 'P', {6, 6}, 'b', -1, 0, 0 };
    BPawn8 = (Piece){ false, true, 'P', {6, 7}, 'b', -1, 0, 0 };

    BPlayer.pieces[0] = BKing;
    BPlayer.pieces[1] = BQueen;
    BPlayer.pieces[2] = BRook1;
    BPlayer.pieces[3] = BRook2;
    BPlayer.pieces[4] = BBishop1;
    BPlayer.pieces[5] = BBishop2;
    BPlayer.pieces[6] = BKnight1;
    BPlayer.pieces[7] = BKnight2;
    BPlayer.pieces[8] = BPawn1;
    BPlayer.pieces[9] = BPawn2;
    BPlayer.pieces[10] = BPawn3;
    BPlayer.pieces[11] = BPawn4;
    BPlayer.pieces[12] = BPawn5;
    BPlayer.pieces[13] = BPawn6;
    BPlayer.pieces[14] = BPawn7;
    BPlayer.pieces[15] = BPawn8;
}

int* PawnPossibleMoves(PtrPiece pawn) {
    //possible moves
    //switch board
    //int num = 1, -1 (color pawn s)
    /*
    if(!ability){
    return NULL
    }
    int index = pawn.index; //{x, y}
    side = pawn.Side;
    x, y;
    count = pawn.count;
    l[][] = {{{0, num},true{piece}}, {0, 2*num}//condition count == 0, {1, num}, {-1, num} // condition en passant}
    List pm[][] = {};
    int ArrayUsed[2];
    for(int i = 0; i < 4; i++){
    ArrayUsed = {l[i][0] + index[0]}
    if(ArrayUsed >= 0 and < 8 )

    }
    promoting
    jjjjjjjj





    iiiiiiii
    */

    int* PossibleMoves;

    return PossibleMoves;
}
void PawnPlayedMoves(PtrPiece pawn) {
    int* PossibleMoves = PawnPossibleMoves(pawn);

}

int* KingPossibleMoves(PtrPiece king) {
    int* PossibleMoves;

    return PossibleMoves;
}
void KingPlayedMoves(PtrPiece king) {
    int* PossibleMoves = KingPossibleMoves(king);
}

int* QueenPossibleMoves(PtrPiece queen) {
    int* PossibleMoves;

    return PossibleMoves;
}
void QueenPlayedMoves(PtrPiece queen) {
    int* PossibleMoves = QueenPossibleMoves(queen);
}

int* RookPossibleMoves(PtrPiece rook) {
    int* PossibleMoves;

    return PossibleMoves;
}
void RookPlayedMoves(PtrPiece rook) {
    int* PossibleMoves = RookPossibleMoves(rook);
}

int* BishopPossibleMoves(PtrPiece bishop) {
    int* PossibleMoves;

    return PossibleMoves;
}
void BishopPlayedMoves(PtrPiece bishop) {
    int* PossibleMoves = BishoPossibleMoves(bishop);
}

int* NightPossibleMoves(PtrPiece night) {
    int* PossibleMoves;

    return PossibleMoves;
}
void NightPlayedMoves(PtrPiece night) {
    int* PossibleMoves = NightPossibleMoves(night);
}

EXPORT TupleMoves MovesToPlay(PtrPiece piece) {
    TupleMoves tupleMoves;





    tupleMoves.length = piece->numMoves;
    tupleMoves.ArrMoves = piece->moves;
    return tupleMoves;
}

EXPORT void CheckMovePlayed(PtrPiece piece, int move[2]) {

}

