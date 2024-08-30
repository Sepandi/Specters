#pragma once

#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <unistd.h>
#define PATH_SEPARATOR '/'

#endif 

#ifdef _WIN32
#include "windows.h"
#include "GL/gl.h"
#include <pdh.h>
#include <stdio.h>
#include <direct.h>
#define mkdir _mkdir  // Windows-specific mkdir does not take a mode argument
#define PATH_SEPARATOR '\\'


#endif

#ifdef __cplusplus
extern "C" {
#endif


char* stringf(const char* format, ...);

int CMAIN();

typedef enum{
    MOUSE_KEY_LEFT,MOUSE_KEY_RIGHT,MOUSE_KEY_MIDDLE
}MOUSE_KEY_CODE;

#ifdef __APPLE__
typedef enum{
    UNKNOWN = 0,
    KEY_A = 0,
    KEY_B = 11,
    KEY_C = 8,
    KEY_D = 2,
    KEY_E = 14,
    KEY_F = 3,
    KEY_G = 5,
    KEY_H = 4,
    KEY_I = 34,
    KEY_J = 38,
    KEY_K = 40,
    KEY_L = 37,
    KEY_M = 46,
    KEY_N = 45,
    KEY_O = 31,
    KEY_P = 35,
    KEY_Q = 12,
    KEY_R = 15,
    KEY_S = 1,
    KEY_T = 17,
    KEY_U = 32,
    KEY_V = 9,
    KEY_W = 13,
    KEY_X = 7,
    KEY_Y = 16,
    KEY_Z = 6,

    KEY_1 = 18,
    KEY_2 = 19,
    KEY_3 = 20,
    KEY_4 = 21,
    KEY_5 = 23,
    KEY_6 = 22,
    KEY_7 = 26,
    KEY_8 = 28,
    KEY_9 = 25,
    KEY_0 = 29,

    KEY_RETURN = 36,
    KEY_ESCAPE = 53,
    KEY_BACKSPACE = 51,
    KEY_TAB = 48,
    KEY_SPACE = 49,

    KEY_MINUS = 27,
    KEY_EQUALS = 24,
    KEY_LEFTBRACKET = 33,
    KEY_RIGHTBRACKET = 30,
    KEY_BACKSLASH = 42,
    KEY_NONUSHASH = 50,
    KEY_SEMICOLON = 41,
    KEY_APOSTROPHE = 39,
    KEY_GRAVE = 50,
    KEY_COMMA = 43,
    KEY_PERIOD = 47,
    KEY_SLASH = 44,

    KEY_CAPSLOCK = 57,

    KEY_F1 = 122,
    KEY_F2 = 120,
    KEY_F3 = 99,
    KEY_F4 = 118,
    KEY_F5 = 96,
    KEY_F6 = 97,
    KEY_F7 = 98,
    KEY_F8 = 100,
    KEY_F9 = 101,
    KEY_F10 = 109,
    KEY_F11 = 103,
    KEY_F12 = 111,

    KEY_ARROW_RIGHT = 124,
    KEY_ARROW_LEFT = 123,
    KEY_ARROW_DOWN = 125,
    KEY_ARROW_UP = 126,

    KEY_LCTRL = 224,
    KEY_LSHIFT = 225,
    KEY_RCTRL = 228,
    KEY_RSHIFT = 229,
}KEY_CODE;
#endif

#ifdef _WIN32
typedef enum{
    UNKNOWN = 0,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,

    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_0 = 48,

    KEY_RETURN = 13,
    KEY_ESCAPE = 27,
    KEY_BACKSPACE = 8,
    KEY_TAB = 9,
    KEY_SPACE = 32,

    KEY_MINUS = 189,
    KEY_EQUALS = 187,
    KEY_LEFTBRACKET = 221,
    KEY_RIGHTBRACKET = 219,
    KEY_BACKSLASH = 220,
    KEY_NONUSHASH = 50,
    KEY_SEMICOLON = 186,
    KEY_APOSTROPHE = 222,
    KEY_GRAVE = 226,
    KEY_COMMA = 188,
    KEY_PERIOD = 190,
    KEY_SLASH = 191,

    KEY_CAPSLOCK = 20,

    KEY_F1 = 112,
    KEY_F2 = 113,
    KEY_F3 = 114,
    KEY_F4 = 115,
    KEY_F5 = 116,
    KEY_F6 = 117,
    KEY_F7 = 118,
    KEY_F8 = 119,
    KEY_F9 = 120,
    KEY_F10 = 121,
    KEY_F11 = 122,
    KEY_F12 = 123,

    KEY_ARROW_RIGHT = 39,
    KEY_ARROW_LEFT = 37,
    KEY_ARROW_DOWN = 40,
    KEY_ARROW_UP = 28,

    KEY_LCTRL = 17,
    KEY_LSHIFT = 16,
    KEY_RCTRL = 17,
    KEY_RSHIFT = 16,

}KEY_CODE;

#endif


void DW_WINDOW_New(int width,int height,const char* title);

void DW_SetResizable(bool resizable);
void DW_SetWindowTitle(const char* title);    
void DW_SetIcon(const char* path);
void DW_SetFullscreen(bool fullscreen);
void DW_HideCursor(bool hideMouse);
int DW_GetFPS();
double DW_GetDeltaTime();

bool DW_IsRunning();

void DW_CreateDirectory(const char *path);

extern const char* DW_SUPPORT_FOLDER_PATH;
void DW_CreateSupportFolder(const char* companyName,const char* gameName);

const char* DW_GetSupportFolder();

void DW_CleanUp();

void DW_ForceQuit();

int DW_GetWindowWidth();
int DW_GetWindowHeight();

int DW_GetMouseX();
int DW_GetMouseY();

extern bool DW_MOUSE_KeyPressedState[3];
extern bool DW_MOUSE_KeyDownState[3];

bool DW_IsMouseKeyDown(MOUSE_KEY_CODE mouseKey);
bool DW_IsMouseKeyPressed(MOUSE_KEY_CODE mouseKey);

extern bool DW_KEYBOARD_KeyPressedState[349];
extern bool DW_KEYBOARD_KeyDownState[349];

bool DW_IsKeyDown(KEY_CODE keycode);
bool DW_IsKeyPressed(KEY_CODE keycode);

#ifdef _WIN32
    extern HWND DW_HWND;
#endif

#ifdef DARWIN_IMP
#undef DARWIN_IMP
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <stdbool.h>

const char* DW_SUPPORT_FOLDER_PATH;

double deltaTime = 0;
float FPS;
int frameCount;

bool DW_MOUSE_KeyPressedState[3];
bool DW_MOUSE_KeyDownState[3];

bool DW_KEYBOARD_KeyPressedState[349];
bool DW_KEYBOARD_KeyDownState[349];

char* stringf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    size_t size = vsnprintf(NULL, 0, format, args) + 1; 
    char* formatted = (char*)malloc(size);
    if (formatted == NULL) {
        va_end(args);
        return NULL; 
    }
    vsnprintf(formatted, size, format, args);
    va_end(args);

    return formatted;
}


