#include "game.h"
#include <stdint.h>
#include <stdio.h>

#define PLAYER_SQUARE_SIZE 16
#define PLAYER_COLOR 0x000000

global_variable float SPEED = 0.3;

void drawGrid(WindowBuffer *windowBuffer) {
    int pitch = windowBuffer->width * windowBuffer->bytesPerPixel;
    for (int y = 0; y < windowBuffer->height; ++y) {
        char *row = windowBuffer->memory + (y * pitch);
        for (int x = 0; x < windowBuffer->width; ++x) {
            uint32_t *p = (uint32_t *)(row + (x * windowBuffer->bytesPerPixel));
            if (x % PLAYER_SQUARE_SIZE && y % PLAYER_SQUARE_SIZE) {
                *p = 0xffffffff;
            } else {
                *p = 0;
            }
        }
    }
}

void updatePlayerPosition(GameState *gameState, double deltaTime) {
    printf("deltaTime ms: %f\n", deltaTime);
    switch (gameState->inputDirection) {
        case UP: {
            gameState->playerPos.y -= (SPEED * deltaTime);
        } break;
        case RIGHT: {
            gameState->playerPos.x += (SPEED * deltaTime);
        } break;
        case DOWN: {
            gameState->playerPos.y += (SPEED * deltaTime);
        } break;
        case LEFT: {
            gameState->playerPos.x -= (SPEED * deltaTime);
        } break;
    }
}

//TODO: GET THE DELTA TIME IN MS INSTEAD OF SECONDS
void gameUpdateAndRender(WindowBuffer *windowBuffer, GameState *gameState, double deltaTime) {
    drawGrid(windowBuffer);

    printf("deltaTime seconds: %f\n", deltaTime);
    updatePlayerPosition(gameState, deltaTime * 1000);

    //TODO: this looks like blinking because it jumps from one square to
    //another instead of going progressivelly to the next square, find a way
    //to fix it
    int playerX = ((int) gameState->playerPos.x/16) * 16;
    int playerY = ((int) gameState->playerPos.y/16) * 16;
    //TODO: draw player
    int pitch = windowBuffer->width * windowBuffer->bytesPerPixel;
    for (int y = playerY; y < playerY + PLAYER_SQUARE_SIZE; ++y) {
        char *row = windowBuffer->memory + (y * pitch);
        for (int x = playerX; x < playerX + PLAYER_SQUARE_SIZE; ++x) {
            uint32_t *pixel = (uint32_t *)(row + (x * windowBuffer->bytesPerPixel));
            *pixel = PLAYER_COLOR;
        }
    }
}
