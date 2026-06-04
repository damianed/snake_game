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

    int windowWidth = 1920;
    int windowHeight = 1080;

    XSetWindowAttributes windowAttributes;
    windowAttributes.bit_gravity = StaticGravity;
    windowAttributes.background_pixel = 0;
    windowAttributes.colormap = XCreateColormap(display, root, visualInfo.visual, AllocNone);
    windowAttributes.event_mask = StructureNotifyMask|KeyPressMask|KeyReleaseMask;

    unsigned long attributesMask = CWBitGravity|CWBackPixel|CWColormap|CWEventMask;
    Window window = XCreateWindow(display, root, 0, 0, windowHeight, windowWidth,
                                  0, visualInfo.depth, InputOutput,
                                  visualInfo.visual, attributesMask, &windowAttributes);

    if (!window) {
        printf("Failed to create window\n");
        XCloseDisplay(display);
        return 1;
    }

    XStoreName(display, window, "Snake - The Game (Called the most innovative game of 2026 by its creator)");
    XMapWindow(display, window);

    int pixelBits = 32;
    int bytesPerPixel = pixelBits / 8;
    size_t windowBufferSize = windowWidth * windowHeight * bytesPerPixel;
    char *memory = mmap(0, windowBufferSize, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    XImage *xWindowBuffer = XCreateImage(display, visualInfo.visual, visualInfo.depth,
                                          ZPixmap, 0, memory, windowWidth, windowHeight,
                                          pixelBits, 0);
    GC graphicsContext = DefaultGC(display, defaultScreen);

    XFlush(display);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XEvent event;
    int windowSizeChanged = 0;
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
                case ConfigureNotify: {
                    printf("configureNotify called\n");
                    XConfigureEvent *e = (XConfigureEvent *) &event;
                    if (windowHeight == e->height && windowWidth == e->width) {
                        break;
                    }
                    windowWidth = e->width;
                    windowHeight = e->height;
                    windowSizeChanged = 1;
                } break;
                case KeyPress: {
                    XKeyPressedEvent *e = (XKeyPressedEvent *) &event;
                    if (e->keycode == XKeysymToKeycode(display, XK_w)) printf("W\n");
                    if (e->keycode == XKeysymToKeycode(display, XK_l)) printf("L\n");
                } break;
                case KeyRelease: {
                    XKeyReleasedEvent *e = (XKeyReleasedEvent *) &event;
                    if (e->keycode == XKeysymToKeycode(display, XK_w)) printf("not W\n");
                    if (e->keycode == XKeysymToKeycode(display, XK_l)) printf("not L\n");
                } break;
            }
        }

        if (windowSizeChanged) {
            printf("window size changed\n");
            windowSizeChanged = 0;
            munmap(memory, windowBufferSize);
            windowBufferSize = windowWidth * windowHeight * bytesPerPixel;
            memory = mmap(0, windowBufferSize, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
            xWindowBuffer = XCreateImage(display, visualInfo.visual, visualInfo.depth,
                                          ZPixmap, 0, memory, windowWidth, windowHeight,
                                          pixelBits, 0);
        }

        int pitch = windowWidth * bytesPerPixel;
        for (int y = 0; y < windowHeight; ++y) {
            char *row = memory + (y * pitch);
            for (int x = 0; x < windowWidth; ++x) {
                uint32_t *p = (uint32_t *) (row + (x * bytesPerPixel));
                if (x % 16 && y % 16) {
                    *p = 0xffffffff;
                } else {
                    *p = 0;
                }
              }
        }

        XPutImage(display, window, graphicsContext, xWindowBuffer, 0, 0, 0, 0, windowWidth, windowHeight);
    }


    return 0;
}
