#include "house.h"
#include "game/player.h"
#include "sep/Darwin.h"

#include "sep/Newton.h"
#include "sep/Rhine.h"

#include "assets.h"

#include <stdbool.h>
#include <stdio.h>
#include "random.h"

#ifdef _WIN32
    #include "windows.h"
#else
    #include "unistd.h"
#endif



HOUSE HOUSE_New(int roomCount,NT_VEC2 houseSize,int tileSize){
    HOUSE house;
    house.seed = GetRandomSeed();
    house.houseSize = houseSize;
    house.tileSize = tileSize;
    house.roomCount = roomCount;
    PLAYER.catchedGhosts = 0;
    PLAYER.detectorIsBeingUsed = false;

    house.tiles = (TILE*)malloc(houseSize.x*houseSize.y*sizeof(TILE));
    for (int i = 0;i<houseSize.x*houseSize.y;i++){
        house.tiles[0].type = AIR;
    }

    house.rooms = (NT_RECT*)malloc(roomCount*sizeof(NT_RECT));
    for (int i = 0;i<roomCount;i++){
        house.rooms[i] = NT_RECT_New(0,0,0,0);
    }

    int indexOfLowestRoom = 0;
    int lowestRoomIndicator = 0;

    // Generate rooms
    house.setUpRoomsCount = 0;
    for (int i = 0; i < roomCount; i++) {
        HOUSE_AddRoom(house);
        house.setUpRoomsCount++;
        // Set Spawn point to lowest room
        if (house.rooms[i].y+house.rooms[i].h > lowestRoomIndicator){
            lowestRoomIndicator = house.rooms[i].y+house.rooms[i].h;
            indexOfLowestRoom = i;
        }
    }

    NT_RECT closestRoom;
    NT_RECT secondClosestRoom;

    for (int i = 0;i < roomCount;i++){
        for (int x = house.rooms[i].x;x < house.rooms[i].x + house.rooms[i].w;x++){
            for (int y = house.rooms[i].y;y < house.rooms[i].y + house.rooms[i].h;y++){
                house.tiles[(y * (int)houseSize.x) + x].type = FLOOR;
            }
        }

        for (int x = house.rooms[i].x;x < house.rooms[i].x + house.rooms[i].w;x++){
            house.tiles[(((int)house.rooms[i].y-1) * (int)houseSize.x) + x].type = WALL;
            house.tiles[(((int)house.rooms[i].y+(int)house.rooms[i].h) * (int)houseSize.x) + x].type = WALL;
        }

        for (int y = house.rooms[i].y;y < house.rooms[i].y + house.rooms[i].h;y++){
            house.tiles[(((y) * (int)houseSize.x) + (int)house.rooms[i].x-1)].type = WALL;
            house.tiles[(((y) * (int)houseSize.x) + (int)house.rooms[i].x+(int)house.rooms[i].w)].type = WALL;
        }

        float closestRoomDistance = house.houseSize.x;
        float secondClosestRoomDistance = house.houseSize.x;
        NT_VEC2 r1Center = HOUSE_GetCenterOfRoom(house.rooms[i]);

        for (int i2 = 0; i2 < roomCount; i2++) {
            if (!NT_RECT_Equal(house.rooms[i],house.rooms[i2])) {
                NT_VEC2 r2Center = HOUSE_GetCenterOfRoom(house.rooms[i2]);

                float distance = HOUSE_GetDistance(r1Center, r2Center);

                if (distance < closestRoomDistance && distance != 0) {
                    // Update second closest
                    secondClosestRoomDistance = closestRoomDistance;
                    secondClosestRoom = closestRoom;

                    // Update closest
                    closestRoomDistance = distance;
                    closestRoom = house.rooms[i2];
                } else if (distance < secondClosestRoomDistance && distance != 0) {
                    // Update second closest only
                    secondClosestRoomDistance = distance;
                    secondClosestRoom = house.rooms[i2];
                }
            }
        }



        // Connect to closest room
        HOUSE_ConnectRooms(house,house.rooms[i],closestRoom);

        // Connect to second closest room
        HOUSE_ConnectRooms(house,house.rooms[i],secondClosestRoom);

    }


    NT_VEC2 spawnPoint = HOUSE_GetCenterOfRoom(house.rooms[indexOfLowestRoom]);
    spawnPoint.y += house.rooms[indexOfLowestRoom].h/2;
    house.spawn = (NT_VEC2){spawnPoint.x*house.tileSize,spawnPoint.y*house.tileSize};
    PLAYER.pos = (NT_VEC2){house.spawn.x,house.spawn.y-(tileSize*1.5)};
    for (int x=0;x<house.houseSize.x;x++){
        for (int y=0;y<house.houseSize.y;y++){
            house.tiles[(y * (int)houseSize.x) + x].rect = NT_RECT_New(x*house.tileSize,y*tileSize,tileSize,tileSize);

            if (house.tiles[(y * (int)houseSize.x) + x].type == WALL && house.tiles[((y+1) * (int)houseSize.x) + x].type == FLOOR && house.tiles[((y-1) * (int)houseSize.x) + x].type == FLOOR){
                house.tiles[(y * (int)houseSize.x) + x].type = FLOOR;
            }

            if (house.tiles[(y * (int)houseSize.x) + x].type == WALL && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR && house.tiles[(y* (int)houseSize.x) + x-1].type == FLOOR){
                house.tiles[(y * (int)houseSize.x) + x].type = FLOOR;
            }

            if (house.tiles[(y * (int)houseSize.x) + x].type == WALL && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR && house.tiles[(y* (int)houseSize.x) + x-1].type == FLOOR && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR){
                house.tiles[(y * (int)houseSize.x) + x].type = FLOOR;
            }

            if (house.tiles[(y * (int)houseSize.x) + x].type == WALL && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR && house.tiles[(y* (int)houseSize.x) + x-1].type == FLOOR && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR && house.tiles[((y-1)* (int)houseSize.x) + x].type == FLOOR && house.tiles[((y+1) * (int)houseSize.x) + x].type == FLOOR){
                house.tiles[(y * (int)houseSize.x) + x].type = FLOOR;
            }
        }
    }


    for (int x=0;x<house.houseSize.x;x++){
        for (int y=0;y<house.houseSize.y;y++){
            house.tiles[(y * (int)houseSize.x) + x].visited = false;
            if (house.tiles[(y * (int)houseSize.x) + x].type == WALL){

                if (house.tiles[(y * (int)houseSize.x) + (x + 1)].type == FLOOR){ // RIGHT
                
                    if (house.tiles[((y+1) * (int)houseSize.x) + x].type == FLOOR){ // BUTTOM RIGHT 
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D0;
                        house.tiles[(y * (int)houseSize.x) + x].status = CORNER_IN;
                    }else if (house.tiles[((y-1) * (int)houseSize.x) + x].type == FLOOR){ // TOP RIGHT 
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D270;
                        house.tiles[(y * (int)houseSize.x) + x].status = CORNER_IN;
                    }else{
                        house.tiles[(y * (int)houseSize.x) + x ].rotation = D270;
                        house.tiles[(y * (int)houseSize.x) + x].status = NORMAL;
                    }
                    
                }else if (house.tiles[(y * (int)houseSize.x) + (x - 1)].type == FLOOR){ // LEFT

                    if (house.tiles[((y+1) * (int)houseSize.x) + x].type == FLOOR){ // BUTTOM LEFT 
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D90;
                        house.tiles[(y * (int)houseSize.x) + x].status = CORNER_IN;
                    }else if (house.tiles[((y-1) * (int)houseSize.x) + x].type == FLOOR){ // TOP LEFT 
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D180;
                        house.tiles[(y * (int)houseSize.x) + x].status = CORNER_IN;
                    }else{
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D90;
                        house.tiles[(y * (int)houseSize.x) + x].status = NORMAL;
                    }
                    
                }else if (house.tiles[((y+1) * (int)houseSize.x) + x].type == FLOOR){ // DOWN
                    house.tiles[(y * (int)houseSize.x) + x].rotation = D0;
                    house.tiles[(y * (int)houseSize.x) + x].status = NORMAL;
                }else if (house.tiles[((y-1) * (int)houseSize.x) + x].type == FLOOR){ // UP
                    house.tiles[(y * (int)houseSize.x) + x].rotation = D180;
                    house.tiles[(y * (int)houseSize.x) + x].status = NORMAL;
                }
            }else if (house.tiles[(y * (int)houseSize.x) + x].type == AIR){
                if (house.tiles[(y * (int)houseSize.x) + (x + 1)].type == WALL){ // RIGHT
                
                    if (house.tiles[((y+1) * (int)houseSize.x) + x].type == WALL && house.tiles[((y+1) * (int)houseSize.x) + x+1].type == FLOOR){ // TOP LEFT 
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D270;
                        house.tiles[(y * (int)houseSize.x) + x].status = CORNER_OUT;
                    }else if (house.tiles[((y-1) * (int)houseSize.x) + x].type == WALL && house.tiles[((y-1) * (int)houseSize.x) + x+1].type == FLOOR){ // BUTTOM LEFT 
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D180;
                        house.tiles[(y * (int)houseSize.x) + x].status = CORNER_OUT;
                    }else{
                        house.tiles[(y * (int)houseSize.x) + x ].rotation = D270;
                        house.tiles[(y * (int)houseSize.x) + x].status = NORMAL;
                    }
                    
                }else if (house.tiles[(y * (int)houseSize.x) + (x - 1)].type == WALL){ // LEFT

                    if (house.tiles[((y+1) * (int)houseSize.x) + x].type == WALL && house.tiles[((y+1) * (int)houseSize.x) + x-1].type == FLOOR){ // TOP RIGHT 
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D0;
                        house.tiles[(y * (int)houseSize.x) + x].status = CORNER_OUT;
                    }else if (house.tiles[((y-1) * (int)houseSize.x) + x].type == WALL && house.tiles[((y-1) * (int)houseSize.x) + x-1].type == FLOOR){ // BUTTOM RIGHT 
                        house.tiles[(y * (int)houseSize.x) + x].rotation = D90;
                        house.tiles[(y * (int)houseSize.x) + x].status = CORNER_OUT;
                    }
                    
                }
            }
        }
    }
    for (int x=0;x<house.houseSize.x;x++){
        for (int y=0;y<house.houseSize.y;y++){
            if (house.tiles[(y * (int)houseSize.x) + x].type == WALL && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR && house.tiles[(y* (int)houseSize.x) + x-1].type == FLOOR && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR && house.tiles[(y * (int)houseSize.x) + x+1].type == FLOOR && house.tiles[((y-1)* (int)houseSize.x) + x].type == FLOOR && house.tiles[((y+1) * (int)houseSize.x) + x].type == FLOOR){
                house.tiles[(y * (int)houseSize.x) + x].type = FLOOR;
            }
        }
    }

    house.ghostPlaced = 0;
    for (int i = 0;i < GHOST_COUNT;i++){
        house.ghosts[house.ghostPlaced].isCatched = false;
        house.ghosts[house.ghostPlaced].isVisable = false;
        house.ghosts[house.ghostPlaced].pos = HOUSE_GHOST_MOVE(house);
        house.ghosts[house.ghostPlaced].ghostPlace = house.ghosts[house.ghostPlaced].pos;
        house.ghosts[house.ghostPlaced].immune = 0;
        house.ghostPlaced++;
    }

    return house;
}

