#pragma once

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdarg.h"
#include "limits.h"
#include <wchar.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#endif 

#include "sep/Wilson.h"

#include "sep/Newton.h"

#include "sep/Darwin.h"

#ifdef _WIN32
#include "windows.h"
#include "GL/gl.h"
#endif

#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
#endif

#define FONT_CHARACTER_COUNT 91

typedef uint32_t uint32;
typedef uint8_t uint8;



//- COLOR

static const uint32 RHN_COLOR_WHITE = 0xFFFFFFFF;
static const uint32 RHN_COLOR_GREEN = 0xFF00FF00;
static const uint32 RHN_COLOR_BLUE = 0xFFFF0000;
static const uint32 RHN_COLOR_RED = 0xFF0000FF;
static const uint32 RHN_COLOR_BLACK = 0xFF000000;
static const uint32 RHN_COLOR_BROWN = 0xFF000044;
static const uint32 RHN_COLOR_PURPLE = 0xFF990099;
static const uint32 RHN_COLOR_ORANGE = 0xFF0080FF;
static const uint32 RHN_COLOR_PINK = 0xFFFF00FF;
static const uint32 RHN_COLOR_YELLOW = 0xFF00FFFF;
static const uint32 RHN_COLOR_CYAN = 0xFFFFFF00;
static const uint32 RHN_COLOR_MAGENTA = 0xFFFF00FF;
static const uint32 RHN_COLOR_TEAL = 0xFF808000;
static const uint32 RHN_COLOR_NAVY = 0xFF800000;
static const uint32 RHN_COLOR_OLIVE = 0xFF008080;
static const uint32 RHN_COLOR_MAROON = 0xFF000080;
static const uint32 RHN_COLOR_SILVER = 0xFFC0C0C0;
static const uint32 RHN_COLOR_GRAY = 0xFF808080;

uint32 RHN_COLOR_New(uint8 r,uint8 g,uint8 b,uint8 a);
float RHN_GetAlpha(uint32 color);
float RHN_GetRed(uint32 pixel);
float RHN_GetGreen(uint32 pixel);
float RHN_GetBlue(uint32 pixel);

//- Recources

char* RHN_ASSETS_GetAssetPath();
extern char* RHN_RESOURCE_PATH;

//- TEXTURE
typedef struct{
    int width;
    int height;
    uint32 *pixels;
}RHN_TEXTURE;

typedef enum {
    D0 = 0,
    D90 = 90,
    D180 = 180,
    D270 = 270
}RHN_ROTATION;

RHN_TEXTURE RHN_TEXTURE_Load(const char* path);
uint32_t    RHN_TEXTURE_GetPixel(RHN_TEXTURE texture,int x,int y);
RHN_TEXTURE RHN_TEXTURE_Flip(RHN_TEXTURE texture,bool horizontal,bool vertical);
RHN_TEXTURE RHN_TEXTURE_Rotate(RHN_TEXTURE texture,RHN_ROTATION rotation);
RHN_TEXTURE RHN_TEXTURE_New(int w,int h);
RHN_TEXTURE RHN_TEXTURE_AddColoredShade(RHN_TEXTURE texture,uint32 color);
RHN_TEXTURE RHN_TEXTURE_Cut(RHN_TEXTURE texture,int x,int y,int w,int h);
RHN_TEXTURE RHN_TEXTURE_Shade(RHN_TEXTURE texture,uint32 color);
RHN_TEXTURE RHN_TEXTURE_COLOR_Replace(RHN_TEXTURE texture,uint32 colorFrom,uint32 colorTo);
RHN_TEXTURE RHN_TEXTURE_Ditherize(RHN_TEXTURE texture);

//- FONT
typedef struct{
    wchar_t character;
    RHN_TEXTURE characterTexture;
}RHN_FONT_CHARACTER;

typedef struct{
    RHN_FONT_CHARACTER fontCharacter[93]; 
}RHN_FONT;

