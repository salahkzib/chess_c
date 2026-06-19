#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ChessProgram.h"

/*

indexing

x
7
^
|
0 ---> 7 y
*/

Player WPlayer;
Player BPlayer;
Piece WKing, WQueen, WRook1, WRook2, WBishop1, WBishop2, WKnight1, WKnight2, WPawn1, WPawn2, WPawn3, WPawn4, WPawn5, WPawn6, WPawn7, WPawn8;
Piece BKing, BQueen, BRook1, BRook2, BBishop1, BBishop2, BKnight1, BKnight2, BPawn1, BPawn2, BPawn3, BPawn4, BPawn5, BPawn6, BPawn7, BPawn8;
Piece DefaultPiece;
PtrPiece board[8][8];

EXPORT void Initializing(void) {

    DefaultPiece = (Piece){ false, false, true, 'E', {-1, -1}, 'd', 0, 0, 0 };

    WKing = (Piece){ false, false, true, 'K', {0, 4}, 'w', 1, 0, 0 };
    WQueen = (Piece){ false, false, true, 'Q', {0, 3}, 'w', 1, 0, 0 };
    WRook1 = (Piece){ false, false, true, 'R', {0, 0}, 'w', 1, 0, 0 };
    WRook2 = (Piece){ false, false, true, 'R', {0, 7}, 'w', 1, 0, 0 };
    WBishop1 = (Piece){ false, false, true, 'B', {0, 2}, 'w', 1, 0, 0 };
    WBishop2 = (Piece){ false, false, true, 'B', {0, 5}, 'w', 1, 0, 0 };
    WKnight1 = (Piece){ false, false, true, 'N', {0, 1}, 'w', 1, 0, 0 };
    WKnight2 = (Piece){ false, false, true, 'N', {0, 6}, 'w', 1, 0, 0 };
    WPawn1 = (Piece){ false, false, true, 'P', {1, 0}, 'w', 1, 0, 0 };
    WPawn2 = (Piece){ false, false, true, 'P', {1, 1}, 'w', 1, 0, 0 };
    WPawn3 = (Piece){ false, false, true, 'P', {1, 2}, 'w', 1, 0, 0 };
    WPawn4 = (Piece){ false, false, true, 'P', {1, 3}, 'w', 1, 0, 0 };
    WPawn5 = (Piece){ false, false, true, 'P', {1, 4}, 'w', 1, 0, 0 };
    WPawn6 = (Piece){ false, false, true, 'P', {1, 5}, 'w', 1, 0, 0 };
    WPawn7 = (Piece){ false, false, true, 'P', {1, 6}, 'w', 1, 0, 0 };
    WPawn8 = (Piece){ false, false, true, 'P', {1, 7}, 'w', 1, 0, 0 };

    WPlayer.tour = 1;

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

    BKing = (Piece){ false, false, true, 'K', {7, 4}, 'b', -1, 0, 0 };
    BQueen = (Piece){ false, false, true, 'Q', {7, 3}, 'b', -1, 0, 0 };
    BRook1 = (Piece){ false, false, true, 'R', {7, 0}, 'b', -1, 0, 0 };
    BRook2 = (Piece){ false, false, true, 'R', {7, 7}, 'b', -1, 0, 0 };
    BBishop1 = (Piece){ false, false, true, 'B', {7, 2}, 'b', -1, 0, 0 };
    BBishop2 = (Piece){ false, false, true, 'B', {7, 5}, 'b', -1, 0, 0 };
    BKnight1 = (Piece){ false, false, true, 'N', {7, 1}, 'b', -1, 0, 0 };
    BKnight2 = (Piece){ false, false, true, 'N', {7, 6}, 'b', -1, 0, 0 };
    BPawn1 = (Piece){ false, false, true, 'P', {6, 0}, 'b', -1, 0, 0 };
    BPawn2 = (Piece){ false, false, true, 'P', {6, 1}, 'b', -1, 0, 0 };
    BPawn3 = (Piece){ false, false, true, 'P', {6, 2}, 'b', -1, 0, 0 };
    BPawn4 = (Piece){ false, false, true, 'P', {6, 3}, 'b', -1, 0, 0 };
    BPawn5 = (Piece){ false, false, true, 'P', {6, 4}, 'b', -1, 0, 0 };
    BPawn6 = (Piece){ false, false, true, 'P', {6, 5}, 'b', -1, 0, 0 };
    BPawn7 = (Piece){ false, false, true, 'P', {6, 6}, 'b', -1, 0, 0 };
    BPawn8 = (Piece){ false, false, true, 'P', {6, 7}, 'b', -1, 0, 0 };

    BPlayer.tour = 0;

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

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = &DefaultPiece;
        }
    }
    for (int m = 0; m < 16; m++) {
        PtrPiece piece = &WPlayer.pieces[m];
        board[piece->position[0]][piece->position[1]] = piece;
    }
    for (int m = 0; m < 16; m++) {
        PtrPiece piece = &BPlayer.pieces[m];
        board[piece->position[0]][piece->position[1]] = piece;
    }
}