#ifdef __APPLE__ // FOR MAC OS

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <AppKit/AppKit.h>


NSWindow * window;
NSSize windowSize;
NSPoint mousePos;

void ToggleFullscreen(NSWindow *window) {
    if ([window styleMask] & NSWindowStyleMaskFullScreen) {
        // Exit fullscreen mode
        [window toggleFullScreen:nil];
    } else {
        // Enter fullscreen mode
        [window toggleFullScreen:nil];
    }
}


@class View;
static CVReturn GlobalDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);

@interface View : NSOpenGLView <NSWindowDelegate> {
@public
	CVDisplayLinkRef displayLink;
	bool running;
	NSRect windowRect;
	NSRecursiveLock* appLock;
    NSTrackingArea *trackingArea;
}    
@end


@implementation View
// Initialize
- (id) initWithFrame: (NSRect) frame {
	running = true;
	
	// No multisampling
	int samples = 0;

	// Keep multisampling attributes at the start of the attribute lists since code below assumes they are array elements 0 through 4.
	NSOpenGLPixelFormatAttribute windowedAttrs[] = 
	{
		NSOpenGLPFAMultisample,
		NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)(samples ? 1 : 0),
		NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)(samples),
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 32,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
		0
	};

	// Try to choose a supported pixel format
	NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:windowedAttrs];

	if (!pf) {
		NSLog(@"OpenGL pixel format not supported.");
		return nil;
	}
	
	self = [super initWithFrame:frame pixelFormat:[pf autorelease]];
	appLock = [[NSRecursiveLock alloc] init];

    trackingArea = [[NSTrackingArea alloc] initWithRect:self.bounds
                                                    options:(NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow | NSTrackingInVisibleRect | NSTrackingEnabledDuringMouseDrag)
                                                      owner:self
                                                   userInfo:nil];
    [self addTrackingArea:trackingArea];

	return self;
}