RHN_FONT RHN_FONT_Load(WLSN_CNFG config, RHN_TEXTURE atlas,int charWidth,int charHeight);

RHN_TEXTURE RHN_FONT_FindCharecter(RHN_FONT font,char character);


//- CANVAS
typedef struct{
    uint32 *pixels;
    int width;
    int height;
    GLuint canvasTexture;
}RHN_CANVAS;

void        RHN_CANVAS_New(int width,int height);
int         RHN_GetCanvasWidth();
int         RHN_GetCanvasHeight();
void        RHN_Fill(uint32_t color);
void        RHN_SetPixel(int x,int y,uint32_t color);
void        RHN_Release();
uint32_t    RHN_GetPixel(int x,int y);
void        RHN_DrawRect(int x,int y,int w,int h,uint32 color);
void        RHN_DrawHollowRect(int x, int y, int w, int h, uint32 color, int thickness);
void        RHN_DrawCircle(int centerX,int centerY, float radius, uint32 color);
void        RHN_DrawTexture(RHN_TEXTURE texture, int x,int y,double scale, float rotationDegrees);
void        RHN_DrawTextureMonoTinted(RHN_TEXTURE texture,int x,int y,double scale, float rotationDegrees,uint32 color);
void        RHN_DrawLine(int startX,int startY,int endX,int endY,int width,uint32 color);
void        RHN_DrawText(RHN_FONT font,const char* text,int x,int y,double size,uint32 color);

void        RHN_Submit();

//- UTILS 
NT_VEC2 RHN_GetCanvasMousePos(int windowWidth,int windowHeight,int mouseX,int mouseY);


//- MAIN_CANVAS
extern RHN_CANVAS RHN_MAIN_CANVAS;

// [ RHINE IMPLEMENTATION ]
#ifdef RHINE_IMP
#undef RHINE_IMP

#define RHINE_RESOURCE_PATH_NOT_FOUND

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

char* RHN_RESOURCE_PATH = "";

char* RHN_ASSETS_GetAssetPath() {
    // Define resourcePath as a static variable
    static char resourcePath[1024] = "";

    // If __APPLE__ and not DEBUG, retrieve the bundle path
    #ifdef __APPLE__
        #ifndef DEBUG
            CFBundleRef mainBundle = CFBundleGetMainBundle();
            if (mainBundle) {
                CFURLRef bundleURL = CFBundleCopyBundleURL(mainBundle);
                if (bundleURL) {
                    CFStringRef bundlePath = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
                    if (bundlePath) {
                        // Convert CFStringRef to C string
                        if (CFStringGetCString(bundlePath, resourcePath, PATH_MAX, kCFStringEncodingUTF8)) {
                            // Append the resource path to the bundle path
                            strcat(resourcePath, "/Contents/Resources/");
                        }
                        CFRelease(bundlePath);
                    }
                    CFRelease(bundleURL);
                }
            }
        #endif
    #endif

    // Return the resourcePath as a pointer to a string
    return resourcePath;
}



//- COLOR

uint32 RHN_COLOR_New(uint8 r,uint8 g,uint8 b,uint8 a){
    uint32_t result = 0;
    result |= (uint32_t)r;
    result |= (uint32_t)g << 8;
    result |= (uint32_t)b << 16;
    result |= (uint32_t)a << 24;
    return result;
}


float RHN_GetRed(uint32 pixel) { return pixel & 0xFF; }
float RHN_GetGreen(uint32 pixel) { return (pixel >> 8) & 0xFF; }
float RHN_GetBlue(uint32 pixel) { return (pixel >> 16)  & 0xFF; }
float RHN_GetAlpha(uint32 color){return (color >> 24) & 0xFF;}


//- TEXTURE