void HOUSE_GHOST_MoveTowards(NT_VEC2 *current, NT_VEC2 target, float v, float playerRadius) {
    float dx = target.x - current->x;
    float dy = target.y - current->y;
    float distance = sqrtf(dx * dx + dy * dy);
    
    if (distance > v) {
        float unitX = dx / distance;
        float unitY = dy / distance;
        current->x += unitX * v;
        current->y += unitY * v;
    } else {
        // If the remaining distance is less than or equal to v, move directly to the target
        current->x = target.x;
        current->y = target.y;
    }
}

void HOUSE_GHOST_Update(HOUSE *house) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        NT_RECT ghostAABB = {house->ghosts[i].pos.x, house->ghosts[i].pos.y, (float)house->tileSize/2, (float)house->tileSize/2};
        if (NT_RECT_IsCollidingWithLine(ghostAABB, PLAYER.gunHeadPos,PLAYER.protonGunEnd) && PLAYER.isShooting) {
            house->ghosts[i].immune+= DW_GetDeltaTime();
            if (DW_IsKeyPressed(KEY_SPACE) || DW_IsControllerButtonPressed(BUTTON_A)){
                PLAYER.pullingForce+=0.1;
            }
            if (PLAYER.pullingForce > 1.5) PLAYER.pullingForce = 1.5;
            if (PLAYER.pullingForce < 0) PLAYER.pullingForce = 0;

            house->ghosts[i].pos.y -= (DW_GetDeltaTime()*(5*PLAYER.pullingForce)*sin(PLAYER.rotation));
            house->ghosts[i].pos.x -= (DW_GetDeltaTime()*(5*PLAYER.pullingForce)*cos(PLAYER.rotation));
            house->ghosts[i].isVisable = true;

            if (!PLAYER.connectedToGhost){
                house->ghosts[i].ghostPlace = HOUSE_GHOST_MOVE(*house);
            }
            PLAYER.connectedToGhost = true;
            PLAYER.protonGunEnd = (NT_VEC2){house->ghosts[i].pos.x+((float)TEXTURE_GHOST.width/2),house->ghosts[i].pos.y+((float)TEXTURE_GHOST.height/2)};
            if (fabs(house->ghosts[i].pos.x-PLAYER.pos.x) > house->tileSize*3 || fabs(house->ghosts[i].pos.y-PLAYER.pos.y) > house->tileSize*3){
                PLAYER.isShooting = false;
                PLAYER.connectedToGhost = false;
                house->ghosts[i].immune = 0;
                house->ghosts[i].isVisable = false;
            }
        }
        if (NT_RECT_IsCollidingWithRect(ghostAABB, (NT_RECT){PLAYER.pos.x-3,PLAYER.pos.y-3,((float)house->tileSize/2)+6,((float)house->tileSize/2)+6}) && PLAYER.isShooting && house->ghosts[i].immune > 5) {
            house->ghosts[i].pos.x = 0;
            house->ghosts[i].pos.y = 0;
            house->ghosts[i].isCatched = true;
            house->ghosts[i].isVisable = false;
            house->ghosts[i].immune = 0;
            PLAYER.catchedGhosts++;
            PLAYER.connectedToGhost = false;
        }else{
            if (!house->ghosts[i].isCatched){
                HOUSE_GHOST_MoveTowards(&house->ghosts[i].pos,house->ghosts[i].ghostPlace,DW_GetDeltaTime()*6,2*house->tileSize);
                house->ghosts[i].rotation = atan2(house->ghosts[i].ghostPlace.y - house->ghosts[i].pos.y, house->ghosts[i].ghostPlace.x - house->ghosts[i].pos.x);
            }
                
        }


        
    }

    
}
NT_VEC2 HOUSE_GHOST_MOVE(HOUSE house){
    int room = RandInt(0,house.roomCount-1);
    return (NT_VEC2){(house.rooms[room].x+RandInt(1,house.rooms[room].w-2))*(house.tileSize),(house.rooms[room].y+RandInt(1,house.rooms[room].h-2))*house.tileSize};
}

