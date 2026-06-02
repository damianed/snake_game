#include <stdio.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/mman.h>

int main() {
    printf("hello world\n");
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        printf("Failed to initilize display\n");
        return 1;
    }

    Window root = DefaultRootWindow(display);
    if (!root) {
        printf("No root window\n");
        XCloseDisplay(display);
        return 1;
    }

    int defaultScreen = DefaultScreen(display);
    int screenBitDepth = 24;
    XVisualInfo visualInfo = {0};
    if(!XMatchVisualInfo(display, defaultScreen, screenBitDepth, TrueColor, &visualInfo))
    {
      printf("No matching visual info\n");
      return 1;
    }

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
    XSetWindowAttributes windowAttributes;
    windowAttributes.background_pixel = 0;
    windowAttributes.colormap = XCreateColormap(display, root, visualInfo.visual, AllocNone);

    Window window = XCreateWindow(display, root, 0, 0, WINDOW_HEIGHT, WINDOW_WIDTH,
                                  0, visualInfo.depth, InputOutput,
                                  visualInfo.visual, CWBackPixel|CWColormap, &windowAttributes);

    if (!window) {
        printf("Failed to create window\n");
        XCloseDisplay(display);
        return 1;
    }

    XStoreName(display, window, "Snake - The Game (Called the most innovative game of 2026 by its creator)");
    XMapWindow(display, window);

    int pixelBits = 32;
    int bytesPerPixel = pixelBits / 8;
    size_t windowBufferSize = WINDOW_WIDTH * WINDOW_HEIGHT * bytesPerPixel;
    char *memory = (char *) mmap(0, windowBufferSize, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    XImage *xWindowBuffer = XCreateImage(display, visualInfo.visual, visualInfo.depth,
                                          ZPixmap, 0, memory, WINDOW_WIDTH, WINDOW_HEIGHT,
                                          pixelBits, 0);
    GC graphicsContext = DefaultGC(display, defaultScreen);

    XFlush(display);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XEvent event;
    int running = 1;
    while (running) {
        while (XPending(display) > 0) {
            XNextEvent(display, &event);
            switch (event.type) {
                case ClientMessage: {
                    if ((Atom) event.xclient.data.l[0] == wm_delete_window) {
                        XDestroyWindow(event.xclient.display, event.xclient.window);
                        running = 0;
                    } break;
                }
            }
        }

        int pitch = WINDOW_WIDTH * bytesPerPixel;
        for (int y = 0; y < WINDOW_HEIGHT; ++y) {
            char *row =  memory + (y * pitch);
            for (int x = 0; x < WINDOW_WIDTH; ++x) {
                uint32_t *p = (uint32_t *) (row + (x * bytesPerPixel));
                if (x % 16 && y % 16) {
                    *p = 0xffffffff;
                } else {
                    *p = 0;
                }
              }
        }

        XPutImage(display, window, graphicsContext, xWindowBuffer, 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }


    return 0;
}