RHN_TEXTURE RHN_TEXTURE_Load(const char* path){
    #ifdef RHINE_RESOURCE_PATH_NOT_FOUND
        #undef RHINE_RESOURCE_PATH_NOT_FOUND
        RHN_RESOURCE_PATH = RHN_ASSETS_GetAssetPath();
    #endif
    RHN_TEXTURE texture;
    unsigned char* image = stbi_load(stringf("%s%s",RHN_RESOURCE_PATH,path), &texture.width, &texture.height, 0, STBI_rgb_alpha);
    if (!image) {
        printf("TEXTURE-ERROR : FAILED TO LOAD TEXTURE : %s : %s\n", stringf("%s%s",RHN_RESOURCE_PATH,path), stbi_failure_reason());
    }
    size_t numPixels = texture.width * texture.height;
    
    texture = RHN_TEXTURE_New(texture.width,texture.height);

    for (size_t i = 0; i < numPixels; ++i) {
        unsigned char r = image[i * 4];     // Red channel
        unsigned char g = image[i * 4 + 1]; // Green channel
        unsigned char b = image[i * 4 + 2]; // Blue channel
        unsigned char a = image[i * 4 + 3]; // Alpha channel
        
        texture.pixels[i] = (a << 24) | (b << 16) | (g << 8) | r;
    }

    stbi_image_free(image);
    return texture;
}
uint32_t RHN_TEXTURE_GetPixel(RHN_TEXTURE texture,int x,int y){
    return texture.pixels[(y*texture.width)+x];
}
RHN_TEXTURE RHN_TEXTURE_Flip(RHN_TEXTURE texture,bool horizontal,bool vertical){
    RHN_TEXTURE flipedTexture = RHN_TEXTURE_New(texture.width,texture.height);
    for (int x = 0;x < flipedTexture.width;x++){
        for (int y = 0;y < flipedTexture.height;y++){
            if (vertical && horizontal){
                flipedTexture.pixels[(y*texture.width)+x] = RHN_TEXTURE_GetPixel(texture,texture.width-x-1,texture.height-y-1);
            }else if (horizontal && !vertical){
                flipedTexture.pixels[(y*texture.width)+x] = RHN_TEXTURE_GetPixel(texture,x,texture.height-y-1);
            }else if (!horizontal && vertical) {
                flipedTexture.pixels[(y*texture.width)+x] = RHN_TEXTURE_GetPixel(texture,texture.width-x-1,y);
            }else{
                flipedTexture.pixels[(y*texture.width)+x] = RHN_TEXTURE_GetPixel(texture,x,y);
            }
        }
    }
    
    return flipedTexture;
}

