#define RHINE_IMP
#define DARWIN_IMP
#define NEWTON_IMP
#define PAT_IMP
#define WILSON_IMP
#define MAD_MAX_IMP

#include "sep/Rhine.h"
#include "sep/Darwin.h"
#include "sep/Newton.h"
#include "sep/Wilson.h"
#include "sep/MadMax.h"

#include "game/house.h"
#include "game/assets.h"
#include "game/player.h"
#include "game/random.h"
#include "game/button.h"

#include "game/save.h"
#include "game/tutorial.h"
#include <string.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
typedef enum{
    GAME,MAIN_MENU,SETTINGS,DEV_LOAD,TUTORIAL
}SCREENS;

int tutorialStep = 0;

const char* TypeWriterEffect(const char* text, float* timerObj, float charsPerSecond) {
    // Update the timer object
    *timerObj += DW_GetDeltaTime() * charsPerSecond;

    // Calculate how many characters to show
    int charsToShow = NT_Clamp((int)(*timerObj), 0, (int)strlen(text));

    // Allocate memory for the partial text (+1 for the null terminator)
    char* partialText = (char*)malloc((charsToShow + 1) * sizeof(char));

    // Copy the portion of the original text into the new string
    strncpy(partialText, text, charsToShow);

    // Null-terminate the new string
    partialText[charsToShow] = '\0';

    return partialText;
}

bool TypeIsFinished(const char* text,float timerObj){
    return timerObj >= strlen(text);
}


