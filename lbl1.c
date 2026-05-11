#include <stdio.h>
#include <stdlib.h>
#include "lbl1.h"

Obj thing;
void Intial(){
    thing = (Obj){99, 100};
}
tm func(){
    tm RPM;
    int arr[3][3][2] = { {{10,10}, {44,1}, {0,9}}, {{27,17}, {21,54}, {3,2}}, {{1,23}, {4,65}, {2,3}} };
    int gc = 3;
    int cs[3] = {3, 3, 3};
    RPM.length = gc;
    RPM.InLength = (int *)calloc(gc, sizeof(int));
    RPM.Arr = (int ***)calloc(gc, sizeof(int **));
    for (int i = 0; i < gc; i++) {
        RPM.Arr[i] = (int **)calloc(cs[i], sizeof(int *));
        RPM.InLength[i] = cs[i];
		for (int j = 0; j < 2; j++) {
            RPM.Arr[i][j] = (int *)calloc(2, sizeof(int));
        }
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 2; k++){
                RPM.Arr[i][j][k] = arr[i][j][k];
            }
            printf("\n");
        }
        printf("\n");
    }
    return RPM;
}