RHN_TEXTURE RHN_TEXTURE_Rotate(RHN_TEXTURE texture, RHN_ROTATION rotation) {
    RHN_TEXTURE rotatedTexture= RHN_TEXTURE_New(texture.height, texture.width);;

    switch (rotation) {
        case D0:
            rotatedTexture = texture;
            break;
        case D90:
            for (int y = 0; y < texture.height; y++) {
                for (int x = 0; x < texture.width; x++) {
                    rotatedTexture.pixels[x * rotatedTexture.width + (rotatedTexture.width - 1 - y)] = texture.pixels[y * texture.width + x];
                }
            }
            break;

        case D180:
            for (int y = 0; y < texture.height; y++) {
                for (int x = 0; x < texture.width; x++) {
                    rotatedTexture.pixels[(rotatedTexture.height - 1 - y) * rotatedTexture.width + (rotatedTexture.width - 1 - x)] = texture.pixels[y * texture.width + x];
                }
            }
            break;

        case D270:
            for (int y = 0; y < texture.height; y++) {
                for (int x = 0; x < texture.width; x++) {
                    rotatedTexture.pixels[(rotatedTexture.height - 1 - x) * rotatedTexture.width + y] = texture.pixels[y * texture.width + x];
                }
            }
            break;
    }

    return rotatedTexture;
}
RHN_TEXTURE RHN_TEXTURE_New(int w,int h){
    return (RHN_TEXTURE){
        .width = w,
        .height = h,
        .pixels = (uint32_t*)malloc(w * h * sizeof(uint32_t))
    };
}
RHN_TEXTURE RHN_TEXTURE_AddColoredShade(RHN_TEXTURE texture,uint32 color){
    RHN_TEXTURE shadedTexture = texture; // Create a copy of the original texture

    for (int y = 0; y < shadedTexture.height; ++y) {
        for (int x = 0; x < shadedTexture.width; ++x) {
            // Get the original pixel color
            uint32 pixelColor = shadedTexture.pixels[(y*shadedTexture.width)+x];
            uint8 originalR = (pixelColor & 0xFF)/255;
            uint8 originalG = ((pixelColor >> 8) & 0xFF)/255;
            uint8 originalB = ((pixelColor >> 16) & 0xFF)/255;
            uint8 originalA = ((pixelColor >> 24) & 0xFF)/255;

            uint8 shadedColorR = (color & 0xFF)/255;
            uint8 shadedColorG = ((color >> 8) & 0xFF)/255;
            uint8 shadedColorB = ((color >> 16) & 0xFF)/255;
            uint8 shadedColorA = ((color >> 24) & 0xFF)/255;

            shadedTexture.pixels[(y*shadedTexture.width)+x] = RHN_COLOR_New((shadedColorR*originalR)*255,(shadedColorG*originalG)*255,(shadedColorB*originalB)*255,(shadedColorA*originalA)*255);
        }
    }

    return shadedTexture;
}
RHN_TEXTURE RHN_TEXTURE_Cut(RHN_TEXTURE texture,int x,int y,int w,int h){
    RHN_TEXTURE cutTexture = RHN_TEXTURE_New(w,h);
    for (int fromX = 0;fromX < w;fromX++){
        for (int fromY = 0;fromY < h;fromY++){
            int item = (fromY*w)+(fromX);
            cutTexture.pixels[item]  = texture.pixels[((y+fromY)*texture.width)+(x+fromX)]; 
        }
    }
    return cutTexture;
}

RHN_TEXTURE RHN_TEXTURE_Ditherize(RHN_TEXTURE texture) {
    int width = texture.width;
    int height = texture.height;
    uint32 *pixels = texture.pixels;

    // Create a new texture for the dithered result
    RHN_TEXTURE dithered = RHN_TEXTURE_New(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint32 pixel = pixels[y * width + x];
            uint8 r = (uint8)RHN_GetRed(pixel);
            uint8 g = (uint8)RHN_GetGreen(pixel);
            uint8 b = (uint8)RHN_GetBlue(pixel);
            uint8 a = (uint8)RHN_GetAlpha(pixel);

            // Slightly vary the pixel's color components to add texture
            int offset = (rand() % 11) - 5; // Random offset between -5 and 5
            r = (uint8)fmin(255, fmax(0, r + offset));
            g = (uint8)fmin(255, fmax(0, g + offset));
            b = (uint8)fmin(255, fmax(0, b + offset));

            // Assign the dithered pixel
            dithered.pixels[y * width + x] = RHN_COLOR_New(r, g, b, a);
        }
    }

    return dithered;
}