bool SafeSquare(int x, int y) {
    return true;
}
void CheckIsProtected(PtrPiece piece) {

}

int** PawnPossibleMoves(PtrPiece pawn)
{
    int PossibleMoves[4][2];
    int x = pawn->position[0];
    int y = pawn->position[1];
    int directions[4][2] = { {1, 0}, {2, 0}, {1, 1}, {1, -1} };
    int nx, ny;
    for (int i = 0; i < 4; i++) {
        nx = x + (directions[i][0] * pawn->side);
        ny = y + directions[i][1];
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            PossibleMoves[i][0] = nx;
            PossibleMoves[i][1] = ny;
        }
        else {
            PossibleMoves[i][0] = -1;
            PossibleMoves[i][1] = -1;
        }
    }
    int** arr = (int**)calloc(4, sizeof(int*));
    for (int i = 0; i < 4; i++) {
        arr[i] = (int*)calloc(2, sizeof(int));
        arr[i][0] = PossibleMoves[i][0];
        arr[i][1] = PossibleMoves[i][1];
    }
    return arr;
}
void PawnPlayedMoves(PtrPiece pawn)
{
    if (pawn->ability) {
        int** ArrPM = PawnPossibleMoves(pawn);
        int PossibleMoves[4][2];
        for (int i = 0; i < 4; i++) {
            PossibleMoves[i][0] = ArrPM[i][0];
            PossibleMoves[i][1] = ArrPM[i][1];
        }
        for (int o = 0; o < 4; o++) {
            free(ArrPM[o]);
        }
        free(ArrPM);
        PtrPiece piece;
        piece = board[PossibleMoves[0][0]][PossibleMoves[0][1]];
        if (PossibleMoves[0][0] != -1 && piece->type == 'E') {
            int index = pawn->numMoves++;
            pawn->moves[index][0] = PossibleMoves[0][0];
            pawn->moves[index][1] = PossibleMoves[0][1];
            if (PossibleMoves[1][0] != -1 && piece->type == 'E' && pawn->count == 0) {
                int index = pawn->numMoves++;
                pawn->moves[index][0] = PossibleMoves[1][0];
                pawn->moves[index][1] = PossibleMoves[1][1];
            }
        }
        char OppColor = (pawn->color == 'w') ? 'b' : 'w';
        piece = board[PossibleMoves[2][0]][PossibleMoves[2][1]];
        if (PossibleMoves[2][0] != -1 && piece->type == OppColor) {
            int index = pawn->numMoves++;
            pawn->moves[index][0] = PossibleMoves[2][0];
            pawn->moves[index][1] = PossibleMoves[2][1];
        }
        piece = board[PossibleMoves[3][0]][PossibleMoves[3][1]];
        if (PossibleMoves[3][0] != -1 && piece->type == OppColor) {
            int index = pawn->numMoves++;
            pawn->moves[index][0] = PossibleMoves[3][0];
            pawn->moves[index][1] = PossibleMoves[3][1];
        }
    }
}

