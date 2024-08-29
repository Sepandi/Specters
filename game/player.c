#include "player.h"
#include "game/house.h"
#include "game/random.h"
#include "sep/Darwin.h"
#include "sep/Newton.h"
#include "sep/Rhine.h"
#include "game/assets.h"
#include "math.h"
#include <stdbool.h>
#include <stdio.h>

PLAYER_TYPE PLAYER;


void PLAYER_Update(HOUSE house){
    PLAYER.animTimer+= DW_GetDeltaTime()*1.75;

    if (!PLAYER.connectedToGhost){
        PLAYER.isShooting = false;
    }
    

    // GET COLISSION
    PLAYER.canMove[0] = true; 
    PLAYER.canMove[1] = true;
    PLAYER.canMove[2] = true;
    PLAYER.canMove[3] = true;

    PLAYER.bumpers[UP] = NT_RECT_New(PLAYER.pos.x,PLAYER.pos.y-1,(float)house.tileSize/2,1);
    PLAYER.bumpers[DOWN] = NT_RECT_New(PLAYER.pos.x,PLAYER.pos.y+((float)house.tileSize/2),(float)house.tileSize/2,1);
    PLAYER.bumpers[LEFT] = NT_RECT_New(PLAYER.pos.x-1,PLAYER.pos.y,1,(float)house.tileSize/2);
    PLAYER.bumpers[RIGHT] = NT_RECT_New(PLAYER.pos.x+((float)house.tileSize/2),PLAYER.pos.y,1,(float)house.tileSize/2);

    for (int i = 0;i < house.houseSize.x*house.houseSize.y;i++){
        if (house.tiles[i].type == WALL){
            if (NT_RECT_IsCollidingWithRect(house.tiles[i].rect,PLAYER.bumpers[UP])){
                PLAYER.canMove[UP] = false;
            }
            if (NT_RECT_IsCollidingWithRect(house.tiles[i].rect,PLAYER.bumpers[DOWN])){
                PLAYER.canMove[DOWN] = false;
            }
            if (NT_RECT_IsCollidingWithRect(house.tiles[i].rect,PLAYER.bumpers[RIGHT])){
                PLAYER.canMove[RIGHT] = false;
            }
            if (NT_RECT_IsCollidingWithRect(house.tiles[i].rect,PLAYER.bumpers[LEFT])){
                PLAYER.canMove[LEFT] = false;
            }   
        }else if (house.tiles[i].type == FLOOR){
            NT_RECT playerRect = NT_RECT_New((int)PLAYER.pos.x-((float)house.tileSize*4),(int)PLAYER.pos.y-((float)house.tileSize*4),(float)house.tileSize*8,(float)house.tileSize*8);
            if (NT_RECT_IsCollidingWithRect(house.tiles[i].rect,playerRect)){
                house.tiles[i].visited = true;
            }
        }
    }


    // SHOOTING
    if (DW_IsMouseKeyDown(MOUSE_KEY_LEFT)){
        PLAYER.isShooting = true;
    }

    if (DW_IsMouseKeyPressed(MOUSE_KEY_LEFT) && PLAYER.connectedToGhost){
        PLAYER.connectedToGhost = false;
        PLAYER.isShooting = false;
    }

    if (DW_IsKeyPressed(KEY_F)){
        PLAYER.detectorIsBeingUsed = !PLAYER.detectorIsBeingUsed;
    }
    

    // MOVEMENT
    //if (!PLAYER.isShooting){
        float moveX = 0;
        float moveY = 0;

        if ((DW_IsKeyDown(KEY_W) || DW_IsKeyDown(KEY_ARROW_UP)) && PLAYER.canMove[UP]){
            PLAYER.pos.y -= PLAYER.velocity * DW_GetDeltaTime();
        }
        if ((DW_IsKeyDown(KEY_S)|| DW_IsKeyDown(KEY_ARROW_DOWN)) && PLAYER.canMove[DOWN]){
            PLAYER.pos.y += PLAYER.velocity * DW_GetDeltaTime();
        }
        if ((DW_IsKeyDown(KEY_A)|| DW_IsKeyDown(KEY_ARROW_LEFT)) && PLAYER.canMove[LEFT]){
            PLAYER.pos.x -= PLAYER.velocity * DW_GetDeltaTime();
        }
        if ((DW_IsKeyDown(KEY_D)|| DW_IsKeyDown(KEY_ARROW_RIGHT)) && PLAYER.canMove[RIGHT]){
            PLAYER.pos.x += PLAYER.velocity * DW_GetDeltaTime();
        }
    //}


    //

    if (PLAYER.connectedToGhost && PLAYER.isShooting){
        if (PLAYER.pullingForce){
            PLAYER.pullingForce -= DW_GetDeltaTime()/40;
        }
        PLAYER.canRotate = false;
    }else{
        PLAYER.pullingForce = 0.6;
        PLAYER.canRotate = true;
    }
    

    int closestGhost = HOUSE_GHOST_GetDistanceToNearestGhost(house);
    if (closestGhost > 5*house.tileSize){
        PLAYER.detectorLevel = 0;
    }else if (closestGhost <= 5*house.tileSize && closestGhost > 4*house.tileSize){
        PLAYER.detectorLevel = 1;
    }else if (closestGhost <= 4*house.tileSize && closestGhost > 2*house.tileSize){
        PLAYER.detectorLevel = 2;
    }else if (closestGhost <= 2*house.tileSize){
        PLAYER.detectorLevel = 3;
    }

    

   
    
}

