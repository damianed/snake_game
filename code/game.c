#include "game.h"
#include <stdint.h>

void gameUpdateAndRender(windowBuffer *windowBuffer) {
    int pitch = windowBuffer->width * windowBuffer->bytesPerPixel;
    for (int y = 0; y < windowBuffer->height; ++y) {
        char *row = windowBuffer->memory + (y * pitch);
        for (int x = 0; x < windowBuffer->width; ++x) {
            uint32_t *p = (uint32_t *)(row + (x * windowBuffer->bytesPerPixel));
            if (x % 16 && y % 16) {
                *p = 0xffffffff;
            } else {
                *p = 0;
            }
        }
    }
}