TupleMoves BishopPossibleMoves(PtrPiece bishop, int* PtrInLength, int*** PtrArrMoves) {
    TupleMoves RPM;
    int x = bishop->position[0];
    int y = bishop->position[1];
    int directions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    int possibleMoves[14][2];
    int counts[4];
    int index = 0;
    int indexArr = 0;
    RPM.length = 0;
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int nx = x + dx;
        int ny = y + dy;
        counts[index] = 0;
        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            possibleMoves[indexArr][0] = nx;
            possibleMoves[indexArr][1] = ny;
            counts[index]++;
            indexArr++;
            nx += dx;
            ny += dy;
        }
        if (counts[index] > 0) {
            index++;
        }
    }
    RPM.length = index;
    PtrInLength = (int*)calloc(index, sizeof(int));
    PtrArrMoves = (int***)calloc(index, sizeof(int**));
    int g = 0;
    for (int i = 0; i < index; i++) {
        PtrArrMoves[i] = (int**)calloc(counts[i], sizeof(int*));
        PtrInLength[i] = counts[i];
        for (int j = 0; j < counts[i]; j++) {
            PtrArrMoves[i][j] = (int*)calloc(2, sizeof(int));
            PtrArrMoves[i][j][0] = possibleMoves[g][0];
            PtrArrMoves[i][j][1] = possibleMoves[g][1];
            g++;
        }
    }
    RPM.InLength = PtrInLength;
    RPM.ArrMoves = PtrArrMoves;
    return RPM;
}
void BishopPlayedMoves(PtrPiece bishop) {
    if (bishop->ability) {
        int* InLength;
        int*** Arr;
        TupleMoves PossibleMoves = BishopPossibleMoves(bishop, InLength, Arr);
        bishop->numMoves = 0;
        char OppColor = (bishop->color == 'w') ? 'b' : 'w';
        int x, y, h, index;
        index = 0;
        for (int g = 0; g < PossibleMoves.length; g++) {
            h = 0;
            x = PossibleMoves.ArrMoves[g][h][0];
            y = PossibleMoves.ArrMoves[g][h][1];
            for (int h = 0; h < PossibleMoves.InLength[g]; h++)
            {
                x = PossibleMoves.ArrMoves[g][h][0];
                y = PossibleMoves.ArrMoves[g][h][1];
                if (board[x][y]->type == 'E') {
                    index = bishop->numMoves++;
                    bishop->moves[index][0] = x;
                    bishop->moves[index][1] = y;
                }
                else {
                    h = PossibleMoves.InLength[g];
                }
            }
            if (board[x][y]->color == OppColor)
            {
                index = bishop->numMoves++;
                bishop->moves[index][0] = x;
                bishop->moves[index][1] = y;
            }
        }
        for (int v = 0; v < PossibleMoves.length; v++)
        {
            for (int w = 0; w < PossibleMoves.InLength[v]; w++)
            {
                free(PossibleMoves.ArrMoves[v][w]);
            }
            free(PossibleMoves.ArrMoves[v]);
        }
        free(PossibleMoves.ArrMoves);
        free(PossibleMoves.InLength);
    }
}

