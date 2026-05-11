#ifndef CHESSPROGRAM_H
#define CHESSPROGRAM_H

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
extern Obj thing;

void Initial();
tm func();

#endif