void PLAYER_Render(NT_VEC2 cameraPos){
    
    float gunHeadX, gunHeadY;
    // Assume the gun's head is initially 32 units in front of the player in the player's direction
    PLAYER.playerCenter = (NT_VEC2){ PLAYER.pos.x + ((float)TEXTURE_PLAYER.width/2), PLAYER.pos.y + ((float)TEXTURE_PLAYER.width/2)};
    NT_VEC2 aim = rotatePoint(PLAYER.playerCenter.x,PLAYER.playerCenter.y, PLAYER.pos.x + TEXTURE_PLAYER.width, PLAYER.pos.y + TEXTURE_PLAYER.width - 5, PLAYER.rotation);
    if (!PLAYER.connectedToGhost){
        PLAYER.protonGunEnd = (NT_VEC2){PLAYER.playerCenter.x+aim.x + cos(PLAYER.rotation) * 64,PLAYER.playerCenter.y+aim.y + sin(PLAYER.rotation) * 64};
    }
    PLAYER.gunHeadPos = (NT_VEC2){PLAYER.playerCenter.x+aim.x,PLAYER.playerCenter.y+aim.y};
    RHN_DrawTexture(TEXTURE_PLAYER,cameraPos.x+PLAYER.pos.x,cameraPos.y+PLAYER.pos.y,1,180-(PLAYER.rotation * (180.0f / NT_PI)));;
    if (PLAYER.isShooting) {
        
        // Draw a line in the direction of the mouse for 32 units
        RHN_DrawProtonRay(
            cameraPos.x +PLAYER.gunHeadPos.x,  // Starting x position
            cameraPos.y +PLAYER.gunHeadPos.y,  // Starting y position
            cameraPos.x +PLAYER.protonGunEnd.x,  // Ending x position after 32 units
            cameraPos.y +PLAYER.protonGunEnd.y,  // Ending y position after 32 units
            1,  // Line thickness
            RHN_COLOR_BLUE,  // Line color
            PLAYER.animTimer
        );
    }else{
        RHN_DrawTexture(TEXTURE_CROSSHAIR,cameraPos.x +(PLAYER.playerCenter.x+aim.x + cos(PLAYER.rotation) * 64)-((float)TEXTURE_CROSSHAIR.width/2),cameraPos.y +(PLAYER.playerCenter.y+aim.y + sin(PLAYER.rotation) * 64)-((float)TEXTURE_CROSSHAIR.height/2),1,90-(PLAYER.rotation * (180.0f / NT_PI)));
    }
    
}

void RHN_DrawProtonRay( int startX, int startY, int endX, int endY, int width, uint32 color, float time) {
    int deltaX = abs(endX - startX);
    int deltaY = abs(endY - startY);
    int signX = (startX < endX) ? 1 : -1;
    int signY = (startY < endY) ? 1 : -1;
    int error = deltaX - deltaY;
    int error2;

    int halfWidth = width / 2;

    double angle = 0.0f;
    int distance = 40;

    while (true) {
        // Draw a thick line by setting pixels in a square around the main line
        for (int dy = -halfWidth; dy <= halfWidth; dy++) {
            for (int dx = -halfWidth; dx <= halfWidth; dx++) {
                // Calculate the rotating position offset with increasing distance
                double offsetX1 = cos(time + angle) * (distance * 0.1f);
                double offsetY1 = sin(time + angle) * (distance * 0.1f);
                double offsetX2 = cos(time + angle + NT_PI_2) * (distance * 0.1f);
                double offsetY2 = sin(time + angle + NT_PI_2) * (distance * 0.1f);
                
                

                // Set the pixels with the offset to create the rotating effect
                RHN_SetPixel(startX + dx + offsetX1, startY + dy + offsetY1, RHN_COLOR_RED);
                RHN_SetPixel(startX + dx - offsetX1, startY + dy - offsetY1, RHN_COLOR_GREEN);
                RHN_SetPixel(startX + dx + offsetX2, startY + dy + offsetY2, RHN_COLOR_PURPLE);
                RHN_SetPixel(startX + dx - offsetX2, startY + dy - offsetY2, RHN_COLOR_BLUE);

                int effectDistance1 = RandInt(0,RandInt(20,30));
                int effectDistance2 = RandInt(0,RandInt(20,30));
                int effectDistance3 = RandInt(0,RandInt(20,30));
                int effectDistance4 = RandInt(0,RandInt(20,30));
                
                if (PLAYER.connectedToGhost){
                    RHN_SetPixel(endX + (cos(time + NT_PI_2)*effectDistance1), endY + (sin(time + angle + NT_PI_2)*effectDistance1), RHN_COLOR_GREEN);
                    RHN_SetPixel(endX + (cos(time)*effectDistance2), endY + (sin(time + angle)*effectDistance2), RHN_COLOR_RED);
                    RHN_SetPixel(endX - (sin(time + NT_PI_2)*effectDistance3), endY + (cos(time + angle + NT_PI_2)*effectDistance3), RHN_COLOR_PURPLE);
                    RHN_SetPixel(endX - (sin(time)*effectDistance4), endY + (cos(time + angle)*effectDistance4), RHN_COLOR_BLUE);
                }

                
                // Increase angle for the slinky effect
                angle += 0.1f;
            }
        }

        //distance++; // Increase distance for the next iteration

        if (startX == endX && startY == endY) {
            break;
        }

        error2 = 2 * error;

        if (error2 > -deltaY) {
            error -= deltaY;
            startX += signX;
        }

        if (error2 < deltaX) {
            error += deltaX;
            startY += signY;
        }
    }
}


NT_VEC2 rotatePoint(float cx, float cy, float pX, float pY, float theta) {
    float s = sin(theta);
    float c = cos(theta);

    // Translate point back to origin
    pX -= cx;
    pY -= cy;

    // Rotate point
    float xnew = pX * c - pY * s;
    float ynew = pX * s + pY * c;

    // Translate point back
    return (NT_VEC2){xnew,ynew};
}