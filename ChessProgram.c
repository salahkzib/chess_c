#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ChessProgram.h"


//DefaultPiece = { false, 'E', {-1, -1}, 'd', 0 };


void Initializing(void)
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

    // Initialize board with default pieces
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board[i][j] = &DefaultPiece;
        }
    }
    // Place white pieces
    for(int p = 0; p < 16; p++) {
        Piece* piece = &WPlayer.pieces[p];
        if(piece->type != 'E') {
            board[piece->position[0]][piece->position[1]] = piece;
        }
    }
    // Place black pieces
    for(int p = 0; p < 16; p++) {
        Piece* piece = &BPlayer.pieces[p];
        if(piece->type != 'E') {
            board[piece->position[0]][piece->position[1]] = piece;
        }
    }
}

TupleMoves PawnPossibleMoves(PtrPiece pawn) {
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

    TupleMoves tupleMoves;

    return tupleMoves;
}
void PawnPlayedMoves(PtrPiece pawn) {
    TupleMoves PossibleMoves = PawnPossibleMoves(pawn);

}

TupleMoves KingPossibleMoves(PtrPiece king) {
    TupleMoves tupleMoves;

    return tupleMoves;
}
void KingPlayedMoves(PtrPiece king) {
    TupleMoves PossibleMoves = KingPossibleMoves(king);
}

TupleMoves QueenPossibleMoves(PtrPiece queen) {
    TupleMoves tupleMoves;

    return tupleMoves;
}
void QueenPlayedMoves(PtrPiece queen) {
    TupleMoves PossibleMoves = QueenPossibleMoves(queen);
}

TupleMoves RookPossibleMoves(PtrPiece rook) {
    TupleMoves tupleMoves;
    int x = rook->position[0];
    int y = rook->position[1];
    int directions[4][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    int possibleMoves[14][2]; 
    int count = 0;
    for(int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int nx = x + dx;
        int ny = y + dy;
        while(nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            possibleMoves[count][0] = nx;
            possibleMoves[count][1] = ny;
            count++;
            nx += dx;
            ny += dy;
        }
    }
    int **array = (int **)calloc(count, sizeof(int *));
    for (int i = 0; i < count; i++) {
        array[i] = (int *)calloc(2, sizeof(int));
    }
    for (int i = 0; i < count; i++)
    {
        array[i][0] = possibleMoves[i][0];
        array[i][1] = possibleMoves[i][1];
    }
    
    tupleMoves.length = count;
    tupleMoves.ArrMoves = array;
    
    return tupleMoves;
}
void RookPlayedMoves(PtrPiece rook) {
    TupleMoves PossibleMoves = RookPossibleMoves(rook);
}

TupleMoves BishopPossibleMoves(PtrPiece bishop) {//function that calculates the possible moves for a bishop piece on the chess board
    TupleMoves tupleMoves;
    int x = bishop->position[0];//x coordinate of the bishop
    int y = bishop->position[1];//y coordinate of the bishop
    int directions[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};//directions for the bishop to move (diagonal moves)
    int possibleMoves[14][2];
    int count = 0;//counter for the number of possible moves
    for(int d = 0; d < 4; d++) {//for each direction
        int dx = directions[d][0];//x coordinate change for the current direction
        int dy = directions[d][1];//y coordinate change for the current direction
        int nx = x + dx;//x coordinate of the next position
        int ny = y + dy;//y coordinate of the next position
        while(nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {//while the next position is within the board
            possibleMoves[count][0] = nx;//x coordinate of the possible move
            possibleMoves[count][1] = ny;//y coordinate of the possible move
            count++;
            nx += dx;//x coordinate of the next position in the current direction
            ny += dy;//y coordinate of the next position in the current direction
        }
    }
    int **array = (int **)calloc(count, sizeof(int *));
    for (int i = 0; i < count; i++) {
        array[i] = (int *)calloc(2, sizeof(int));
    }
    for (int i = 0; i < count; i++)
    {
        array[i][0] = possibleMoves[i][0];
        array[i][1] = possibleMoves[i][1];
    }
    
    tupleMoves.length = count;
    tupleMoves.ArrMoves = array;
    return tupleMoves;
}
void BishopPlayedMoves(PtrPiece bishop) {
    TupleMoves PossibleMoves = BishopPossibleMoves(bishop);
}

TupleMoves KnightPossibleMoves(PtrPiece knight) {
    TupleMoves tupleMoves;

    return tupleMoves;
}
void KnightPlayedMoves(PtrPiece knight) {
    TupleMoves PossibleMoves = KnightPossibleMoves(knight);
}

EXPORT TupleMoves MovesToPlay(PtrPiece piece) {
    TupleMoves tupleMoves;

    return tupleMoves;
}

EXPORT void CheckMovePlayed(PtrPiece piece, int move[2]) {

}