void HOUSE_AddRoom(HOUSE house){
    NT_RECT newRoom;
    bool acceptableDimensions = false;
    while (!acceptableDimensions) {
        // Adjust Perlin noise parameters for more controlled room dimensions
        newRoom.x = (int)(GetPerlinValue((float)RandInt(0, 100) / 20, (float)RandInt(0, 100) / 40, house.seed) * (house.houseSize.x - 1));
        newRoom.y = (int)(GetPerlinValue((float)RandInt(0, 100) / 20, (float)RandInt(0, 100) / 40, house.seed) * (house.houseSize.y - 1));
        newRoom.w = (int)(GetPerlinValue((float)RandInt(0, 100) / 20, (float)RandInt(0, 100) / 40, house.seed) * (NT_Clamp(house.houseSize.x,5.0,20.0)));
        newRoom.h = (int)(GetPerlinValue((float)RandInt(0, 100) / 20, (float)RandInt(0, 100) / 40, house.seed) * (NT_Clamp(house.houseSize.y,5.0,20.0)));

        // Check if the new room is within bounds
        if (newRoom.x < 0 || newRoom.y < 0 ||newRoom.x + newRoom.w >= house.houseSize.x ||newRoom.y + newRoom.h >= house.houseSize.y) {
            continue; // Room is out of bounds, generate again
        }

        // Check if the new room overlaps with existing rooms
        bool acceptableDimensionsForAllRects = true;
        NT_RECT extendedRoom = NT_RECT_New(newRoom.x - 1,newRoom.y - 1, newRoom.w + 2,newRoom.h + 2);
        for (int i = 0;i < house.setUpRoomsCount;i++) {
            if (NT_RECT_IsCollidingWithRect(house.rooms[i], extendedRoom)) {
                acceptableDimensionsForAllRects = false;
                break;
            }
        }

        acceptableDimensions = acceptableDimensionsForAllRects;
    }

    house.rooms[house.setUpRoomsCount] = newRoom;
}

