#include <stdio.h>
#include "AppUI.c"


//array[][][] = {8*8*2}
struct Player1
{
    int *Who;//point on an array of pieces of the player {piece with its *point *ability{true , false}}
    int array[];
};
struct Player2
{
    int *Who;//point on an array of pieces of the player {piece with its *point *ability{true , false}}
    int array[];
};


/*
player1 move
Check
Moves(player 2)
Check
player move
Check
Moves PLayer1
.....



Move*/


void PawnMovement(){
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
}
void KingMovement(){

}
void QueenMovement(){

}
void RookMovement(){

}
void BishopMovement(){
/*
bp = Bishop.index;

for(int j = 0; j < )

*/
}
void NightMovement(){
/*
ability
in board
square : empty, piece diff color
*/
}