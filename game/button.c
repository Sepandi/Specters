#include "button.h"
#include "sep/Rhine.h"

#include <stdbool.h>
#include <string.h>


bool UI_Button(RHN_FONT font,const char *title,int x,int y,int size,NT_VEC2 mousePos,bool effect){
    float margin = 3;
    bool clicked = false;
    NT_RECT rect = {x-margin,y-margin,(strlen(title)*(9*size))+(margin*2),(9*size)+(margin+2)};
    if (NT_RECT_IsCollidingWithPoint(rect,mousePos)){
        RHN_DrawHollowRect(rect.x,rect.y,rect.w,rect.h,RHN_COLOR_New(206,206,206,255),1);
        if (effect)
            RHN_DrawText(font,title,x-1,y-1,size,RHN_COLOR_GRAY);
        RHN_DrawText(font,title,x,y,size,RHN_COLOR_WHITE);

        if (DW_IsMouseKeyPressed(MOUSE_KEY_LEFT)){
            clicked = true;
        }
    }else{
        if (effect)
            RHN_DrawText(font,title,x-1,y-1,size,RHN_COLOR_GRAY);
        RHN_DrawText(font,title,x,y,size,RHN_COLOR_WHITE);
    }
    return clicked;
}