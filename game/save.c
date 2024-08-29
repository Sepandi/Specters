#include "save.h"
#include "sep/Wilson.h"
#include "sep/Rhine.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir  // Windows-specific mkdir does not take a mode argument
#define PATH_SEPARATOR '\\'
#else
#include <unistd.h>
#define PATH_SEPARATOR '/'
#endif

WLSN_CNFG SAVE_FILE;
const char* pathToFolder;
LANG SAVE_Lang;

void createDirectory(const char *path) {
    char temp[256];
    char *p = NULL;
    size_t len;

    // Copy the path to a temporary buffer
    snprintf(temp, sizeof(temp), "%s", path);
    len = strlen(temp);
    
    // Remove trailing slash if present
    if (temp[len - 1] == PATH_SEPARATOR)
        temp[len - 1] = '\0';

    // Iterate through the path and create directories as needed
    for (p = temp + 1; *p; p++) {
        if (*p == PATH_SEPARATOR) {
            *p = '\0';

            #ifdef _WIN32
            mkdir(temp);  // Windows mkdir doesn't take mode
            #else
            mkdir(temp, 0700);  // Unix-like mkdir with mode
            #endif

            *p = PATH_SEPARATOR;
        }
    }

    // Create the final directory
    #ifdef _WIN32
    mkdir(temp);
    #else
    mkdir(temp, 0700);
    #endif
}

void SAVE_CreateFolder(){

    #ifdef __APPLE__        
        pathToFolder = stringf("%s/Library/Application Support",getenv("HOME"));
    #endif
    #ifdef _WIN32
        pathToFolder = stringf("%s",getenv("APPDATA"));
    #endif
    const char* companyFolder = stringf("%s/%s",pathToFolder,COMPANY_NAME);
    createDirectory(companyFolder);

    const char* gameFolder = stringf("%s/%s/%s",pathToFolder,COMPANY_NAME,GAME_NAME);
    createDirectory(gameFolder);
    
}
void SAVE_LoadSave(){
    SAVE_CreateFolder();
    const char* saveFilePath = stringf("%s/%s/%s/%s",pathToFolder,COMPANY_NAME,GAME_NAME,"SAVE");     
    FILE *file = fopen(saveFilePath, "r");
    if (file) {
        // File exists
        SAVE_FILE = WLSN_Load_From_File(saveFilePath,false);
        fclose(file);
    } else {
        // File does not exist
        const char* defaultSave = "\"LANG\"=\"EN\"\n\"VOLUME\"=\"1\"\n";
        SAVE_FILE = WLSN_Load_From_String(defaultSave,false);
        WLSN_Dump(SAVE_FILE,saveFilePath);
    }
    fclose(file);
}
void SAVE_DumpTheSaveFile(){
    const char* saveFilePath = stringf("%s/%s/%s/%s",pathToFolder,COMPANY_NAME,GAME_NAME,"SAVE");     
    WLSN_Dump(SAVE_FILE,saveFilePath);
}

void SAVE_GetLang(){
    SAVE_Lang = ENGLISH;
    const char* savelang = WLSN_Get_Value(SAVE_FILE,"LANG");
    if (strcmp(savelang,"NL") == 0){
        SAVE_Lang = DUTCH;
    }else if (strcmp(savelang,"DE") == 0){
        SAVE_Lang = GERMAN;
    }
}

void SAVE_SetLang(LANG language){
    SAVE_Lang = language;
    const char *languageName = "";
    switch (SAVE_Lang) {
        case ENGLISH:
            languageName = "EN";
            break;
        case DUTCH:
            languageName = "NL";
            break;
        case GERMAN:
            languageName = "DE";
            break;
    }
    WLSN_Set_Value(&SAVE_FILE,"LANG",languageName);
}