int HOUSE_GHOST_GetDistanceToNearestGhost(HOUSE house){
    int closestGhost = 2000;
    for (int i = 0;i < GHOST_COUNT;i++){
        int distance = HOUSE_GetDistance(PLAYER.pos,house.ghosts[i].pos);
        if (distance < closestGhost){
            closestGhost = distance;
        }
    }
    return closestGhost;
}

NT_VEC2 HOUSE_GetCenterOfRoom(NT_RECT rect){
    return (NT_VEC2){rect.x+(rect.w/2),rect.y+(rect.h/2)};
}

float HOUSE_GetDistance(NT_VEC2 point1,NT_VEC2 point2){
    return sqrt((fabs(point1.x-point2.x)*fabs(point1.x-point2.x))+(fabs(point1.y-point2.y)*fabs(point1.y-point2.y)));
}

void HOUSE_ConnectRooms(HOUSE house,NT_RECT room1,NT_RECT room2) {
    NT_VEC2 r1Center = HOUSE_GetCenterOfRoom(room1);
    NT_VEC2 r2Center = HOUSE_GetCenterOfRoom(room2);

    // Casting to int multipel times can give verying results
    // so I just set them right here
    int r1x = r1Center.x;
    int r1y = r1Center.y;

    int r2x = r2Center.x;
    int r2y = r2Center.y;

    // Connect horizontally
    if (r1x > r2x) {
        for (int x = r1x; x >= r2x; x--) {
            house.tiles[(r1y *  (int)house.houseSize.x) + x].type = FLOOR;

            if (r1y > 0 &&  house.tiles[((r1y - 1) * (int)house.houseSize.x) + x].type != FLOOR)
                 house.tiles[((r1y - 1) *  (int)house.houseSize.x) + x].type = WALL;
            if (r1y < house.houseSize.y - 1 &&  house.tiles[((r1y + 1) *  (int)house.houseSize.x) + x].type != FLOOR)
                 house.tiles[((r1y + 1) *  (int)house.houseSize.x) + x].type = WALL;

        }
    } else {
        for (int x = r1x; x <= r2x; x++) {
             house.tiles[(r1y *  (int)house.houseSize.x) + x].type = FLOOR;

            if (r1y > 0 &&  house.tiles[((r1y - 1) *  (int)house.houseSize.x) + x].type != FLOOR)
                 house.tiles[((r1y - 1) *  (int)house.houseSize.x) + x].type = WALL;
            if (r1y <  house.houseSize.y - 1 &&  house.tiles[((r1y + 1) *  (int)house.houseSize.x) + x].type != FLOOR)
                 house.tiles[((r1y + 1) *  (int)house.houseSize.x) + x].type = WALL;
        }
    }

    // Connect vertically
    if (r1y <= r2y) {
        for (int y = r1y; y <= r2y; y++) {
            house.tiles[((y * (int)house.houseSize.x) + r2x)].type = FLOOR;

            if (r2x > 0 && house.tiles[(y * (int)house.houseSize.x) + r2x - 1].type != FLOOR )
                house.tiles[(y * (int)house.houseSize.x) + r2x - 1].type = WALL;
            if (r2x < house.houseSize.x - 1 && house.tiles[(y * (int)house.houseSize.x) + r2x + 1].type != FLOOR)
                house.tiles[(y * (int)house.houseSize.x) + r2x + 1].type = WALL;
        }
    } else {
        for (int y = r1y; y >= r2y; y--) {
            house.tiles[(y * (int)house.houseSize.x) + r2x].type = FLOOR;

            if (r2x > 0 && house.tiles[(y * (int)house.houseSize.x) + r2x - 1].type != FLOOR)
                house.tiles[(y * (int)house.houseSize.x) + r2x - 1].type = WALL;
            if (r2x < house.houseSize.x - 1 && house.tiles[(y * (int)house.houseSize.x) + r2x + 1].type != FLOOR)
                house.tiles[(y * (int)house.houseSize.x) + r2x + 1].type = WALL;

        }

    }





}



