#pragma once

#include "sep/Wilson.h"

#define COMPANY_NAME "Sepano"
#define GAME_NAME "Specters"


typedef enum{
    ENGLISH = 0,DUTCH = 1,GERMAN = 2
}LANG;

extern LANG SAVE_Lang;

extern WLSN_CNFG SAVE_FILE;

void SAVE_CreateFolder();
void SAVE_LoadSave();
void SAVE_DumpTheSaveFile();

void SAVE_GetLang();
void SAVE_SetLang(LANG language);