- (IBAction)toggleFullscreen:(id)sender {
    ToggleFullscreen(window);
}

- (void) prepareOpenGL {
	[super prepareOpenGL];
		
	[[self window] setLevel: NSNormalWindowLevel];
	[[self window] makeKeyAndOrderFront: self];
	
	// Make all the OpenGL calls to setup rendering and build the necessary rendering objects
	[[self openGLContext] makeCurrentContext];
	// Synchronize buffer swaps with vertical refresh rate
	GLint swapInt = 0; // Vsync off!
	[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLContextParameterSwapInterval];
	
	// Create a display link capable of being used with all active displays
	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	
	// Set the renderer output callback function
	CVDisplayLinkSetOutputCallback(displayLink, &GlobalDisplayLinkCallback, self);
	
	CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
	CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
	
	GLint dim[2] = {(GLint)windowRect.size.width, (GLint)windowRect.size.height};
	CGLSetParameter(cglContext, kCGLCPSurfaceBackingSize, dim);
	CGLEnable(cglContext, kCGLCESurfaceBackingSize);
	
	[appLock lock];
	CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
	// Temp
	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
	glViewport(0, 0, windowRect.size.width, windowRect.size.height);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
	// End temp
	CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]); 
	[appLock unlock];

	// Activate the display link
	CVDisplayLinkStart(displayLink);

    NSRect bounds = [self bounds];
    [[self openGLContext] makeCurrentContext];
    glViewport(0, 0, NSWidth(bounds), NSHeight(bounds));
}

// Tell the window to accept input events
- (BOOL)acceptsFirstResponder {
	return YES;
}

// Update
- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime {
	return kCVReturnSuccess;
}



// Add observers for fullscreen state changes
- (void)observeWindowNotifications {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowDidEnterFullscreen:)
                                                 name:NSWindowDidEnterFullScreenNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowDidExitFullscreen:)
                                                 name:NSWindowDidExitFullScreenNotification
                                               object:nil];
}

// Handle fullscreen enter event
- (void)windowDidEnterFullscreen:(NSNotification *)notification {
    [self updateViewportForCurrentSize];
}

// Handle fullscreen exit event
- (void)windowDidExitFullscreen:(NSNotification *)notification {
    [self updateViewportForCurrentSize];
}

// Update viewport with current window size
- (void)updateViewportForCurrentSize {
    NSSize newSize = [[[self window] contentView] frame].size;
    
    [appLock lock];
    [[self openGLContext] makeCurrentContext];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    
    glViewport(0, 0, newSize.width, newSize.height);
    
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    [appLock unlock];
}

// Resize
- (void)windowDidResize:(NSNotification*)notification {
    [self updateViewportForCurrentSize];
    NSSize newSize = [[[notification object] contentView] frame].size;
    
    [appLock lock];
    [[self openGLContext] makeCurrentContext];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    
    // Update the window size
    windowSize.width = newSize.width;
    windowSize.height = newSize.height;
    //NSLog(@"Window resize: %lf, %lf", windowSize.width, windowSize.height);
    
    // Update the OpenGL viewport
    glViewport(0, 0, newSize.width, newSize.height);
    
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    [appLock unlock];
}


- (void)reshape {
    [super reshape];

    // Update the OpenGL viewport to the new size
    glViewport(0, 0, windowSize.width, windowSize.height);

}


- (void)resumeDisplayRenderer  {
	[appLock lock];
	CVDisplayLinkStop(displayLink);
	[appLock unlock]; 
}

- (void)haltDisplayRenderer  {
	[appLock lock];
	CVDisplayLinkStop(displayLink);
	[appLock unlock];
}

// Terminate window when the red X is pressed
-(void)windowWillClose:(NSNotification *)notification {
	running = false;
}