int** KingPossibleMoves(PtrPiece king) {
    int x = king->position[0];
    int y = king->position[1];
    int PossibleMoves[8][2];
    int directions[8][2] = { {1, 1}, {1, 0}, {1, -1}, {0, -1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };
    int nx, ny;
    for (int i = 0; i < 8; i++) {
        nx = x + directions[i][0];
        ny = y + directions[i][1];
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            PossibleMoves[i][0] = nx;
            PossibleMoves[i][1] = ny;
        }
        else {
            PossibleMoves[i][0] = -1;
            PossibleMoves[i][1] = -1;
        }
    }
    int** arr = (int**)calloc(8, sizeof(int*));
    for (int i = 0; i < 8; i++) {
        arr[i] = (int*)calloc(2, sizeof(int));
        arr[i][0] = PossibleMoves[i][0];
        arr[i][1] = PossibleMoves[i][1];
    }
    return arr;
}
void KingPlayedMoves(PtrPiece king) {
    int InitialMoves[8][2];
    int len = 0;
    int index = 0;
    king->numMoves = 0;
    int** PossibleMoves = KingPossibleMoves(king);
    char OppColor = (king->color == 'w') ? 'b' : 'w';
    int x, y;
    for (int i = 0; i < 8; i++) {
        x = PossibleMoves[i][0];
        y = PossibleMoves[i][1];
        if (x != -1) {
            if (board[x][y]->type == 'E' && SafeSquare(x, y)) {
                king->moves[index][0] = x;
                king->moves[index][1] = y;
                index++;
            }
            else if (board[x][y]->color == OppColor) {
                CheckIsProtected(board[x][y]);
                if (!(board[x][y]->isProtected)) {
                    king->moves[index][0] = x;
                    king->moves[index][1] = y;
                    index++;
                }
            }
        }
    }
    king->numMoves = index;
    for (int out = 0; out < 8; out++) {
        free(PossibleMoves[out]);
    }
    free(PossibleMoves);
}

