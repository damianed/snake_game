#if X11
#include <X11/Xlib.h>
#endif

typedef struct {
    char *memory;
    int width;
    int height;
    int sizeChanged;
    int bytesPerPixel;
#if X11
    XImage *xImage;
#endif
} windowBuffer;

void gameUpdateAndRender(windowBuffer *windowBuffer);