RHN_TEXTURE RHN_TEXTURE_Shade(RHN_TEXTURE texture, uint32 color) {
    RHN_TEXTURE newTexture = RHN_TEXTURE_New(texture.width, texture.height);
    for (int i = 0; i < texture.width * texture.height; i++) {
        uint32 textureColor = texture.pixels[i];
        
        uint8 textureR = RHN_GetRed(textureColor);
        uint8 textureG = RHN_GetGreen(textureColor);
        uint8 textureB = RHN_GetBlue(textureColor);
        uint8 textureA = RHN_GetAlpha(textureColor); // Corrected this to get the alpha channel

        uint8 shadeR = RHN_GetRed(color);
        uint8 shadeG = RHN_GetGreen(color);
        uint8 shadeB = RHN_GetBlue(color);
        uint8 shadeA = RHN_GetAlpha(color); // Use the correct function for alpha
        
        // Multiply the color components
        uint8 newR = (textureR * shadeR) / 255;
        uint8 newG = (textureG * shadeG) / 255;
        uint8 newB = (textureB * shadeB) / 255;
        uint8 newA = (textureA * shadeA) / 255;

        newTexture.pixels[i] = RHN_COLOR_New(newR, newG, newB, newA);
    }

    return newTexture;
}
RHN_TEXTURE RHN_TEXTURE_COLOR_Replace(RHN_TEXTURE texture,uint32 colorFrom,uint32 colorTo){
    RHN_TEXTURE newTexture = RHN_TEXTURE_New(texture.width, texture.height);
    for (int i = 0; i < texture.width * texture.height; i++) {
        if (texture.pixels[i] == colorFrom) newTexture.pixels[i] = colorTo; else newTexture.pixels[i] = texture.pixels[i];
    }

    return newTexture;
}

//- FONT

RHN_FONT RHN_FONT_Load(WLSN_CNFG config, RHN_TEXTURE atlas, int charWidth, int charHeight) {
    RHN_FONT font;

    for (int i = 0; i < FONT_CHARACTER_COUNT; i++) {
        font.fontCharacter[i].character = config.data[i].key[0];
        
        int x, y;
        if (sscanf(config.data[i].value, "%d,%d", &x, &y) != 2) {
            fprintf(stderr, "RHN : FONT : LOAD : Failed to parse character coordinates for character %c.\n", font.fontCharacter[i].character);
            continue;
        }
        font.fontCharacter[i].characterTexture = RHN_TEXTURE_Cut(atlas, x * charWidth, y * charHeight, charWidth, charHeight);
    }

    return font;
}

RHN_TEXTURE RHN_FONT_FindCharacter(RHN_FONT font, char character) {
    for (int i = 0; i < FONT_CHARACTER_COUNT; i++) {
        if (font.fontCharacter[i].character == character) {
            return font.fontCharacter[i].characterTexture;
        }
    }
    fprintf(stderr, "Character '%c' not found in font.\n", character);
    return RHN_TEXTURE_New(0, 0); // Return an empty texture or a placeholder
}

