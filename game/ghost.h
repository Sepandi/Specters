#pragma once

#include "sep/Newton.h"



typedef struct{
    NT_VEC2 pos;
    NT_VEC2 ghostPlace;
    bool isVisable;
    bool isCatched;
    float immune;
    float rotation;
}GHOST;