// Cleanup
- (void) dealloc {   
	[appLock release];
    if (displayLink) {
        CVDisplayLinkRelease(displayLink);
        displayLink = NULL;
    }
    [super dealloc];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

// MOUSE

- (void)mouseMoved:(NSEvent*) event {
	mousePos = [self convertPoint:[event locationInWindow] fromView:nil];
}

- (void) mouseDown: (NSEvent*) event {
	if (!DW_MOUSE_KeyDownState[0]) {
        DW_MOUSE_KeyPressedState[0] = true;
	}
    DW_MOUSE_KeyDownState[0] = true;
    [self trackMouse:event];
}

- (void) mouseUp: (NSEvent*) event {
    DW_MOUSE_KeyDownState[0] = false;
    [self trackMouse:event];
}

- (void)mouseDragged:(NSEvent *)event {
    [self trackMouse:event];
}


- (void) rightMouseDown: (NSEvent*) event {
	if (!DW_MOUSE_KeyDownState[1]) {
        DW_MOUSE_KeyPressedState[1] = true;
	}
    DW_MOUSE_KeyDownState[1] = true;
}

- (void) rightMouseUp: (NSEvent*) event {
    DW_MOUSE_KeyDownState[1] = false;
}

- (void)otherMouseDown: (NSEvent*) event {
	if (!DW_MOUSE_KeyDownState[2]) {
        DW_MOUSE_KeyPressedState[2] = true;
	}
    DW_MOUSE_KeyDownState[2] = true;
}

- (void)otherMouseUp: (NSEvent*) event {
    DW_MOUSE_KeyDownState[2] = false;
}

- (void) keyDown: (NSEvent*) event {
	if ([event isARepeat] == NO) {
        DW_KEYBOARD_KeyPressedState[[event keyCode]] = true;
	}
    DW_KEYBOARD_KeyDownState[[event keyCode]] = true;
}

- (void) keyUp: (NSEvent*) event {
    DW_KEYBOARD_KeyPressedState[[event keyCode]] = false;
    DW_KEYBOARD_KeyDownState[[event keyCode]] = false;
}

- (void)trackMouse:(NSEvent *)event {
    mousePos = [self convertPoint:[event locationInWindow] fromView:nil];

    // Continue tracking the mouse while button is held down
}

- (void)setupDisplayLink {
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink, &GlobalDisplayLinkCallback, self);
    
    // Add the display link to the appropriate run loop
    CVDisplayLinkStart(displayLink);
}



@end

View* view;
NSAutoreleasePool * pool;

static CVReturn GlobalDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext) {
    CVReturn result = [(View*)displayLinkContext getFrameForTime:outputTime];

    // Calculate delta time in seconds
    static double lastTime = 0.0;
    double currentTime = (double)now->hostTime / (double)now->videoTimeScale;
    
    if (lastTime == 0.0) {
        lastTime = currentTime;
    }

    deltaTime = (currentTime - lastTime);
    lastTime = currentTime;

    // Update FPS calculation
    static double fpsRefreshRate = 1.0; // Update FPS every second
    static double fpsTimer = 0.0;
    static int frameCount = 0;

    frameCount++;
    fpsTimer += deltaTime;

    if (fpsTimer >= fpsRefreshRate) {
        FPS = (double)frameCount / fpsTimer;
        fpsTimer = 0.0;
        frameCount = 0;
    }

    return result;
}

#endif // END FOR MAC



