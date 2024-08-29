#pragma once

#include "game/house.h"
#include "sep/Newton.h"
#include "sep/Rhine.h"

enum Directions{
    UP = 0,DOWN = 1,LEFT = 2,RIGHT = 3
};

typedef struct {
    NT_VEC2 pos;
    NT_VEC2 playerCenter;
    NT_VEC2 gunHeadPos;
    NT_VEC2 protonGunEnd;
    float velocity;
    bool canMove[4];
    NT_RECT bumpers[4] ;
    float animTimer;
    float rotation;
    bool isShooting;
    bool connectedToGhost;
    bool canRotate;
    float pullingForce;
    bool detectorIsBeingUsed;
    int detectorLevel; // 3 to 1
    int catchedGhosts;
    
}PLAYER_TYPE;

extern PLAYER_TYPE PLAYER;

void PLAYER_Update(HOUSE house);

void PLAYER_Render(NT_VEC2 cameraPos);

void RHN_DrawProtonRay(int startX, int startY, int endX, int endY, int width, uint32 color, float time);

NT_VEC2 rotatePoint(float cx, float cy, float pX, float pY, float theta);