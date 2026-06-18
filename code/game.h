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

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

typedef struct {
    float x;
    float y;
} Position;

typedef struct {
    Position position;
    Direction direction;
} Player;

typedef struct {
    Player player;
    Direction inputDirection;
} GameState;

#define STARTING_GAME_STATE {.inputDirection = RIGHT, .player = {{0, 16}, RIGHT}}

#define GAME_UPDATE_AND_RENDER(name) void name(WindowBuffer *windowBuffer, GameState *gameState, float deltaTime)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);