#ifdef _WIN32
    #include "windows.h"
    #include "GL/gl.h"
    #pragma comment (lib, "kernel32")
    #pragma comment (lib, "user32")
    #pragma comment (lib, "shell32")    /* CommandLineToArgvW, DragQueryFileW, DragFinished */
    #pragma comment (lib, "gdi32")
    #pragma comment (lib, "opengl32")
    #pragma comment(lib, "pdh.lib")


    #define IDI_ICON1 ICON "icon.ico"


    int winWidth,winHeight;
    POINT mousePos;


    #ifdef _WIN32
        HWND DW_HWND;
    #endif
    MSG msg;
    HDC hdc;
    bool running = true;
    LARGE_INTEGER frequency;
    LARGE_INTEGER currentTime;
    LARGE_INTEGER previousTime;
    double elapsedTime = 0.0;
    RECT oldWindowRect; // FOR WHEN WE GO BACK TO WINDOWED MODE AFTER A FULLSCREEN


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message){
        case WM_CREATE:
            
            return 0;
        case WM_MOUSEMOVE:
            GetCursorPos(&mousePos);

            // Convert the screen coordinates to client coordinates
            ScreenToClient(hWnd, &mousePos);
            return 0;
        case WM_SIZE:
            winWidth = LOWORD(lParam);
            winHeight = HIWORD(lParam);
            glViewport(0,0,winWidth,winHeight);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (!DW_KEYBOARD_KeyDownState[wParam]){
                DW_KEYBOARD_KeyPressedState[wParam] = true;
            }
            DW_KEYBOARD_KeyDownState[wParam] = true;
            return 0;
        case WM_KEYUP:
            DW_KEYBOARD_KeyDownState[wParam] = false;
            DW_KEYBOARD_KeyPressedState[wParam] = false;
            return 0;
        case WM_LBUTTONDOWN:
            if (!DW_MOUSE_KeyDownState[0]){
                DW_MOUSE_KeyPressedState[0] = true;
            }
            DW_MOUSE_KeyDownState[0] = true;
            return 0;
        case WM_LBUTTONUP:
            DW_MOUSE_KeyPressedState[0] = false;
            DW_MOUSE_KeyDownState[0] = false;
            return 0;
        case WM_RBUTTONDOWN:
            if (!DW_MOUSE_KeyDownState[1]){
                DW_MOUSE_KeyPressedState[1] = true;
            }
            DW_MOUSE_KeyDownState[1] = true;
            return 0;
        case WM_RBUTTONUP:
            DW_MOUSE_KeyPressedState[1] = false;
            DW_MOUSE_KeyDownState[1] = false;
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

// Set up a pixel format for the device context
void DarwinSetPixelFormat(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int format = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, format, &pfd);
}


// Initialize OpenGL
void InitOpenGL(HDC hdc) {
    DarwinSetPixelFormat(hdc);

    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

}

#endif




// MULTIPLATFORM PART

void DW_WINDOW_New(int width,int height,const char* title){

    
    #ifdef __APPLE__
        
        // Autorelease Pool: 
        // Objects declared in this scope will be automatically 
        // released at the end of it, when the pool is "drained". 
        pool = [[NSAutoreleasePool alloc] init]; 
    
        // Create a shared app instance. 
        // This will initialize the global variable 
        // 'NSApp' with the application instance. 
        [NSApplication sharedApplication]; 
    
        // Create a window: 
    
        // Style flags 
        NSUInteger windowStyle = NSWindowStyleMaskTitled  | 
                            NSWindowStyleMaskClosable | 
                            NSWindowStyleMaskResizable;

        // Window bounds (x, y, width, height) 
        NSRect screenRect = [[NSScreen mainScreen] frame];
        NSRect viewRect = NSMakeRect(0, 0, width, height); 
        NSRect windowRect = NSMakeRect(NSMidX(screenRect) - NSMidX(viewRect),
                                    NSMidY(screenRect) - NSMidY(viewRect),
                                    viewRect.size.width, 
                                    viewRect.size.height);
        windowSize = viewRect.size;

        window = [[NSWindow alloc] initWithContentRect:windowRect 
                            styleMask:windowStyle 
                            backing:NSBackingStoreBuffered 
                            defer:NO]; 

        [window autorelease]; 

        GLint swapInt = 0; // Disable V-Sync
        [[view openGLContext] setValues:&swapInt forParameter:NSOpenGLContextParameterSwapInterval];

    
        // Window controller 
        NSWindowController * windowController = [[NSWindowController alloc] initWithWindow:window]; 
        [windowController autorelease]; 

        // Since Snow Leopard, programs without application bundles and Info.plist files don't get a menubar 
        // and can't be brought to the front unless the presentation option is changed
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        // Next, we need to create the menu bar. You don't need to give the first item in the menubar a name 
        // (it will get the application's name automatically)
        id menubar = [[NSMenu new] autorelease];
        id appMenuItem = [[NSMenuItem new] autorelease];
        [menubar addItem:appMenuItem];
        [NSApp setMainMenu:menubar];

        // Then we add the quit item to the menu. Fortunately the action is simple since terminate: is 
        // already implemented in NSApplication and the NSApplication is always in the responder chain.
        id appMenu = [[NSMenu new] autorelease];
        id appName = [NSString stringWithUTF8String:"Game"];
        id quitTitle = [@"Quit " stringByAppendingString:appName];
        id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
            action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
        [appMenu addItem:quitMenuItem];
        [appMenuItem setSubmenu:appMenu];

        // Create app delegate to handle system events
        view = [[[View alloc] initWithFrame:windowRect] autorelease];
        view->windowRect = windowRect;
        [window setAcceptsMouseMovedEvents:YES];
        [window setContentView:view];
        [window setDelegate:view];
        [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];

        


        // Set app title
        [window setTitle:[NSString stringWithUTF8String:title]];

        // Add fullscreen button
        [window setCollectionBehavior: NSWindowCollectionBehaviorFullScreenPrimary];

        // Show window and run event loop
        [window makeKeyAndOrderFront:nil]; 
        [NSApp activateIgnoringOtherApps:YES]; 

        

        // Show window and run event loop 
        [window orderFrontRegardless]; 

        [[view openGLContext] makeCurrentContext];

    #endif
    #ifdef _WIN32

    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "DarwinClass";
    RegisterClass(&wc);

    DW_HWND = CreateWindow(
        wc.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);

    hdc = GetDC(DW_HWND);

    InitOpenGL(hdc);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&previousTime);

    SetCursor(LoadCursor(NULL, IDC_ARROW));


    
    #endif

}