//- CANVAS
void RHN_CANVAS_New(int width,int height){
    RHN_MAIN_CANVAS = (RHN_CANVAS){
        .width = width,
        .height = height,
        .pixels = (uint32_t*)malloc(width * height * sizeof(uint32_t))
    };

    // START OPENGL 2.1
    glGenTextures(1, &RHN_MAIN_CANVAS.canvasTexture);
    glBindTexture(GL_TEXTURE_2D, RHN_MAIN_CANVAS.canvasTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    #ifdef __APPLE__
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    #endif
    #ifdef _WIN32
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    #endif

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RHN_MAIN_CANVAS.width, RHN_MAIN_CANVAS.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, RHN_MAIN_CANVAS.pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    // END OPENGL 2.1
}

int RHN_GetCanvasWidth(){
    return RHN_MAIN_CANVAS.width;
}
int RHN_GetCanvasHeight(){
    return RHN_MAIN_CANVAS.height;
}
void RHN_Fill(uint32_t color){
    // Calculate the size of the RHN_MAIN_CANVAS in bytes
    size_t canvasSizeBytes = RHN_MAIN_CANVAS.width * RHN_MAIN_CANVAS.height * sizeof(uint32_t);

    // Treat the RHN_MAIN_CANVAS memory as an array of bytes
    uint8_t* canvasBytes = (uint8_t*)RHN_MAIN_CANVAS.pixels;

    // Use memset to fill the RHN_MAIN_CANVAS memory with the color
    uint32_t colorLE = color; // Assuming little-endian representation
    size_t colorSize = sizeof(uint32_t);
    for (size_t offset = 0; offset < canvasSizeBytes; offset += colorSize) {
        memcpy(canvasBytes + offset, &colorLE, colorSize);
    }
}

void RHN_SetPixel(int x,int y,uint32_t color){
    if (x < RHN_MAIN_CANVAS.width && y < RHN_MAIN_CANVAS.height && x >= 0 && y >= 0){
        if (RHN_GetAlpha(color) >= 255){
            RHN_MAIN_CANVAS.pixels[(RHN_MAIN_CANVAS.width*y)+x] = color;
        }else{
            uint32 bgColor = RHN_GetPixel(x,y);

            
            uint8 bgR = RHN_GetRed(bgColor);
            uint8 bgG = RHN_GetGreen(bgColor);
            uint8 bgB = RHN_GetBlue(bgColor);
            uint8 bgA = RHN_GetAlpha(bgColor);

            uint8 newR = RHN_GetRed(color);
            uint8 newG = RHN_GetGreen(color);
            uint8 newB = RHN_GetBlue(color);
            uint8 newA = RHN_GetAlpha(color);

            float alpha_src = newA / 255.0f;
            float alpha_dst = (1.0f - alpha_src) * (bgA / 255.0f);

            uint8 endR = (newR * alpha_src) + (bgR * alpha_dst);
            uint8 endG = (newG * alpha_src) + (bgG * alpha_dst);
            uint8 endB = (newB * alpha_src) + (bgB * alpha_dst);
            uint8 endA = (newA + alpha_dst) * 255.0;

            RHN_MAIN_CANVAS.pixels[(RHN_MAIN_CANVAS.width*y)+x] = RHN_COLOR_New(endR,endG,endB,endA);
        }
        
    }
}
void RHN_Release(){
    free(RHN_MAIN_CANVAS.pixels);
}
uint32_t RHN_GetPixel(int x,int y){
    return RHN_MAIN_CANVAS.pixels[(RHN_MAIN_CANVAS.width*y)+x];
}
void RHN_DrawRect(int x,int y,int w,int h,uint32 color){
    for (int fromX = x;fromX < x+w;fromX++){
        for (int fromY = y;fromY < y+h;fromY++){
            RHN_SetPixel(fromX,fromY,color);
        }
    }
}

void RHN_DrawHollowRect(int x, int y, int w, int h, uint32 color, int thickness) {
    // Ensure thickness is positive and does not exceed rectangle dimensions
    if (thickness <= 0 || thickness > w || thickness > h) {
        return; // Invalid thickness
    }

    // Draw the top border
    for (int i = 0; i < thickness; i++) {
        for (int fromX = x; fromX < x + w; fromX++) {
            RHN_SetPixel(fromX, y + i, color);
        }
    }

    // Draw the bottom border
    for (int i = 0; i < thickness; i++) {
        for (int fromX = x; fromX < x + w; fromX++) {
            RHN_SetPixel(fromX, y + h - i - 1, color);
        }
    }

    // Draw the left border
    for (int i = 0; i < thickness; i++) {
        for (int fromY = y; fromY < y + h; fromY++) {
            RHN_SetPixel( x + i, fromY, color);
        }
    }

    // Draw the right border
    for (int i = 0; i < thickness; i++) {
        for (int fromY = y; fromY < y + h; fromY++) {
            RHN_SetPixel( x + w - i - 1, fromY, color);
        }
    }
}

void RHN_DrawCircle(int centerX, int centerY, float radius, uint32 color) {
    int x = (int)(radius) - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - ((int)(radius) << 1);

    while (x >= y) {
        // Draw upper part of the circle
        for (int xi = centerX - x; xi <= centerX + x; xi++) {
            RHN_SetPixel(xi, centerY - y, color);
        }

        // Draw lower part of the circle
        for (int xi = centerX - x; xi <= centerX + x; xi++) {
            RHN_SetPixel(xi, centerY + y, color);
        }

        // Draw left part of the circle
        for (int xi = centerX - y; xi <= centerX + y; xi++) {
            RHN_SetPixel(xi, centerY - x, color);
        }

        // Draw right part of the circle
        for (int xi = centerX - y; xi <= centerX + y; xi++) {
            RHN_SetPixel(xi, centerY + x, color);
        }

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - ((int)(radius) << 1);
        }
    }
}
float DegreesToRadians(float degrees) {
    return degrees * (NT_PI / 180.0f);
}


