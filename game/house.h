#pragma once

#include "game/ghost.h"
#include "sep/Newton.h"
#include "sep/Rhine.h"

#define GHOST_COUNT 4

typedef enum{
    AIR,
    WALL,
    FLOOR
}TILE_TYPE;

typedef enum{
    NORMAL,
    CORNER_IN,
    CORNER_OUT
}TILE_STATUS;


typedef struct{
    NT_RECT rect;
    TILE_TYPE type;
    RHN_ROTATION rotation;
    TILE_STATUS status;
    bool visited;
}TILE;

typedef struct{
    int seed;
    TILE* tiles;
    NT_RECT* rooms;
    NT_VEC2 houseSize;
    int setUpRoomsCount;
    int roomCount;
    NT_VEC2 spawn;
    int tileSize;
    GHOST ghosts[GHOST_COUNT];
    int ghostPlaced;
}HOUSE;

HOUSE HOUSE_New(int roomCount,NT_VEC2 houseSize,int tileSize);

void HOUSE_AddRoom(HOUSE house);

NT_VEC2 HOUSE_GetCenterOfRoom(NT_RECT rect);

float HOUSE_GetDistance(NT_VEC2 point1,NT_VEC2 point2);

void HOUSE_ConnectRooms(HOUSE house,NT_RECT room1,NT_RECT room2);

void HOUSE_GHOST_Update(HOUSE *house);
NT_VEC2 HOUSE_GHOST_MOVE(HOUSE house);

int HOUSE_GHOST_GetDistanceToNearestGhost(HOUSE house);

void HOUSE_Render(HOUSE house,int winWidth,int winHeight,NT_VEC2 cameraPos);
void HOUSE_RenderMiniMap(HOUSE house,NT_VEC2 playerPos,int tileSize);
