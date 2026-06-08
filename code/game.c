#include "game.h"
#include <stdint.h>

#define SQUARE_SIZE 16
#define PLAYER_COLOR 0x000000

global_variable float SPEED = 0.2;

void drawGrid(WindowBuffer *windowBuffer) {
    int pitch = windowBuffer->width * windowBuffer->bytesPerPixel;
    for (int y = 0; y < windowBuffer->height; ++y) {
        char *row = windowBuffer->memory + (y * pitch);
        for (int x = 0; x < windowBuffer->width; ++x) {
            uint32_t *p = (uint32_t *)(row + (x * windowBuffer->bytesPerPixel));
            if (x % SQUARE_SIZE && y % SQUARE_SIZE) {
                *p = 0xffffffff;
            } else {
                *p = 0;
            }
        }
    }
}

void updatePlayerPosition(GameState *gameState, float deltaTimeMs) {
    switch (gameState->player.direction) {
        case UP: {
            gameState->player.position.y -= (SPEED * deltaTimeMs);
        } break;
        case RIGHT: {
            gameState->player.position.x += (SPEED * deltaTimeMs);
        } break;
        case DOWN: {
            gameState->player.position.y += (SPEED * deltaTimeMs);
        } break;
        case LEFT: {
            gameState->player.position.x -= (SPEED * deltaTimeMs);
        } break;
    }

    //TODO: make a queue of inputs so we don't lose input directions if they press before a SQUARE_SIZE boundary
    // and maybe think or another way to do this so it doesn't feel liek there is some input lag
    if (((int)gameState->player.position.x) % SQUARE_SIZE == 0 && ((int)gameState->player.position.y) % SQUARE_SIZE == 0) {
        gameState->player.direction = gameState->inputDirection;
    }
}

void gameUpdateAndRender(WindowBuffer *windowBuffer, GameState *gameState, float deltaTimeMs) {
    drawGrid(windowBuffer);

    updatePlayerPosition(gameState, deltaTimeMs);

    int playerX = gameState->player.position.x;
    int playerY = gameState->player.position.y;

    //TODO: check window boundaries do this doesn't cause segfault at the end of window
    int pitch = windowBuffer->width * windowBuffer->bytesPerPixel;
    for (int y = playerY; y < playerY + SQUARE_SIZE; ++y) {
        char *row = windowBuffer->memory + (y * pitch);
        for (int x = playerX; x < playerX + SQUARE_SIZE; ++x) {
            uint32_t *pixel = (uint32_t *)(row + (x * windowBuffer->bytesPerPixel));
            *pixel = PLAYER_COLOR;
        }
    }
}