TupleMoves QueenPossibleMoves(PtrPiece queen, int* PtrInLength, int*** PtrArrMoves) {
    TupleMoves RPM;
    int x = queen->position[0];
    int y = queen->position[1];
    int directions[8][2] = { {0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    int possibleMoves[27][2];
    int counts[8];
    int index = 0;
    int indexArr = 0;
    RPM.length = 0;
    for (int d = 0; d < 8; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int nx = x + dx;
        int ny = y + dy;
        counts[index] = 0;
        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            possibleMoves[indexArr][0] = nx;
            possibleMoves[indexArr][1] = ny;
            counts[index]++;
            indexArr++;
            nx += dx;
            ny += dy;
        }
        if (counts[index] > 0) {
            index++;
        }
    }
    RPM.length = index;
    PtrInLength = (int*)calloc(index, sizeof(int));
    PtrArrMoves = (int***)calloc(index, sizeof(int**));
    int g = 0;
    for (int i = 0; i < index; i++) {
        PtrArrMoves[i] = (int**)calloc(counts[i], sizeof(int*));
        PtrInLength[i] = counts[i];
        for (int j = 0; j < counts[i]; j++) {
            PtrArrMoves[i][j] = (int*)calloc(2, sizeof(int));
            PtrArrMoves[i][j][0] = possibleMoves[g][0];
            PtrArrMoves[i][j][1] = possibleMoves[g][1];
            g++;
        }
    }
    RPM.InLength = PtrInLength;
    RPM.ArrMoves = PtrArrMoves;
    return RPM;
}
void QueenPlayedMoves(PtrPiece queen) {
    if (queen->ability) {
        int* InLength;
        int*** Arr;
        TupleMoves PossibleMoves = QueenPossibleMoves(queen, InLength, Arr);
        queen->numMoves = 0;
        char OppColor = (queen->color == 'w') ? 'b' : 'w';
        int x, y, h, index;
        index = 0;
        for (int g = 0; g < PossibleMoves.length; g++) {
            for (int h = 0; h < PossibleMoves.InLength[g]; h++)
            {
                x = PossibleMoves.ArrMoves[g][h][0];
                y = PossibleMoves.ArrMoves[g][h][1];
                if (board[x][y]->type == 'E') {
                    index = queen->numMoves++;
                    queen->moves[index][0] = x;
                    queen->moves[index][1] = y;
                }
                else {
                    h = PossibleMoves.InLength[g];
                }
            }
            if (board[x][y]->color == OppColor)
            {
                index = queen->numMoves++;
                queen->moves[index][0] = x;
                queen->moves[index][1] = y;
            }
        }
        for (int v = 0; v < PossibleMoves.length; v++)
        {
            for (int w = 0; w < PossibleMoves.InLength[v]; w++)
            {
                free(PossibleMoves.ArrMoves[v][w]);
            }
            free(PossibleMoves.ArrMoves[v]);
        }
        free(PossibleMoves.ArrMoves);
        free(PossibleMoves.InLength);
    }
}

TupleMoves RookPossibleMoves(PtrPiece rook, int* PtrInLength, int*** PtrArrMoves) {
    TupleMoves RPM;
    int x = rook->position[0];
    int y = rook->position[1];
    int directions[4][2] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
    int possibleMoves[14][2];
    int counts[4];
    int index = 0;
    int indexArr = 0;
    RPM.length = 0;
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int nx = x + dx;
        int ny = y + dy;
        counts[index] = 0;
        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            possibleMoves[indexArr][0] = nx;
            possibleMoves[indexArr][1] = ny;
            counts[index]++;
            indexArr++;
            nx += dx;
            ny += dy;
        }
        if (counts[index] > 0) {
            index++;
        }
    }
    RPM.length = index;
    PtrInLength = (int*)calloc(index, sizeof(int));
    PtrArrMoves = (int***)calloc(index, sizeof(int**));
    int g = 0;
    for (int i = 0; i < index; i++) {
        PtrArrMoves[i] = (int**)calloc(counts[i], sizeof(int*));
        PtrInLength[i] = counts[i];
        for (int j = 0; j < counts[i]; j++) {
            PtrArrMoves[i][j] = (int*)calloc(2, sizeof(int));
            PtrArrMoves[i][j][0] = possibleMoves[g][0];
            PtrArrMoves[i][j][1] = possibleMoves[g][1];
            g++;
        }
    }
    RPM.InLength = PtrInLength;
    RPM.ArrMoves = PtrArrMoves;
    return RPM;
}
void RookPlayedMoves(PtrPiece rook) {
    if (rook->ability) {
        int* InLength;
        int*** Arr;
        TupleMoves PossibleMoves = RookPossibleMoves(rook, InLength, Arr);
        rook->numMoves = 0;
        char OppColor = (rook->color == 'w') ? 'b' : 'w';
        int x, y, h, index;
        index = 0;
        for (int g = 0; g < PossibleMoves.length; g++) {
            for (int h = 0; h < PossibleMoves.InLength[g]; h++)
            {
                x = PossibleMoves.ArrMoves[g][h][0];
                y = PossibleMoves.ArrMoves[g][h][1];
                if (board[x][y]->type == 'E') {
                    index = rook->numMoves++;
                    rook->moves[index][0] = x;
                    rook->moves[index][1] = y;
                }
                else {
                    h = PossibleMoves.InLength[g];
                }
            }
            if (board[x][y]->color == OppColor)
            {
                index = rook->numMoves++;
                rook->moves[index][0] = x;
                rook->moves[index][1] = y;
            }
        }
        for (int v = 0; v < PossibleMoves.length; v++)
        {
            for (int w = 0; w < PossibleMoves.InLength[v]; w++)
            {
                free(PossibleMoves.ArrMoves[v][w]);
            }
            free(PossibleMoves.ArrMoves[v]);
        }
        free(PossibleMoves.ArrMoves);
        free(PossibleMoves.InLength);
    }
}