int CMAIN(){
    MDMX_Init();
    ASSETS_LOAD();

    DW_WINDOW_New(800,600,"Specters - OPENGL");
    
    RHN_CANVAS_New(256*1.5, 144*1.5);
    DW_SetFullscreen(true);
    
    SAVE_LoadSave();

    
    
    int tileSize = 32;
    HOUSE h = HOUSE_New(10,NT_VEC2_New(100,100),tileSize);


    
    NT_VEC2 point = HOUSE_GetCenterOfRoom(h.rooms[RandInt(0,h.roomCount-1)]);
    float anim = 0;
    NT_VEC2 cameraPos = (NT_VEC2){-point.x*h.tileSize,-point.y*h.tileSize};


    PLAYER.velocity = 6;
    PLAYER.connectedToGhost = false;
    PLAYER.canRotate = true;
    PLAYER.catchedGhosts = 0;
    PLAYER.pullingForce = 1;
    PLAYER.detectorIsBeingUsed = false;
    
    float tick = 0;

    bool debugMenu = false;
    
    WLSN_CNFG config = WLSN_Load_From_String(CONFIG_FONT_GEMINI_TYPE,true);
    RHN_FONT font = RHN_FONT_Load(config,TEXTURE_FONT_GEMINI_TYPE,9,9);
    RHN_TEXTURE testChar = RHN_TEXTURE_Cut(TEXTURE_FONT_GEMINI_TYPE,0,0,9,9);
    
    SAVE_GetLang();
    WLSN_CNFG localization = WLSN_Load_From_String(LOCALIZATIONS[SAVE_Lang],false);

    NT_VEC2 detectorHUDPos = NT_VEC2_New(10,RHN_GetCanvasHeight());
    

    float soundTimer = 0;

    
    bool turnTheDetectorLightOn = false;
    float lightTimer = 0;

    SCREENS currentScreen = DEV_LOAD;

    float loadTimer = 0;
    float devNameY = 2;
    MDMX_SOUND_Play(&SOUND_DEV_LOAD);
    NT_VEC2 mousePos;

    bool menuOpen = false;
    float menuY = RHN_GetCanvasHeight();
    float menuColor = 0;

    bool wantToQuitPrompt = false;

    DW_SetIcon("Icon.ico");

    while (DW_IsRunning()){
        mousePos = RHN_GetCanvasMousePos(DW_GetWindowWidth(),DW_GetWindowHeight(),DW_GetMouseX(),DW_GetMouseY());

        switch (currentScreen){
            case DEV_LOAD:
                if(!MDMX_SOUND_IsPlaying(&SOUND_DEV_LOAD) || DW_IsKeyPressed(KEY_ESCAPE)){
                    currentScreen = MAIN_MENU;
                    MDMX_SOUND_Stop(&SOUND_DEV_LOAD);
                }
                break;
            case MAIN_MENU:
                // START MAIN_MENU RENDER
                if (!menuOpen && (DW_IsKeyPressed(KEY_RETURN) || DW_IsMouseKeyPressed(MOUSE_KEY_LEFT) ||DW_IsControllerButtonPressed(BUTTON_START))){
                    menuOpen = true;
                }else if (menuOpen && DW_IsKeyPressed(KEY_ESCAPE) ||DW_IsControllerButtonPressed(BUTTON_B)){
                    menuOpen = false;
                }
                // END MAIN_MENU UPDATE
                break;
            case GAME:
                // START GAME UPDATE
                if (!wantToQuitPrompt){
                    soundTimer+= DW_GetDeltaTime();
                    lightTimer+= DW_GetDeltaTime();

                    turnTheDetectorLightOn = false;
                    if (lightTimer > 1){
                        lightTimer = 0;
                        turnTheDetectorLightOn = true;
                    }

                    if (PLAYER.detectorLevel > 0 && soundTimer > (float)(4-PLAYER.detectorLevel) && PLAYER.detectorIsBeingUsed && !PLAYER.isShooting){
                        soundTimer = 0;
                        MDMX_SOUND_Play(&SOUND_GAM_CLOSE);
                    }else if (PLAYER.detectorIsBeingUsed && !PLAYER.isShooting){
                    if(!MDMX_SOUND_IsPlaying(&SOUND_GAM_IDLE)){
                            MDMX_SOUND_Play(&SOUND_GAM_IDLE);
                            
                        }
                    }

                    if (PLAYER.isShooting && !MDMX_SOUND_IsPlaying(&SOUND_SHOOT)){
                        MDMX_SOUND_Play(&SOUND_SHOOT);
                    }
                    

                    
                    tick+=DW_GetDeltaTime();

                    if (PLAYER.canRotate){
                        if ((DW_GetControllerRightStickX() > 0 || DW_GetControllerRightStickX() < 0) && (DW_GetControllerRightStickY() > 0 || DW_GetControllerRightStickY() < 0)){
                            PLAYER.rotation = atan2(0-DW_GetControllerRightStickY(),DW_GetControllerRightStickX()-0);
                        }else{
                            PLAYER.rotation = atan2(mousePos.y - ((float)RHN_GetCanvasHeight() / 2), (mousePos.x - ((float)RHN_GetCanvasWidth() / 2)));
                        }
                        
                    }else{
                        PLAYER.rotation = atan2(PLAYER.protonGunEnd.y - PLAYER.pos.y, (PLAYER.protonGunEnd.x - PLAYER.pos.x));
                    }
                    PLAYER_Update(h);
                    
                    cameraPos.x = -PLAYER.pos.x+((float)RHN_GetCanvasWidth()/2)-((float)TEXTURE_PLAYER.width/2);
                    cameraPos.y = -PLAYER.pos.y+((float)RHN_GetCanvasHeight()/2)-((float)TEXTURE_PLAYER.height/2);

                    HOUSE_GHOST_Update(&h);

                    if ((DW_IsKeyPressed(KEY_ESCAPE)||DW_IsControllerButtonPressed(BUTTON_START))|| PLAYER.catchedGhosts > GHOST_COUNT-1){
                        wantToQuitPrompt = true;
                    }
                }
                // END GAME UPDATE
                break;
            case SETTINGS:
                // START SETTINGS UPDATE

                // END SETTINGS UPDATE
                break;
            case TUTORIAL:
                // START TUTORIAL UPDATE
                if (!wantToQuitPrompt){
                    soundTimer+= DW_GetDeltaTime();
                    lightTimer+= DW_GetDeltaTime();


                    turnTheDetectorLightOn = false;
                    if (lightTimer > 1){
                        lightTimer = 0;
                        turnTheDetectorLightOn = true;
                    }

                    if (PLAYER.detectorLevel > 0 && soundTimer > (float)(4-PLAYER.detectorLevel) && PLAYER.detectorIsBeingUsed && !PLAYER.isShooting){
                        soundTimer = 0;
                        MDMX_SOUND_Play(&SOUND_GAM_CLOSE);
                    }else if (PLAYER.detectorIsBeingUsed && !PLAYER.isShooting){
                        if(!MDMX_SOUND_IsPlaying(&SOUND_GAM_IDLE)){
                            MDMX_SOUND_Play(&SOUND_GAM_IDLE);

                        }
                    }

                    if (PLAYER.isShooting){
                        if (!MDMX_SOUND_IsPlaying(&SOUND_SHOOT)){
                            MDMX_SOUND_Play(&SOUND_SHOOT);
                        }
                        
                    }
                    

                    
                    tick+=DW_GetDeltaTime();

                    if (PLAYER.canRotate){
                        PLAYER.rotation = atan2(mousePos.y - ((float)RHN_GetCanvasHeight() / 2), (mousePos.x - ((float)RHN_GetCanvasWidth() / 2)));
                    }else{
                        PLAYER.rotation = atan2(PLAYER.protonGunEnd.y - PLAYER.pos.y, (PLAYER.protonGunEnd.x - PLAYER.pos.x));
                    }

                    if (tutorialStep > 0){
                        PLAYER_Update(h);
                    }
                    cameraPos.x = -PLAYER.pos.x+((float)RHN_GetCanvasWidth()/2)-((float)TEXTURE_PLAYER.width/2);
                    cameraPos.y = -PLAYER.pos.y+((float)RHN_GetCanvasHeight()/2)-((float)TEXTURE_PLAYER.height/2);

                    HOUSE_GHOST_Update(&h);
                    if (DW_IsKeyPressed(KEY_ESCAPE)||DW_IsControllerButtonPressed(BUTTON_START)){
                        wantToQuitPrompt = true;
                    }
                }
                // END TUTORIAL UPDATE
                break;
        }

        // END UPDATE


        // START RENDER
        if (PLAYER.detectorIsBeingUsed){
            if (detectorHUDPos.y > RHN_GetCanvasHeight()-(TEXTURE_GAM_DETECTOR.height+20)){
                detectorHUDPos.y -= DW_GetDeltaTime()*8;
            }
            
        }else{
            if (detectorHUDPos.y < RHN_GetCanvasHeight()){
                detectorHUDPos.y += DW_GetDeltaTime()*8;
            }
        }

        switch (currentScreen){
            case DEV_LOAD:
                RHN_Fill(RHN_COLOR_BLACK);
                RHN_DrawText(font,TypeWriterEffect(WLSN_Get_Value(localization,"dev"),&loadTimer,0.4),((float)RHN_GetCanvasWidth()/2)-(strlen(WLSN_Get_Value(localization,"dev"))*4.5),devNameY,1,RHN_COLOR_WHITE);    
                if (TypeIsFinished(WLSN_Get_Value(localization,"dev"),loadTimer)){
                    if (devNameY < (float)RHN_GetCanvasHeight()/2){
                        devNameY+=DW_GetDeltaTime()*4;
                    }
                }
                
                break;
            case MAIN_MENU:
                // START MAIN_MENU RENDER
                menuColor += DW_GetDeltaTime()/10;
                RHN_Fill(RHN_COLOR_New(140+(sin(menuColor)), 48+(cos(menuColor)*25), 97-(sin(menuColor-2)*25),255));
                RHN_DrawCircle(10+(cos(menuColor)*10),10+(sin(menuColor)*10),10+(sin(menuColor)*2), RHN_COLOR_New(200+(sin(menuColor)*50), 95+(cos(menuColor)*50), 86-(sin(menuColor-2)*50),255));
                RHN_DrawCircle(330+(cos(menuColor)*10),100+(sin(menuColor)*10),30+(sin(menuColor)*2), RHN_COLOR_New(200+(sin(menuColor)*50), 95+(cos(menuColor)*50), 86-(sin(menuColor-2)*50),255));
                RHN_DrawCircle(200+(cos(menuColor)*10),30+(sin(menuColor)*10),50+(sin(menuColor)*2), RHN_COLOR_New(200+(sin(menuColor)*50), 95+(cos(menuColor)*50), 86-(sin(menuColor-2)*50),255));
                RHN_DrawCircle(20+(cos(menuColor)*10),200+(sin(menuColor)*10),50+(sin(menuColor)*2), RHN_COLOR_New(200+(sin(menuColor)*50), 95+(cos(menuColor)*50), 86-(sin(menuColor-2)*50),255));
                
                if (menuOpen){
                    if (menuY>(RHN_GetCanvasHeight()-(TEXTURE_MAINMENU.height*2))+2){
                        menuY-=DW_GetDeltaTime()*16;
                    }
                    if (menuY<(RHN_GetCanvasHeight()-(TEXTURE_MAINMENU.height*2))+2){
                        menuY = (RHN_GetCanvasHeight()-(TEXTURE_MAINMENU.height*2))+2;
                    }
                }else{
                    if (menuY<RHN_GetCanvasHeight()){
                        menuY+=DW_GetDeltaTime()*16;
                    }else{
                        const char * enterText = WLSN_Get_Value(localization,"press_enter_to_start");
                        RHN_DrawText(font,enterText,(RHN_GetCanvasWidth()/2)-((strlen(enterText)/2)*9),RHN_GetCanvasHeight()-30,1,RHN_COLOR_WHITE); 
                    }
                }
                NT_VEC2 titlePos = NT_VEC2_New(((float)RHN_GetCanvasWidth()/2)-(((float)strlen("Specters")*45)/2),10);
                RHN_DrawText(font,"Specters",titlePos.x,titlePos.y,5,RHN_COLOR_GRAY);   
                RHN_DrawText(font,"Specters",titlePos.x+1,titlePos.y+1,5,RHN_COLOR_GRAY);   
                RHN_DrawText(font,"Specters",titlePos.x+2,titlePos.y+2,5,RHN_COLOR_GRAY);  
                RHN_DrawText(font,"Specters",titlePos.x+3,titlePos.y+3,5,RHN_COLOR_WHITE);   
                RHN_DrawTexture(TEXTURE_MAINMENU,(RHN_GetCanvasWidth()/2)-((TEXTURE_MAINMENU.width*2)/2),menuY,2,0);
                const char * button1Text = stringf("> %s",WLSN_Get_Value(localization,"begin_game"));
                if (UI_Button(font,button1Text,(RHN_GetCanvasWidth()/2)-((strlen(button1Text)/2)*9),menuY+20,1,mousePos,true)){
                    h = HOUSE_New(10,NT_VEC2_New(100,100),tileSize);
                    currentScreen = GAME;
                }
                const char* tutorialText = stringf("> %s",WLSN_Get_Value(localization,"tutorial"));
                if (UI_Button(font,tutorialText,(RHN_GetCanvasWidth()/2)-((strlen(tutorialText)/2)*9),menuY+40,1,mousePos,true)){
                    h = TUTORIAL_Make();
                    currentScreen = TUTORIAL;
                }

                const char* setttingText = stringf("> %s",WLSN_Get_Value(localization,"settings"));
                if (UI_Button(font,setttingText,(RHN_GetCanvasWidth()/2)-((strlen(setttingText)/2)*9),menuY+60,1,mousePos,true)){
                    currentScreen = SETTINGS;
                }
                const char* quitText = stringf("> %s",WLSN_Get_Value(localization,"exit"));
                if (UI_Button(font,quitText,(RHN_GetCanvasWidth()/2)-((strlen(quitText)/2)*9),menuY+80,1,mousePos,true)){
                    DW_ForceQuit();
                }
                // END MAIN_MENU RENDER
                break;
            case GAME:
                // START GAME RENDER
                RHN_Fill(RHN_COLOR_BLACK);

                HOUSE_Render(h,DW_GetWindowWidth(),DW_GetWindowHeight(),cameraPos);

                PLAYER_Render(cameraPos);
                
                HOUSE_RenderMiniMap(h,PLAYER.pos,tileSize);

                if (PLAYER.connectedToGhost){
                    int x = 16;
                    int y = 107;
                    RHN_DrawRect(RHN_GetCanvasWidth()-x-2,RHN_GetCanvasHeight()-y-2,14,79,RHN_COLOR_WHITE);
                    RHN_DrawRect(RHN_GetCanvasWidth()-x,RHN_GetCanvasHeight()-y,10,75,RHN_COLOR_RED);
                    RHN_DrawRect(RHN_GetCanvasWidth()-x,(RHN_GetCanvasHeight()-(y-24)+(50-(PLAYER.pullingForce*50))+1),10,(PLAYER.pullingForce*50)+1,RHN_COLOR_OLIVE);
                }
                
                if (turnTheDetectorLightOn){
                    RHN_DrawTexture(TEXTURE_GAM_DETECTOR_ON, detectorHUDPos.x,detectorHUDPos.y,1,0);
                }else{
                    RHN_DrawTexture(TEXTURE_GAM_DETECTOR, detectorHUDPos.x,detectorHUDPos.y,1,0);
                }
                
                if (PLAYER.detectorLevel >= 1){
                    RHN_DrawRect(detectorHUDPos.x+11,detectorHUDPos.y+37,10,9,RHN_COLOR_YELLOW);
                } 
                if (PLAYER.detectorLevel >= 2){
                    RHN_DrawRect(detectorHUDPos.x+11,detectorHUDPos.y+28,10,9,RHN_COLOR_ORANGE);
                }
                if (PLAYER.detectorLevel >= 3){
                    RHN_DrawRect(detectorHUDPos.x+11,detectorHUDPos.y+19,10,9,RHN_COLOR_MAROON);
                }


                if (PLAYER.connectedToGhost){
                    const char* text = WLSN_Get_Value(localization,"press_space");
                    RHN_DrawRect((RHN_GetCanvasWidth()/2)-(strlen(text)*9)/2,RHN_GetCanvasHeight()-15+(sin(tick*2)*2),strlen(text)*9,9,RHN_COLOR_MAROON);
                    RHN_DrawText(font,text,(RHN_GetCanvasWidth()/2)-(strlen(text)*9)/2,RHN_GetCanvasHeight()-15+(sin(tick*2)*2),1,RHN_COLOR_RED);
                }

                if (wantToQuitPrompt){
                    if (PLAYER.catchedGhosts < GHOST_COUNT){
                        const char* abortText = stringf("%s?",WLSN_Get_Value(localization,"abort"));
                        const char* yesText = stringf("%s",WLSN_Get_Value(localization,"yes"));
                        const char* noText = stringf("%s",WLSN_Get_Value(localization,"no"));
                        RHN_DrawRect((RHN_GetCanvasWidth()/2)-75,(RHN_GetCanvasHeight()/2)-50,150,100,RHN_COLOR_OLIVE);
                        RHN_DrawHollowRect( (RHN_GetCanvasWidth()/2)-75,(RHN_GetCanvasHeight()/2)-50,150,100,RHN_COLOR_WHITE,1);
                        RHN_DrawText(font,abortText,(RHN_GetCanvasWidth()/2)-((strlen(abortText)/2)*9),(RHN_GetCanvasHeight()/2)-40,1,RHN_COLOR_WHITE);
                        if (UI_Button(font,yesText,(RHN_GetCanvasWidth()/2)-75+30,(RHN_GetCanvasHeight()/2),1,mousePos,false)|| DW_IsControllerButtonPressed(BUTTON_A)){
                            currentScreen = MAIN_MENU;
                            wantToQuitPrompt = false;
                            MDMX_SOUND_Stop(&SOUND_GAM_IDLE);
                            MDMX_SOUND_Stop(&SOUND_GAM_CLOSE);
                            MDMX_SOUND_Stop(&SOUND_SHOOT);
                        }
                        if (UI_Button(font,noText,(RHN_GetCanvasWidth()/2)+75-(strlen(noText)*9)-30,(RHN_GetCanvasHeight()/2),1,mousePos,false)|| DW_IsControllerButtonPressed(BUTTON_B)){
                            wantToQuitPrompt = false;
                        }
                    }else{
                        const char* endText = stringf("%s!",WLSN_Get_Value(localization,"end_game"));
                        const char* endButtonText = stringf("%s",WLSN_Get_Value(localization,"back_to_menu"));
                        RHN_DrawRect((RHN_GetCanvasWidth()/2)-125,(RHN_GetCanvasHeight()/2)-100,250,150,RHN_COLOR_OLIVE);
                        RHN_DrawHollowRect( (RHN_GetCanvasWidth()/2)-125,(RHN_GetCanvasHeight()/2)-100,250,150,RHN_COLOR_WHITE,1);
                        RHN_DrawText(font,endText,(RHN_GetCanvasWidth()/2)-((strlen(endText)/2)*9),(RHN_GetCanvasHeight()/2)-60,1,RHN_COLOR_WHITE);
                        if (UI_Button(font,endButtonText,(RHN_GetCanvasWidth()/2)-((strlen(endButtonText)/2)*9),(RHN_GetCanvasHeight()/2),1,mousePos,false)|| DW_IsControllerButtonPressed(BUTTON_A)){
                            currentScreen = MAIN_MENU;
                            wantToQuitPrompt = false;
                            MDMX_SOUND_Stop(&SOUND_GAM_IDLE);
                            MDMX_SOUND_Stop(&SOUND_GAM_CLOSE);
                            MDMX_SOUND_Stop(&SOUND_SHOOT);
                        }
                    }
                    
                }

                RHN_DrawTexture(TEXTURE_GHOST,RHN_GetCanvasWidth()-43,20,1,0);
                RHN_DrawText(font,stringf("%i/4",PLAYER.catchedGhosts),RHN_GetCanvasWidth()-27,25,1,RHN_COLOR_WHITE);

                // END GAME RENDER
                break;
            case SETTINGS:
                // START SETTINGS RENDER
                menuColor += DW_GetDeltaTime()/10;
                RHN_Fill(RHN_COLOR_New(140+(sin(menuColor)), 48+(cos(menuColor)*25), 97-(sin(menuColor-2)*25),255));
                RHN_DrawCircle(10+(cos(menuColor)*10),10+(sin(menuColor)*10),10+(sin(menuColor)*2), RHN_COLOR_New(200+(sin(menuColor)*50), 95+(cos(menuColor)*50), 86-(sin(menuColor-2)*50),255));
                RHN_DrawCircle(330+(cos(menuColor)*10),100+(sin(menuColor)*10),30+(sin(menuColor)*2), RHN_COLOR_New(200+(sin(menuColor)*50), 95+(cos(menuColor)*50), 86-(sin(menuColor-2)*50),255));
                RHN_DrawCircle(200+(cos(menuColor)*10),30+(sin(menuColor)*10),50+(sin(menuColor)*2), RHN_COLOR_New(200+(sin(menuColor)*50), 95+(cos(menuColor)*50), 86-(sin(menuColor-2)*50),255));
                RHN_DrawCircle(20+(cos(menuColor)*10),200+(sin(menuColor)*10),50+(sin(menuColor)*2), RHN_COLOR_New(200+(sin(menuColor)*50), 95+(cos(menuColor)*50), 86-(sin(menuColor-2)*50),255));
                if (UI_Button(font,"<",20,20,1,mousePos,false)){
                    currentScreen = MAIN_MENU;
                    SAVE_SetLang(SAVE_Lang);
                    localization = WLSN_Load_From_String(LOCALIZATIONS[SAVE_Lang],false);
                    SAVE_DumpTheSaveFile();
                }
                
                const char* LangText = WLSN_Get_Value(localization,"language");;
                RHN_DrawText(font,LangText,20,50,2,RHN_COLOR_WHITE);
                RHN_TEXTURE flag;
                switch (SAVE_Lang){
                    case ENGLISH:
                        flag = FLAGS_UK;
                        break;
                    case DUTCH:
                        flag = FLAGS_NL;
                        break;
                    case GERMAN:
                        flag = FLAGS_DE;
                        break;
                }
                if (UI_Button(font,"<",(strlen(LangText)*18)+30,50,2,mousePos,false)){
                    if (SAVE_Lang == 0){
                        SAVE_Lang = 2;
                    }else{
                        SAVE_Lang--;
                    }
                    SAVE_DumpTheSaveFile();
                }
                RHN_DrawTexture(flag,(strlen(LangText)*18)+55,52,1,0);
                if (UI_Button(font,">",((strlen(LangText)*18)+(FLAGS_UK.width*2) + 40),50,2,mousePos,false)){
                    if (SAVE_Lang == 2){
                        SAVE_Lang = 0;
                    }else{
                        SAVE_Lang++;
                    }
                    SAVE_DumpTheSaveFile();
                }

                SAVE_SetLang(SAVE_Lang);
                localization = WLSN_Load_From_String(LOCALIZATIONS[SAVE_Lang],false);

                
                // END SETTINGS RENDER
                break;
            case TUTORIAL:
                // START TUTORIAL RENDER
                RHN_Fill(RHN_COLOR_BLACK);

                HOUSE_Render(h,DW_GetWindowWidth(),DW_GetWindowHeight(),cameraPos);

                PLAYER_Render(cameraPos);
                
                
                if (PLAYER.connectedToGhost){
                    int x = 16;
                    int y = 107;
                    RHN_DrawRect(RHN_GetCanvasWidth()-x-2,RHN_GetCanvasHeight()-y-2,14,79,RHN_COLOR_WHITE);
                    RHN_DrawRect(RHN_GetCanvasWidth()-x,RHN_GetCanvasHeight()-y,10,75,RHN_COLOR_RED);
                    RHN_DrawRect(RHN_GetCanvasWidth()-x,(RHN_GetCanvasHeight()-(y-24)+(50-(PLAYER.pullingForce*50))+1),10,(PLAYER.pullingForce*50)+1,RHN_COLOR_OLIVE);
                }
                
                if (turnTheDetectorLightOn){
                    RHN_DrawTexture(TEXTURE_GAM_DETECTOR_ON, detectorHUDPos.x,detectorHUDPos.y,1,0);
                }else{
                    RHN_DrawTexture(TEXTURE_GAM_DETECTOR, detectorHUDPos.x,detectorHUDPos.y,1,0);
                }
                
                if (PLAYER.detectorLevel >= 1){
                    RHN_DrawRect(detectorHUDPos.x+11,detectorHUDPos.y+37,10,9,RHN_COLOR_YELLOW);
                } 
                if (PLAYER.detectorLevel >= 2){
                    RHN_DrawRect(detectorHUDPos.x+11,detectorHUDPos.y+28,10,9,RHN_COLOR_ORANGE);
                }
                if (PLAYER.detectorLevel >= 3){
                    RHN_DrawRect(detectorHUDPos.x+11,detectorHUDPos.y+19,10,9,RHN_COLOR_MAROON);
                }


                if (PLAYER.connectedToGhost){
                    const char* text = WLSN_Get_Value(localization,"press_space");
                    RHN_DrawRect((RHN_GetCanvasWidth()/2)-(strlen(text)*9)/2,RHN_GetCanvasHeight()-15+(sin(tick*2)*2),strlen(text)*9,9,RHN_COLOR_MAROON);
                    RHN_DrawText(font,text,(RHN_GetCanvasWidth()/2)-(strlen(text)*9)/2,RHN_GetCanvasHeight()-15+(sin(tick*2)*2),1,RHN_COLOR_RED);
                }


                if (tutorialStep == 0){
                    const char* pressContinue = WLSN_Get_Value(localization,"T01");
                    const char* line1 = WLSN_Get_Value(localization,"T02");
                    int lineCount = 1;
                    RHN_DrawRect(0,0,RHN_GetCanvasWidth(),9*(lineCount+2),RHN_COLOR_BLACK);
                    RHN_DrawText(font,line1,0,0,1,RHN_COLOR_RED);
                    RHN_DrawText(font,pressContinue,RHN_GetCanvasWidth()-(strlen(pressContinue)*9),(9*(lineCount+2))-9,1,RHN_COLOR_RED);
                    if (DW_IsKeyPressed(KEY_RETURN)||DW_IsControllerButtonPressed(BUTTON_A)){
                        tutorialStep = 1;
                    }
                }else if (tutorialStep == 1){
                    const char* pressContinue = WLSN_Get_Value(localization,"T11");
                    const char* line1 = WLSN_Get_Value(localization,"T12");
                    const char* line2 =  WLSN_Get_Value(localization,"T13");
                    int lineCount = 2;
                    RHN_DrawRect(0,0,RHN_GetCanvasWidth(),9*(lineCount+2),RHN_COLOR_BLACK);
                    RHN_DrawText(font,line1,0,0,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line2,0,9,1,RHN_COLOR_RED);
                    RHN_DrawText(font,pressContinue,RHN_GetCanvasWidth()-(strlen(pressContinue)*9),(9*(lineCount+2))-9,1,RHN_COLOR_RED);
                    if (PLAYER.detectorIsBeingUsed){
                        tutorialStep = 2;
                    }
                }else if (tutorialStep == 2){
                    const char* line1 = WLSN_Get_Value(localization,"T21");
                    const char* line2 =  WLSN_Get_Value(localization,"T22");
                    const char* line3 =  WLSN_Get_Value(localization,"T23");
                    int lineCount = 3;
                    RHN_DrawRect(0,0,RHN_GetCanvasWidth(),9*(lineCount+2),RHN_COLOR_BLACK);
                    RHN_DrawText(font,line1,0,0,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line2,0,9,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line3,0,18,1,RHN_COLOR_RED);
                    if (PLAYER.detectorLevel == 3){
                        tutorialStep = 3;
                    }
                }else if (tutorialStep == 3){
                    const char* line1 = WLSN_Get_Value(localization,"T31");
                    const char* line2 = WLSN_Get_Value(localization,"T32");
                    const char* line3 = WLSN_Get_Value(localization,"T33");
                    const char* line4 = WLSN_Get_Value(localization,"T34");
                    const char* line5 = WLSN_Get_Value(localization,"T35");
                    const char* line6 = WLSN_Get_Value(localization,"T36");
                    int lineCount = 6;
                    RHN_DrawRect(0,0,RHN_GetCanvasWidth(),9*(lineCount+2),RHN_COLOR_BLACK);
                    RHN_DrawText(font,line1,0,0,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line2,0,9,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line3,0,18,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line4,0,27,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line5,0,36,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line6,0,45,1,RHN_COLOR_RED);
                    if (PLAYER.connectedToGhost){
                        tutorialStep = 4;
                    }
                }else if (tutorialStep == 4){
                    const char* line1 = WLSN_Get_Value(localization,"T41");
                    const char* line2 = WLSN_Get_Value(localization,"T42");
                    int lineCount = 2;
                    RHN_DrawRect(0,0,RHN_GetCanvasWidth(),9*(lineCount+2),RHN_COLOR_BLACK);
                    RHN_DrawText(font,line1,0,0,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line2,0,9,1,RHN_COLOR_RED);
                    if (PLAYER.catchedGhosts >= 1){
                        tutorialStep = 5;
                    }
                }else if (tutorialStep == 5){
                    const char* line1 = WLSN_Get_Value(localization,"T51");
                    const char* line2 = stringf("%s : %i/4",WLSN_Get_Value(localization,"T52"),PLAYER.catchedGhosts);
                    int lineCount = 2;
                    RHN_DrawRect(0,0,RHN_GetCanvasWidth(),9*(lineCount+2),RHN_COLOR_BLACK);
                    RHN_DrawText(font,line1,0,0,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line2,0,9,1,RHN_COLOR_RED);
                    if (PLAYER.catchedGhosts >= GHOST_COUNT){
                        tutorialStep = 6;
                    }
                }else if (tutorialStep == 6){
                    const char* line1 = WLSN_Get_Value(localization,"T61");
                    const char* line2 = WLSN_Get_Value(localization,"T62");
                    const char* pressContinue = WLSN_Get_Value(localization,"T63");
                    int lineCount = 2;
                    RHN_DrawRect(0,0,RHN_GetCanvasWidth(),9*(lineCount+2),RHN_COLOR_BLACK);
                    RHN_DrawText(font,line1,0,0,1,RHN_COLOR_RED);
                    RHN_DrawText(font,line2,0,9,1,RHN_COLOR_RED);
                    RHN_DrawText(font,pressContinue,RHN_GetCanvasWidth()-(strlen(pressContinue)*9),(9*(lineCount+2))-9,1,RHN_COLOR_RED);
                    if (DW_IsKeyPressed(KEY_RETURN)||DW_IsControllerButtonPressed(BUTTON_A)){
                        currentScreen = MAIN_MENU;
                        tutorialStep = 0;
                        PLAYER.catchedGhosts = 0;
                        PLAYER.detectorIsBeingUsed = false;
                    }
                }

                if (wantToQuitPrompt){
                    const char* abortText = stringf("%s?",WLSN_Get_Value(localization,"abort"));
                    const char* yesText = stringf("%s",WLSN_Get_Value(localization,"yes"));
                    const char* noText = stringf("%s",WLSN_Get_Value(localization,"no"));
                    RHN_DrawRect((RHN_GetCanvasWidth()/2)-75,(RHN_GetCanvasHeight()/2)-50,150,100,RHN_COLOR_OLIVE);
                    RHN_DrawHollowRect( (RHN_GetCanvasWidth()/2)-75,(RHN_GetCanvasHeight()/2)-50,150,100,RHN_COLOR_WHITE,1);
                    RHN_DrawText(font,abortText,(RHN_GetCanvasWidth()/2)-((strlen(abortText)/2)*9),(RHN_GetCanvasHeight()/2)-40,1,RHN_COLOR_WHITE);
                    if (UI_Button(font,yesText,(RHN_GetCanvasWidth()/2)-75+30,(RHN_GetCanvasHeight()/2),1,mousePos,true) || DW_IsControllerButtonPressed(BUTTON_A)){
                        currentScreen = MAIN_MENU;
                        wantToQuitPrompt = false;
                        MDMX_SOUND_Stop(&SOUND_GAM_IDLE);
                        MDMX_SOUND_Stop(&SOUND_GAM_CLOSE);
                        MDMX_SOUND_Stop(&SOUND_SHOOT);
                        currentScreen = MAIN_MENU;
                        tutorialStep = 0;
                        PLAYER.catchedGhosts = 0;
                        PLAYER.detectorIsBeingUsed = false;
                    }
                    if (UI_Button(font,noText,(RHN_GetCanvasWidth()/2)+75-(strlen(noText)*9)-30,(RHN_GetCanvasHeight()/2),1,mousePos,true) || DW_IsControllerButtonPressed(BUTTON_B)){
                        wantToQuitPrompt = false;
                    }
                }
                // END TUTORIAL RENDER
                break;
        }

        RHN_Submit();
        
    }
    SAVE_DumpTheSaveFile();
    RHN_Release();
    DW_CleanUp();
    return 0;
}
