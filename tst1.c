#include<stdio.h>
#include"lbl1.h"

int main(){
    //Initial();
    tm another_thing = func();
    printf("length : %d\n", another_thing.length);
    for(int i = 0; i < 3; i++){
        printf("length : %d\n", another_thing.InLength[i]);
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 2; k++){
                printf("%d ", another_thing.Arr[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}