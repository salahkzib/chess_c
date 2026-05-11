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

void Initializing(void)
{
    DefaultPiece = (Piece){ false, true, 'E', {-1, -1}, 'd', 0, 0, 0 };
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

	for (int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
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

int** PawnPossibleMoves(PtrPiece pawn) {
    int PossibleMoves[4][2];
	int x = pawn->position[0];
	int y = pawn->position[1];
    int len = 0;
    int directions[4][2] = { {1, 0}, {2, 0}, {1, 1}, {1, -1} };
    int nx, ny;
    for (int i = 0; i < 4; i++) {
		nx = x + (directions[i][0] * pawn->side);
		ny = y + directions[i][1];
		if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
			PossibleMoves[len][0] = nx;
			PossibleMoves[len][1] = ny;
            len++;
        }
        else
        {
            PossibleMoves[len][0] = -1;
            PossibleMoves[len][1] = -1;
            len++;
        }
    }
    int** arr = (int **)calloc(len, sizeof(int *));
    for (int i = 0; i < len; i++) {
        arr[i] = (int *)calloc(2, sizeof(int));
		arr[i][0] = PossibleMoves[i][0];
		arr[i][1] = PossibleMoves[i][1];
	}
    return arr;
}
void PawnPlayedMoves(PtrPiece pawn) {
    if (pawn->ability) {
		int **ArrPM = PawnPossibleMoves(pawn);
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
        if (PossibleMoves[0][0] != -1 && piece->type != 'E') {
            int index = pawn->numMoves++;
            pawn->moves[index][0] = PossibleMoves[0][0];
            pawn->moves[index][1] = PossibleMoves[0][1];
        }
        piece = board[PossibleMoves[1][0]][PossibleMoves[1][1]];
        if (PossibleMoves[1][0] != -1 && piece->type != 'E' && piece->count == 0) {
            int index = pawn->numMoves++;
            pawn->moves[index][0] = PossibleMoves[1][0];
            pawn->moves[index][1] = PossibleMoves[1][1];
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

int** KingPossibleMoves(PtrPiece king) {
    int len = 0;
	int x = king->position[0];
    int y = king->position[1];
	int PossibleMoves[8][2];
    int directions[8][2] = { {1, 1}, {1, 0}, {1, -1}, {0, -1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };
    int nx, ny;
    for (int i = 0; i < 8; i++) {
        nx = x + directions[i][0];
        ny = y + directions[i][1];
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            PossibleMoves[len][0] = nx;
            PossibleMoves[len][1] = ny;
            len++;
        }
    }
    int** arr = (int**)calloc(len, sizeof(int*));
    for (int i = 0; i < len; i++) {
        arr[i] = (int*)calloc(2, sizeof(int));
        arr[i][0] = PossibleMoves[i][0];
        arr[i][1] = PossibleMoves[i][1];
    }
    return arr;
}
void KingPlayedMoves(PtrPiece king, int IndexPieceMoved) {
    int len1 = 0;
	int InitialMoves[8][2];
    int** PossibleMoves = KingPossibleMoves(king);
    int x, y, index;
    for (int i = 0; i < 8; i++) {
        x = PossibleMoves[i][0];
        y = PossibleMoves[i][1];
        if (board[x][y]->color != king->color) {
			index = king->numMoves++;
            InitialMoves[index][0] = x;
            InitialMoves[index][1] = y;
        }
	}
}

TupleMoves QueenPossibleMoves(PtrPiece queen) {
    TupleMoves RPM;
    int x = queen->position[0];
    int y = queen->position[1];
    int directions[8][2] = { {0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    int possibleMoves[27][2];
    int counts[8];
    int general_count = 0;
    int nx, ny;
    for (int d = 0; d < 8; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        nx = x + dx;
        ny = y + dy;
        counts[d] = 0;
        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            possibleMoves[counts[d]][0] = nx;
            possibleMoves[counts[d]][1] = ny;
            counts[d]++;
            nx += dx;
            ny += dy;
        }
        if (counts[d] != 0) {
            general_count++;
        }
    }
    RPM.length = general_count;
    RPM.InLength = (int*)calloc(general_count, sizeof(int));
    int*** array = (int***)calloc(general_count, sizeof(int**));
    for (int i = 0; i < general_count; i++) {
        int** array = (int**)calloc(counts[i], sizeof(int*));
        RPM.InLength[i] = counts[i];
		for (int j = 0; j < 2; j++) {
            array[i] = (int*)calloc(2, sizeof(int));
        }
    }
    int g = 0;
    for (int v = 0; v < general_count; v++)
    {
        for (int w = 0; w < counts[v]; w++)
        {
            array[v][w][0] = possibleMoves[g][0];
            array[v][w][1] = possibleMoves[g][1];
            g++;
        }
    }
    RPM.ArrMoves = array;
    return RPM;
}
void QueenPlayedMoves(PtrPiece queen) {
    if (queen->ability) {
        TupleMoves PossibleMoves = BishopPossibleMoves(queen);



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

TupleMoves RookPossibleMoves(PtrPiece rook) {
    TupleMoves RPM;
    int x = rook->position[0];
    int y = rook->position[1];
    int directions[4][2] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
    int possibleMoves[14][2];
    int counts[8];
    int general_count = 0;
	int nx, ny;
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        nx = x + dx;
        ny = y + dy;
        counts[d] = 0;
        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            possibleMoves[counts[d]][0] = nx;
            possibleMoves[counts[d]][1] = ny;
            counts[d]++;
            nx += dx;
            ny += dy;
        }
        if (counts[d] != 0) {
            general_count++;
        }
    }
    RPM.length = general_count;
    RPM.InLength = (int *)calloc(general_count, sizeof(int));
    int*** array = (int ***)calloc(general_count, sizeof(int **));
    for (int i = 0; i < general_count; i++) {
        int** array = (int **)calloc(counts[i], sizeof(int *));
        RPM.InLength[i] = counts[i];
        for (int j = 0; j < 2; j++) {
            array[i] = (int *)calloc(2, sizeof(int));
        }
    }
    int g = 0;
    for (int v = 0; v < general_count; v++)
    {
        for (int w = 0; w < counts[v]; w++)
        {
            array[v][w][0] = possibleMoves[g][0];
            array[v][w][1] = possibleMoves[g][1];
            g++;
        }
    }
    RPM.ArrMoves = array;
    return RPM;
}
void RookPlayedMoves(PtrPiece rook) {
    if (rook->ability) {
        TupleMoves PossibleMoves = BishopPossibleMoves(rook);



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

TupleMoves BishopPossibleMoves(PtrPiece bishop) {
    TupleMoves RPM;
    int x = bishop->position[0];
    int y = bishop->position[1];
    int directions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    int possibleMoves[14][2];
    int counts[8];
    int general_count = 0;
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int nx = x + dx;
        int ny = y + dy;
        counts[d] = 0;
        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            possibleMoves[counts[d]][0] = nx;
            possibleMoves[counts[d]][1] = ny;
            counts[d]++;
            nx += dx;
            ny += dy;
        }

        if (counts[d] != 0) {
            general_count++;
        }
    }
    RPM.length = general_count;
    RPM.InLength = (int *)calloc(general_count, sizeof(int));
    int*** array = (int ***)calloc(general_count, sizeof(int **));
    for (int i = 0; i < general_count; i++) {
        int** array = (int**)calloc(counts[i], sizeof(int *));
        RPM.InLength[i] = counts[i];
        for (int j = 0; j < 2; j++) {
            array[i] = (int*)calloc(2, sizeof(int));
        }
    }
    int g = 0;
    for (int v = 0; v < general_count; v++)
    {
        for (int w = 0; w < counts[v]; w++)
        {
            array[v][w][0] = possibleMoves[g][0];
            array[v][w][1] = possibleMoves[g][1];
            g++;
        }
    }
    RPM.ArrMoves = array;
    return RPM;
}
int** BishopPlayedMoves(PtrPiece bishop) {
    if (bishop->ability) {
        TupleMoves PossibleMoves = BishopPossibleMoves(bishop);
        bishop->numMoves = 0;
        int Arr[14][2];
        char OppColor = (bishop->color == 'w') ? 'b' : 'w';
        int x, y, h;
        for (int g = 0; g < PossibleMoves.length; g++) {
            h = 0;
            x = PossibleMoves.ArrMoves[g][h][0];
            y = PossibleMoves.ArrMoves[g][h][1];
            while (h < PossibleMoves.InLength[g] && board[x][y]->type == 'd') {
                Arr[bishop->numMoves++][0] = x;
                Arr[bishop->numMoves++][1] = y;
                h++;
                x = PossibleMoves.ArrMoves[g][h][0];
                y = PossibleMoves.ArrMoves[g][h][1];
            }
            if(board[x][y]->type == OppColor){
                Arr[bishop->numMoves++][0] = x;
                Arr[bishop->numMoves++][1] = y;
            }
        }
        /*
        bishop->moves = (int**)calloc(bishop->numMoves, sizeof(int*));
        for(int l = 0; l < bishop->numMoves; l++){
            bishop->moves[l] = (int*)calloc(2, sizeof(int));
            bishop->moves[l][0] = Arr[l][0];
            bishop->moves[l][1] = Arr[l][1];
        }
           */ 
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
        int **NA = (int **)calloc(bishop->numMoves, sizeof(int*));
        for(int k = 0; k < bishop->numMoves; k++){
            NA[k] = (int *)calloc(2, sizeof(int));
        }
        for(int i = 0; i < bishop->numMoves; i++){
            NA[i][0] = Arr[i][0];
            NA[i][1] = Arr[i][1];
        }
        return NA;
    }
}

int** KnightPossibleMoves(PtrPiece knight) {
    int len = 0;
    int x = knight->position[0];
    int y = knight->position[1];
    int PossibleMoves[8][2];
    int directions[8][2] = { {2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2} };
    int nx, ny;
    for (int i = 0; i < 8; i++) {
        nx = x + directions[i][0];
        ny = y + directions[i][1];
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            PossibleMoves[len][0] = nx;
            PossibleMoves[len][1] = ny;
            len++;
        }
    }
    int** arr = (int **)calloc(len, sizeof(int*));
    for (int i = 0; i < len; i++) {
        arr[i] = (int *)calloc(2, sizeof(int));
        arr[i][0] = PossibleMoves[i][0];
        arr[i][1] = PossibleMoves[i][1];
    }
    return arr;
}
void KnightPlayedMoves(PtrPiece knight) {
    if (knight->ability) {
        int** ArrPM = KnightPossibleMoves(knight);
        int PossibleMoves[8][2];
        for (int i = 0; i < 8; i++) {
            PossibleMoves[i][0] = ArrPM[i][0];
            PossibleMoves[i][1] = ArrPM[i][1];
        }
        for (int o = 0; o < 8; o++) {
            free(ArrPM[o]);
        }
        free(ArrPM);
		int len = sizeof(PossibleMoves) / sizeof(PossibleMoves[0]);
        for (int i = 0; i < len; i++) {
            int x = PossibleMoves[i][0];
            int y = PossibleMoves[i][1];
			char OppColor = (knight->color == 'w') ? 'b' : 'w';
            if (board[x][y]->type == 'd' || board[x][y]->type == OppColor) {
                int index = knight->numMoves++;
                knight->moves[index][0] = x;
				knight->moves[index][1] = y;
            }
        }
    }
}
/*
EXPORT int** MovesToPlay(Point position) {
    TupleMoves tst = BishopPossibleMoves(board[0][2]);
    return tst.ArrMoves;
}*/

EXPORT void CheckMovePlayed(PtrPiece piece, int *move) {
}

EXPORT void FreePointerDg1(int *ptr){
    free(ptr);
}
EXPORT void FreePointerDg2(int **ptr){
    free(ptr);
}
EXPORT void FreePointerDg3(int ***ptr){
    free(ptr);
}