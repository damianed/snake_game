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

void updatePlayerPosition(GameState *gameState, int width, int height, float deltaTimeMs) {
    float newX = gameState->player.position.x;
    float newY = gameState->player.position.y;

    switch (gameState->player.direction) {
        case UP: {
            newY -= (SPEED * deltaTimeMs);
        } break;
        case RIGHT: {
            newX += (SPEED * deltaTimeMs);
        } break;
        case DOWN: {
            newY += (SPEED * deltaTimeMs);
        } break;
        case LEFT: {
            newX -= (SPEED * deltaTimeMs);
        } break;
    }

    //TODO: allow travel from end/start of screen to start/end of screen
    if (newX < 0) {
        newX = 0;
    } else if (newX + SQUARE_SIZE > width) {
        newX = width - SQUARE_SIZE;
    }

    if (newY < 0) {
        newY = 0;
    } else if (newY + SQUARE_SIZE > height) {
        newY = height - SQUARE_SIZE;
    }

    gameState->player.position.x = newX;
    gameState->player.position.y = newY;

    // TODO: this is a very bad way to do this because it only changes the direction if it happends to land right at the boundary
    // of a square, which is not enforced in any way and makes it feel like input lag, I think a better way to do do this is to check if
    // it crossed a square boundary with the last position update
    //if (((int)gameState->player.position.x) % SQUARE_SIZE == 0 && ((int)gameState->player.position.y) % SQUARE_SIZE == 0) {
        gameState->player.direction = gameState->inputDirection;
    //}
}

void gameUpdateAndRender(WindowBuffer *windowBuffer, GameState *gameState, float deltaTimeMs) {
    drawGrid(windowBuffer);

    updatePlayerPosition(gameState, windowBuffer->width, windowBuffer->height, deltaTimeMs);

    int playerX = gameState->player.position.x;
    int playerY = gameState->player.position.y;

    int pitch = windowBuffer->width * windowBuffer->bytesPerPixel;
    for (int y = playerY; y < playerY + SQUARE_SIZE; ++y) {
        char *row = windowBuffer->memory + (y * pitch);
        for (int x = playerX; x < playerX + SQUARE_SIZE; ++x) {
            uint32_t *pixel = (uint32_t *)(row + (x * windowBuffer->bytesPerPixel));
            *pixel = PLAYER_COLOR;
        }
    }
}
