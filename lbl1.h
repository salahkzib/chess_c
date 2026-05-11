#ifndef CHESSPROGRAM_H
#define CHESSPROGRAM_H
#include <stdbool.h>

typedef struct
{
    int length;
    int *InLength;
    int ***Arr;
}tm;
typedef struct
{
    int x;
    int y;
}Obj, *PtrObj;
Obj thing;

void Initial();
tm func(PtrObj something);

#endif