int** KnightPossibleMoves(PtrPiece knight) {
    int x = knight->position[0];
    int y = knight->position[1];
    int PossibleMoves[8][2];
    int directions[8][2] = { {2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2} };
    int nx, ny;
    int j = 0;
    for (int i = 0; i < 8; i++) {
        nx = x + directions[i][0];
        ny = y + directions[i][1];
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            PossibleMoves[j][0] = nx;
            PossibleMoves[j][1] = ny;
            j++;
        }
    }
    while (j < 8) {
        PossibleMoves[j][0] = -1;
        PossibleMoves[j][1] = -1;
        j++;
    }
    int** arr = (int**)calloc(8, sizeof(int*));
    for (int i = 0; i < 8; i++) {
        arr[i] = (int*)calloc(2, sizeof(int));
        arr[i][0] = PossibleMoves[i][0];
        arr[i][1] = PossibleMoves[i][1];
    }
    return arr;
}
void KnightPlayedMoves(PtrPiece knight) {
    if (knight->ability) {
        int** ArrPM = KnightPossibleMoves(knight);
        int PossibleMoves[8][2];
        int len = 0;
        while (len < 8) {
            if (ArrPM[len][0] != -1) {
                PossibleMoves[len][0] = ArrPM[len][0];
                PossibleMoves[len][1] = ArrPM[len][1];
            }
            else {
                break;
            }
            len++;
        }
        for (int o = 0; o < 8; o++) {
            free(ArrPM[o]);
        }
        free(ArrPM);
        char OppColor = (knight->color == 'w') ? 'b' : 'w';
        int x, y;
        for (int i = 0; i < len; i++) {
            x = PossibleMoves[i][0];
            y = PossibleMoves[i][1];
            if (board[x][y]->color == 'd' || board[x][y]->color == OppColor) {
                int index = knight->numMoves;
                knight->moves[index][0] = x;
                knight->moves[index][1] = y;
                knight->numMoves++;
            }
        }
    }
}

EXPORT Moves MovesToPlay(int x, int y) {
    PtrPiece piece = board[x][y];
    switch (piece->color) {
    case 'w':
        if (WPlayer.tour == 0) {
            Moves EmptyMoves = { 0 };
            return EmptyMoves;
        }
        break;
    case 'b':
        if (BPlayer.tour == 0) {
            Moves EmptyMoves = { 0 };
            return EmptyMoves;
        }
        break;
    default:
        Moves EmptyMoves = { 0 };
        return EmptyMoves;
        break;
    }
    Moves moves;
    switch (piece->type)
    {
    case 'P':
        PawnPlayedMoves(piece);
        break;
    case 'N':
        KnightPlayedMoves(piece);
        break;
    case 'B':
        BishopPlayedMoves(piece);
        break;
    case 'Q':
        QueenPlayedMoves(piece);
        break;
    case 'R':
        RookPlayedMoves(piece);
        break;
    case 'K':
        KingPlayedMoves(piece);
        break;
    default:
        break;
    }
    moves.MovesNumber = piece->numMoves;
    for (int i = 0; i < 27; i++) {
        for (int j = 0; j < 2; j++) {
            moves.MovesToP[i][j] = 0;
        }
    }
    for (int i = 0; i < piece->numMoves; i++) {
        moves.MovesToP[i][0] = piece->moves[i][0];
        moves.MovesToP[i][1] = piece->moves[i][1];
    }
    return moves;
}

EXPORT void CheckMovePlayed(int x, int y, int nx, int ny) {
    PtrPiece selected_piece = board[x][y];
    board[nx][ny] = selected_piece;
    board[x][y] = &DefaultPiece;
    selected_piece->position[0] = nx;
    selected_piece->position[1] = ny;
    selected_piece->count++;
    for (int i = 0; i < selected_piece->numMoves; i++) {
        selected_piece->moves[i][0] = 0;
        selected_piece->moves[i][1] = 0;
    }
    selected_piece->numMoves = 0;
    if (selected_piece->color == 'w') {
        WPlayer.tour = 0;
        BPlayer.tour = 1;
    }
    else {
        WPlayer.tour = 1;
        BPlayer.tour = 0;
    }
}