void RHN_DrawTexture(RHN_TEXTURE texture, int x, int y, double scale, float rotationDegrees) {
    // Calculate the center of the texture
    float centerX = (texture.width * scale) / 2.0f;
    float centerY = (texture.height * scale) / 2.0f;

    // Convert rotation degrees to radians
    float rotationRadians = DegreesToRadians(rotationDegrees);
    float cosAngle = cos(rotationRadians);
    float sinAngle = sin(rotationRadians);

    // Iterate through each pixel of the RHN_MAIN_CANVAS where the texture will be drawn
    for (int fromX = x; fromX < x + texture.width * scale; fromX++) {
        for (int fromY = y; fromY < y + texture.height * scale; fromY++) {
            // Calculate the coordinates relative to the center of the texture
            float relativeX = (fromX - x) - centerX;
            float relativeY = (fromY - y) - centerY;

            // Apply the rotation transformation
            float rotatedX = relativeX * cosAngle - relativeY * sinAngle;
            float rotatedY = relativeX * sinAngle + relativeY * cosAngle;

            // Calculate the texture coordinates after rotation
            int texX = (int)(rotatedX + centerX);  // Adjusted to keep within rotated bounds
            int texY = (int)(rotatedY + centerY);  // Adjusted to keep within rotated bounds

            // Check if the calculated position is within the bounds of the original texture
            if (texX >= 0 && texX < texture.width * scale && texY >= 0 && texY < texture.height * scale) {
                // Sample the pixel from the original texture
                int originalTexX = texX / scale;
                int originalTexY = texY / scale;

                // Draw the pixel
                RHN_SetPixel(fromX, fromY, RHN_TEXTURE_GetPixel(texture, originalTexX, originalTexY));
            }
        }
    }
}

void RHN_DrawTextureMonoTinted(RHN_TEXTURE texture,int x,int y,double scale, float rotationDegrees,uint32 color) {
    // Calculate the center of the texture
    float centerX = (texture.width * scale) / 2.0f;
    float centerY = (texture.height * scale) / 2.0f;

    // Convert rotation degrees to radians
    float rotationRadians = DegreesToRadians(rotationDegrees);
    float cosAngle = cos(rotationRadians);
    float sinAngle = sin(rotationRadians);

    // Iterate through each pixel of the RHN_MAIN_CANVAS where the texture will be drawn
    for (int fromX = x; fromX < x + texture.width * scale; fromX++) {
        for (int fromY = y; fromY < y + texture.height * scale; fromY++) {
            // Calculate the coordinates relative to the center of the texture
            float relativeX = (fromX - x) - centerX;
            float relativeY = (fromY - y) - centerY;

            // Apply the rotation transformation
            float rotatedX = relativeX * cosAngle - relativeY * sinAngle;
            float rotatedY = relativeX * sinAngle + relativeY * cosAngle;

            // Calculate the texture coordinates after rotation
            int texX = (int)(rotatedX + centerX);  // Adjusted to keep within rotated bounds
            int texY = (int)(rotatedY + centerY);  // Adjusted to keep within rotated bounds

            // Check if the calculated position is within the bounds of the original texture
            if (texX >= 0 && texX < texture.width * scale && texY >= 0 && texY < texture.height * scale) {
                // Sample the pixel from the original texture
                int originalTexX = texX / scale;
                int originalTexY = texY / scale;

                // Draw the pixel
                if (RHN_GetAlpha(RHN_TEXTURE_GetPixel(texture, originalTexX, originalTexY)) == 255){
                    RHN_SetPixel(fromX, fromY, color);
                }
                
            }
        }
    }
}

