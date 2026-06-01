#include <stdio.h>
#include <stdint.h>
#include <X11/Xlib.h>

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        printf("Failed to initilize display");
        return 1;
    }

    Window root = DefaultRootWindow(display);
    if (root == None) {
        printf("No root window");
        XCloseDisplay(display);
        return 1;
    }
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

    Window window = XCreateSimpleWindow(display, root, 0, 0, WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0, 0xffffff);
    if (window == None) {
        printf("Failed to create window");
        XCloseDisplay(display);
        return 1;
    }

    //int deafultWindow = DefaultScreen(display);
    XMapWindow(display, window);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, & wm_delete_window, 1);

    XEvent event;

    int running = 1;
    while (running) {
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


    return 0;
}