void DW_HideCursor(bool hide){
    #ifdef __APPLE__
        if (hide){
            [NSCursor hide];
        }else{
            [NSCursor unhide];
        }
    #endif

    #ifdef _WIN32
        ShowCursor(!hide);
    #endif
}

void DW_SetIcon(const char* path) {
    #ifdef _WIN32
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Load icon for both big (taskbar) and small (window) icons
    HICON hIconBig = (HICON)LoadImage(hInstance, path, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    HICON hIconSmall = (HICON)LoadImage(hInstance, path, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

    if (hIconBig && hIconSmall) {
        // Set icon for the window (ICON_SMALL)
        SendMessage(DW_HWND, WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);

        // Set icon for the taskbar (ICON_BIG)
        SendMessage(DW_HWND, WM_SETICON, ICON_BIG, (LPARAM)hIconBig);
    } else {
        MessageBox(DW_HWND, "DW : SetIcon : Failed to load icon!", "Error", MB_ICONERROR);
    }
    #endif
}

void DW_SetFullscreen(bool fullscreen) {
    #ifdef __APPLE__
        ToggleFullscreen(window);
    #endif
    #ifdef _WIN32
    if (!fullscreen) {
        // Restore windowed mode
        SetWindowLong(DW_HWND, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(DW_HWND, HWND_TOP, oldWindowRect.left, oldWindowRect.top,
                     oldWindowRect.right - oldWindowRect.left,
                     oldWindowRect.bottom - oldWindowRect.top,
                     SWP_SHOWWINDOW);

    } else if (fullscreen) {
        // Store current window position and size
        GetWindowRect(DW_HWND, &oldWindowRect);

        // Switch to fullscreen mode
        SetWindowLong(DW_HWND, GWL_STYLE, WS_POPUP);
        SetWindowPos(DW_HWND, HWND_TOP, 0, 0,
                     GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                     SWP_SHOWWINDOW);

    }
    #endif

}


void DW_SetWindowTitle(const char* title){
    #ifdef __APPLE__
    [window setTitle:[NSString stringWithUTF8String:title]];
    #endif
    #ifdef _WIN32
    SetWindowText(DW_HWND, title);
    #endif
}



bool DW_IsRunning() {
    #ifdef __APPLE__
    // Ensure OpenGL context is current
    [[view openGLContext] makeCurrentContext];


    // Render a frame (assumes view is an NSOpenGLView)
    CVTimeStamp outputTime;
    [view getFrameForTime:&outputTime];

    // Flush OpenGL buffer to display
    [[view openGLContext] flushBuffer];

    

    // Process events until none are left in the queue
    
    NSEvent *event;
    while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES])) {
        [NSApp sendEvent:event];
    }
        

    
    // Return running state
    return view->running;

    #endif

    #ifdef _WIN32
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        // Calculate delta time in seconds
        deltaTime = ((double)(currentTime.QuadPart - previousTime.QuadPart) / (double)frequency.QuadPart)*10;

        previousTime = currentTime;

        // Update FPS calculation
        elapsedTime += deltaTime;
        frameCount++;

        if (elapsedTime >= 1.0) {  // Update FPS every second
            FPS = (double)frameCount / elapsedTime;
            frameCount = 0;
            elapsedTime = 0.0;
        }

        // Handle Windows messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            SwapBuffers(hdc);
        }

        return running;
        
    #endif
}