void RHN_DrawLine(int startX,int startY,int endX,int endY,int width,uint32 color){
    int deltaX = abs(endX - startX);
    int deltaY = abs(endY - startY);
    int signX = (startX < endX) ? 1 : -1;
    int signY = (startY < endY) ? 1 : -1;
    int error = deltaX - deltaY;
    int error2;

    int halfWidth = width / 2;

    while (true) {
        // Draw a thick line by setting pixels in a square around the main line
        for (int dy = -halfWidth; dy <= halfWidth; ++dy) {
            for (int dx = -halfWidth; dx <= halfWidth; ++dx) {
                RHN_SetPixel(startX + dx, startY + dy, color);
            }
        }

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

void RHN_DrawText(RHN_FONT font, const char* text, int x, int y, double size, uint32_t color) {
    size_t textLen = strlen(text);
    for (size_t i = 0; i < textLen; i++) {
        if (text[i] != ' ') {
            RHN_TEXTURE character = RHN_FONT_FindCharacter(font, text[i]);
            RHN_DrawTextureMonoTinted(character, x + (i * size * 9), y,size, 0,color);
        }
        
        
    }
}

void RHN_Submit(){
    int windowWidth = DW_GetWindowWidth();
    int windowHeight = DW_GetWindowHeight();
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,windowWidth,windowHeight, 0, 1, -2);
    
    glBindTexture(GL_TEXTURE_2D, RHN_MAIN_CANVAS.canvasTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RHN_MAIN_CANVAS.width, RHN_MAIN_CANVAS.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, RHN_MAIN_CANVAS.pixels);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);

    float textureAspect = (float)RHN_MAIN_CANVAS.width / RHN_MAIN_CANVAS.height;
    float screenAspect = (float)windowWidth / windowHeight;
    float newWidth, newHeight;
    float xOffset = 0, yOffset = 0;
    if (textureAspect > screenAspect) {
        // Texture is wider than the screen
        newWidth = windowWidth;
        newHeight = windowWidth / textureAspect;
        yOffset = (windowHeight - newHeight) / 2;
    } else {
        // Texture is taller than the screen
        newWidth = windowHeight * textureAspect;
        newHeight = windowHeight;
        xOffset = (windowWidth - newWidth) / 2;
    }

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2d(xOffset, yOffset);
    glTexCoord2f(1, 0); glVertex2d(xOffset + newWidth, yOffset);
    glTexCoord2f(1, 1); glVertex2d(xOffset + newWidth, yOffset + newHeight);
    glTexCoord2f(0, 1); glVertex2d(xOffset, yOffset + newHeight);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, 0);

}

//- UTILS
NT_VEC2 RHN_GetCanvasMousePos(int windowWidth, int windowHeight, int mouseX, int mouseY) {
    float textureAspect = (float)RHN_MAIN_CANVAS.width / RHN_MAIN_CANVAS.height;
    float screenAspect = (float)windowWidth / windowHeight;
    float newWidth, newHeight;
    float xOffset = 0, yOffset = 0;

    if (textureAspect > screenAspect) {
        // Texture is wider than the screen
        newWidth = windowWidth;
        newHeight = windowWidth / textureAspect;
        yOffset = (windowHeight - newHeight) / 2;
    } else {
        // Texture is taller than the screen
        newWidth = windowHeight * textureAspect;
        newHeight = windowHeight;
        xOffset = (windowWidth - newWidth) / 2;
    }

    // Adjust mouse coordinates from window space to framebuffer space
    float framebufferX = (float)(mouseX - xOffset) / newWidth * RHN_MAIN_CANVAS.width;
    float framebufferY = (float)(mouseY - yOffset) / newHeight * RHN_MAIN_CANVAS.height;

    return (NT_VEC2){framebufferX, framebufferY};
}

//- MAIN_CANVAS
RHN_CANVAS RHN_MAIN_CANVAS;
#endif

