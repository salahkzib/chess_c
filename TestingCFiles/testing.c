#include <stdio.h>
#include "ChessProgram.h"

int main() {
    Initializing();
    while (1) {
        int x, y;
        printf("enter x : ");
        scanf("%d", &x);
        printf("enter y : ");
        scanf("%d", &y);
        printf("%d, %d\n", x, y);
        if (x != -1 | y != -1) {
            Moves moves = MovesToPlay(x, y);
            for (int i = 0; i < moves.MovesNumber; i++) {
                printf("possible move : x = %d, y = %d\n", moves.MovesToP[i++], moves.MovesToP[i]);
            }
            for (int i = 0; i < moves.TakenMovesNumber; i++) {
                printf("move to take in index : %d\n", moves.TakenMoves[i]);
            }
            int nx, ny;
            printf("enter new x : ");
            scanf("%d", &nx);
            printf("enter new y : ");
            scanf("%d", &ny);
            if (board[nx][ny]->color == 'd') {
                CheckMovePlayed(x, y, nx, ny, false);
            }
            else {
                CheckMovePlayed(x, y, nx, ny, true);
            }
            for (int i = 0; i < 16; i++) {
                printf("white %c position is (%d, %d)\n", WPlayer.pieces[i].type, WPlayer.pieces[i].position[0], WPlayer.pieces[i].position[1]);
                printf("black %c position is (%d, %d)\n", BPlayer.pieces[i].type, BPlayer.pieces[i].position[0], BPlayer.pieces[i].position[1]);
            }
        }
    }


    return 0;
}