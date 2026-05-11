#include <stdio.h>
#include<stdlib.h>
#include "ChessProgram.h"

int main(){
    Initializing();
    TupleMoves ptr = BishopPossibleMoves(&BBishop1);
    int u[2] = {2, 5};
    printf("length : %d\n", ptr.length);
    printf("length2 : %d\n", ptr.InLength[0]);
    printf("length2 : %d\n", ptr.InLength[1]);
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < u[i];j++){
            printf("(%d, %d)\n",ptr.ArrMoves[i][j]);
        }
    }
    printf("done");
    for(int k = 0; k < 2; k++){
        free(ptr.ArrMoves[k]);
    }
    free(ptr.ArrMoves);
    return 0;
}