#if X11
#include <X11/Xlib.h>
#endif

#define internal static
#define local_persist static
#define global_variable static

typedef struct {
    char *memory;
    int width;
    int height;
    int sizeChanged;
    int bytesPerPixel;
#if X11
    XImage *xImage;
#endif
} WindowBuffer;

typedef enum {UP, DOWN, LEFT, RIGHT} InputDirection;

typedef struct {
    float x;
    float y;
} Position;

typedef struct {
    InputDirection inputDirection;
    Position playerPos;
} GameState;

#define STARTING_GAME_STATE {.inputDirection = RIGHT, .playerPos = {0, 16}}

void gameUpdateAndRender(WindowBuffer *windowBuffer, GameState *input, double deltaTime);