void HOUSE_Render(HOUSE house,int winWidth,int winHeight,NT_VEC2 cameraPos){
    int index = 0;
    for (int y = 0;y < house.houseSize.y;y++){
        for (int x = 0;x < house.houseSize.x;x++){
            if (cameraPos.x+(x*house.tileSize) > -house.tileSize && cameraPos.x+(x*house.tileSize) < RHN_GetCanvasWidth() && cameraPos.y+(y*house.tileSize) > -house.tileSize && cameraPos.y+(y*house.tileSize) < RHN_GetCanvasHeight()){
                switch (house.tiles[index].type) {
                    case AIR:
                        if (house.tiles[index].status == CORNER_OUT){
                            RHN_DrawTexture(RHN_TEXTURE_Rotate(TEXTURE_WALL_CORNER_OUT,house.tiles[index].rotation),cameraPos.x+(x*house.tileSize),cameraPos.y+(y*house.tileSize),2,0);
                        }
                        break;
                    case WALL:
                        if (house.tiles[index].status == CORNER_IN){
                            RHN_DrawTexture(RHN_TEXTURE_Rotate(TEXTURE_WALL_CORNER_IN,house.tiles[index].rotation),cameraPos.x+(x*house.tileSize),cameraPos.y+(y*house.tileSize),2,0);
                        }else if (house.tiles[index].status == NORMAL){
                            RHN_DrawTexture(RHN_TEXTURE_Rotate(TEXTURE_WALL,house.tiles[index].rotation),cameraPos.x+(x*house.tileSize),cameraPos.y+(y*house.tileSize),2,0);
                        }
                        
                        break;
                    case FLOOR:
                        RHN_DrawTexture(TEXTURE_FLOOR,cameraPos.x+(x*house.tileSize),cameraPos.y+(y*house.tileSize),2,0);
                        break;
                }
            }
            index++;
        }
    }

    for (int i = 0;i < GHOST_COUNT;i++){
        if (house.ghosts[i].isVisable){
            NT_VEC2 ghostsPos = house.ghosts[i].pos;
            RHN_DrawTexture(TEXTURE_GHOST,cameraPos.x+ghostsPos.x,cameraPos.y+ghostsPos.y,1,90-((house.ghosts[i].rotation+180) * (180.0f / NT_PI)));
        }
    }
}

