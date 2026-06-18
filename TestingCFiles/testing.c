#include <stdio.h>
#include "ChessProgram.h"

int main(){
    Initializing();
    while(1){
        int x, y;
        printf("enter x : ");
        scanf("%d", &x);
        printf("enter y : ");
        scanf("%d", &y);
        printf("%d, %d\n", x, y);
        if(x!=-1 | y!= -1){
            Moves moves = MovesToPlay(x, y);
            for(int i = 0; i < moves.MovesNumber; i++) {
                printf("possible move %d : x = %d, y = %d\n", i + 1, moves.MovesToP[i][0], moves.MovesToP[i][1]);
            }
            int nx, ny;
            printf("enter new x : ");
            scanf("%d", &nx);
            printf("enter new y : ");
            scanf("%d", &ny);
            CheckMovePlayed(x, y, nx, ny);
            for(int i = 0; i < 16; i++){
                printf("white %c position is (%d, %d)\n", WPlayer.pieces[i].type, WPlayer.pieces[i].position[0], WPlayer.pieces[i].position[1]);
                printf("black %c position is (%d, %d)\n", BPlayer.pieces[i].type, BPlayer.pieces[i].position[0], BPlayer.pieces[i].position[1]);
            }
        }
    }


    return 0;
}