#include<stdio.h>
#include"ChessProgram.h"

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
        int** array = (int **)calloc(counts[i], sizeof(int *));
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