void HOUSE_RenderMiniMap(HOUSE house,NT_VEC2 playerPos,int tileSize){
    int size = 2;
    int x2 = 0;
    int y2 = 0;
    RHN_DrawRect(0,0,40,40,RHN_COLOR_BLACK);
    for (int y = (playerPos.y/tileSize)-10;y < (playerPos.y/tileSize)+10;y++){
        x2 = 0;
        for (int x = (playerPos.x/tileSize)-10;x < (playerPos.x/tileSize)+9;x++){
            switch (house.tiles[(y*(int)house.houseSize.x)+x].type) {
                case AIR:
                    break;
                case WALL:
                    RHN_DrawRect(x2*size,y2*size,size,size,RHN_COLOR_RED);
                    break;
                case FLOOR:
                    if (house.tiles[(y*(int)house.houseSize.x)+x].visited){
                        RHN_DrawRect(x2*size,y2*size,size,size,RHN_COLOR_GREEN);
                    }else{
                        RHN_DrawRect(x2*size,y2*size,size,size,RHN_COLOR_OLIVE);
                    }
                    
                    break;
            }
            x2++;
        }
        
        y2++;
    }
    int px = playerPos.x / tileSize;
    int py = playerPos.y / tileSize;
    RHN_DrawRect(20,20,size,size,RHN_COLOR_PINK);
    RHN_DrawTexture(TEXTURE_MINI_MAP_FRAME,0,0,2,0);
}
