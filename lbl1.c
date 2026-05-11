#include <stdio.h>
#include "lbl1.h"


void Intial(){
    thing = (Obj){99, 100};
}
tm func(PtrObj something){
    tm RPM;
    int arr[3][3][2] = { {{something->x,something->y}, {44,1}, {0,9}}, {{27,17}, {21,54}, {3,2}}, {{1,23}, {4,65}, {2,3}} };
    int gc = 3;
    int cs[3] = {3, 3, 3};
    RPM.InLength = (int *)calloc(gc, sizeof(int));
    int ***array = (int ***)calloc(gc, sizeof(int **));
    for (int i = 0; i < gc; i++) {
        int **array = (int **)calloc(cs[i], sizeof(int *));
        RPM.InLength[i] = cs[i];
		for (int j = 0; j < 2; j++) {
            array[i] = (int *)calloc(2, sizeof(int));
        }
    }
    RPM.Arr = array;
    return RPM;
}