void DW_CleanUp(){
    #ifdef __APPLE__
    [view->appLock lock];

    CVDisplayLinkStop(view->displayLink);
    CVDisplayLinkRelease(view->displayLink);

    [view->appLock unlock];
    [NSApp terminate:view];
    [NSApp run]; 
	[pool drain]; 
    #endif
}

int DW_GetWindowWidth(){
    #ifdef __APPLE__
    return windowSize.width;
    #endif

    #ifdef _WIN32
        return winWidth;
    #endif
}
int DW_GetWindowHeight(){
    #ifdef __APPLE__
    return windowSize.height;
    #endif

    #ifdef _WIN32
        return winHeight;
    #endif
}

int DW_GetMouseX(){
    #ifdef __APPLE__
    return mousePos.x;
    #endif

    #ifdef _WIN32
        return mousePos.x;
    #endif
}
int DW_GetMouseY(){
    #ifdef __APPLE__
    return (windowSize.height-mousePos.y);
    #endif

    #ifdef _WIN32
        return mousePos.y;
    #endif
}

void DW_ForceQuit(){
    #ifdef __APPLE__
        view->running = false;
    #endif

    #ifdef _WIN32
        running = false;
    #endif
}

void DW_CreateDirectory(const char *path) {
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

void DW_CreateSupportFolder(const char* companyName,const char* gameName){
    const char* pathToFolder;
    #ifdef __APPLE__        
        pathToFolder = stringf("%s/Library/Application Support",getenv("HOME"));
    #endif
    #ifdef _WIN32
        pathToFolder = stringf("%s",getenv("APPDATA"));
    #endif
    const char* companyFolder = stringf("%s/%s",pathToFolder,companyName);
    DW_CreateDirectory(companyFolder);

    DW_SUPPORT_FOLDER_PATH = stringf("%s/%s/%s",pathToFolder,companyName,gameName);
    DW_CreateDirectory(DW_SUPPORT_FOLDER_PATH);
    
}

const char* DW_GetSupportFolder(){
    return DW_SUPPORT_FOLDER_PATH;
}

void DW_SetResizable(bool resizable) {
    #ifdef __APPLE__
    // Toggle resizable state
    if (!resizable) {
        [window setStyleMask:window.styleMask & ~NSWindowStyleMaskResizable]; // Remove resizable flag
        NSButton *zoomButton = [window standardWindowButton:NSWindowZoomButton];
        [zoomButton setEnabled:NO];
    } else {
        [window setStyleMask:window.styleMask | NSWindowStyleMaskResizable]; // Add resizable flag
        NSButton *zoomButton = [window standardWindowButton:NSWindowZoomButton];
        [zoomButton setEnabled:YES];
    }
    #endif

    #ifdef _WIN32
        LONG style = GetWindowLong(DW_HWND, GWL_STYLE);
        if (resizable) {
            style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        } else {
            style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
        }
        SetWindowLong(DW_HWND, GWL_STYLE, style);
        SetWindowPos(DW_HWND, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    #endif
}

bool DW_IsMouseKeyPressed(MOUSE_KEY_CODE mouseKey){
    if (DW_MOUSE_KeyPressedState[mouseKey]) {
        DW_MOUSE_KeyPressedState[mouseKey] = false;
        return true;
    }
    return false;
}

bool DW_IsMouseKeyDown(MOUSE_KEY_CODE mouseKey){
    return DW_MOUSE_KeyDownState[mouseKey];
}

bool DW_IsKeyPressed(KEY_CODE keycode){
    if (DW_KEYBOARD_KeyPressedState[keycode]) {
        DW_KEYBOARD_KeyPressedState[keycode] = false;
        return true;
    }
    return false;
}

bool DW_IsKeyDown(KEY_CODE keycode){
    return DW_KEYBOARD_KeyDownState[keycode];
}


double DW_GetDeltaTime(){
    #ifndef DEBUG
        #ifdef __APPLE__
            return deltaTime/3;
        #endif
    #endif
    return deltaTime;
}

int DW_GetFPS(){
    return FPS;
}

#ifndef DARWIN_DONT_HIJACK_MAIN
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Hide the console window
    HWND hWndConsole = GetConsoleWindow();
    ShowWindow(hWndConsole, SW_HIDE);

    return CMAIN();
}
#else
// Standard entry point for other platforms
int main() {
    return CMAIN();
}

#endif


#endif
#endif


#ifdef __cplusplus
}
#endif