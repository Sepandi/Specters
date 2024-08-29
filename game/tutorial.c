#include "tutorial.h"
#include "game/house.h"
#include "game/player.h"
#include "sep/Newton.h"




HOUSE TUTORIAL_Make(){
    HOUSE house;
    NT_VEC2 houseSize = NT_VEC2_New(100,100);
    int tileSize = 32;
    int roomCount = GHOST_COUNT;
    house.seed = 1;
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
    for (int i = 0;i < roomCount;i++){
        if (house.ghostPlaced < GHOST_COUNT){
            house.ghosts[house.ghostPlaced].isCatched = false;
            house.ghosts[house.ghostPlaced].isVisable = false;
            house.ghosts[house.ghostPlaced].pos = HOUSE_GHOST_MOVE(house);
            house.ghosts[house.ghostPlaced].ghostPlace = house.ghosts[house.ghostPlaced].pos;
            house.ghosts[house.ghostPlaced].immune = 0;
            house.ghostPlaced++;
        }
    }

    return house;
}
void TUTORIAL_Udpate();
void TUTORIAL_Render();