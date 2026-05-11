#include<stdio.h>
#include"lbl1.h"

int main(){
    Initial();
    tm another_thing = func(&thing);
    printf("length : %d", another_thing.length);
    for(int i = 0; i < 3; i++){
        printf("length : %d", another_thing.InLength[i]);
    }

    return 0;
}