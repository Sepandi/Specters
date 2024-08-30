#include "assets.h"
#include "sep/Pat.h"
#include "sep/Rhine.h"
#include "sep/MadMax.h"
#include <stdio.h>

PAT_ASSETPACK* GAME_assetpack;

RHN_TEXTURE TEXTURE_MAINMENU;
RHN_TEXTURE TEXTURE_WALL;
RHN_TEXTURE TEXTURE_WALL_CORNER_IN;
RHN_TEXTURE TEXTURE_WALL_CORNER_OUT;
RHN_TEXTURE TEXTURE_FLOOR;
RHN_TEXTURE TEXTURE_MINI_MAP_FRAME;
RHN_TEXTURE TEXTURE_PLAYER;
RHN_TEXTURE TEXTURE_CROSSHAIR;
RHN_TEXTURE TEXTURE_GHOST;
RHN_TEXTURE TEXTURE_FONT_GEMINI_TYPE;
RHN_TEXTURE TEXTURE_GAM_DETECTOR;
RHN_TEXTURE TEXTURE_GAM_DETECTOR_ON;
RHN_TEXTURE FLAGS_UK;
RHN_TEXTURE FLAGS_NL;
RHN_TEXTURE FLAGS_DE;
const char* CONFIG_FONT_GEMINI_TYPE;
const char* LOCALIZATIONS[3];
MDMX_SOUND SOUND_GAM_IDLE;
MDMX_SOUND SOUND_GAM_CLOSE;
MDMX_SOUND SOUND_SHOOT;
MDMX_SOUND SOUND_DEV_LOAD;



void ASSETS_LOAD(){
    #ifdef RHINE_RESOURCE_PATH_NOT_FOUND
        #undef RHINE_RESOURCE_PATH_NOT_FOUND
        RHN_RESOURCE_PATH = RHN_ASSETS_GetAssetPath();
    #endif
    GAME_assetpack = PAT_LoadAssetPack(stringf("%s%s",RHN_ASSETS_GetAssetPath(),"assets.abp"));
    if (!GAME_assetpack) {
        printf("ASSETS : LOAD : Failed to load asset pack. %s\n",stringf("%s%s",RHN_ASSETS_GetAssetPath(),"assets.abp"));
        return;
    }

    TEXTURE_MAINMENU = GAME_GetRhineTexture(GAME_assetpack,"Menu");
    TEXTURE_WALL = RHN_TEXTURE_Ditherize(GAME_GetRhineTexture(GAME_assetpack,"Wall"));
    TEXTURE_WALL_CORNER_IN = RHN_TEXTURE_Ditherize(GAME_GetRhineTexture(GAME_assetpack,"WallCornerIn"));
    TEXTURE_WALL_CORNER_OUT = RHN_TEXTURE_Ditherize(GAME_GetRhineTexture(GAME_assetpack,"WallCornerOut"));
    TEXTURE_FLOOR = RHN_TEXTURE_Ditherize(GAME_GetRhineTexture(GAME_assetpack,"Floor"));
    TEXTURE_MINI_MAP_FRAME = RHN_TEXTURE_Ditherize(GAME_GetRhineTexture(GAME_assetpack,"MiniMapFrame"));
    TEXTURE_PLAYER = GAME_GetRhineTexture(GAME_assetpack,"Player");
    TEXTURE_GHOST = RHN_TEXTURE_Ditherize(GAME_GetRhineTexture(GAME_assetpack,"Ghost"));
    TEXTURE_CROSSHAIR = GAME_GetRhineTexture(GAME_assetpack,"Crosshair");
    TEXTURE_FONT_GEMINI_TYPE = GAME_GetRhineTexture(GAME_assetpack,"GeminiType");
    TEXTURE_GAM_DETECTOR = RHN_TEXTURE_Ditherize(GAME_GetRhineTexture(GAME_assetpack,"GAM_Detector"));
    TEXTURE_GAM_DETECTOR_ON = RHN_TEXTURE_Ditherize(GAME_GetRhineTexture(GAME_assetpack,"GAM_Detector_On"));
    FLAGS_UK = GAME_GetRhineTexture(GAME_assetpack,"UK");
    FLAGS_NL = GAME_GetRhineTexture(GAME_assetpack,"NL");
    FLAGS_DE = GAME_GetRhineTexture(GAME_assetpack,"DE");
    CONFIG_FONT_GEMINI_TYPE = PAT_GetTextByKey(GAME_assetpack,"geminitype")->data;
    LOCALIZATIONS[0] = stringf("%s",PAT_GetTextByKey(GAME_assetpack,"LANG_EN")->data);
    LOCALIZATIONS[1] = stringf("%s",PAT_GetTextByKey(GAME_assetpack,"LANG_NL")->data);
    LOCALIZATIONS[2] = stringf("%s",PAT_GetTextByKey(GAME_assetpack,"LANG_DE")->data);
    SOUND_GAM_IDLE = GAME_GetMDMXSound(GAME_assetpack,"GAMIdle");
    SOUND_GAM_CLOSE = GAME_GetMDMXSound(GAME_assetpack,"GAMClose");
    SOUND_SHOOT = GAME_GetMDMXSound(GAME_assetpack,"Shoot");
    SOUND_DEV_LOAD = GAME_GetMDMXSound(GAME_assetpack,"DevLoad");

}

RHN_TEXTURE GAME_GetRhineTexture(PAT_ASSETPACK* pack,const char* key){
    PAT_IMAGE* image = PAT_GetImageByKey(pack, key);
    if (!image) {
        printf("ASSETS : CONVERT PAT IMAGE TO RHN_TEXTURE : Image with key %s not found.\n", key);
        RHN_TEXTURE empty_texture = {0}; // Return an empty texture
        return empty_texture;
    }

    RHN_TEXTURE out = {image->width, image->height, image->data};
    // Do not free image data here; it is managed by the asset pack
    return out;
}

MDMX_SOUND GAME_GetMDMXSound(PAT_ASSETPACK* pack, const char* key) {
    PAT_SOUND* patSound = PAT_GetSoundByKey(pack,key);
    MDMX_SOUND mdmx_sound = MDMX_SOUND_LoadFromBin(patSound->data,patSound->sampleRate,patSound->channels,patSound->size);
    return mdmx_sound;
}