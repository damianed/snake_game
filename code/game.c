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
    Position oldPosition = gameState->player.position;
    Position newPosition = gameState->player.position;
    int prevXSquare = ((int)oldPosition.x) / SQUARE_SIZE;
    int prevYSquare = ((int)oldPosition.y) / SQUARE_SIZE;

    switch (gameState->player.direction) {
        case UP: {
            newPosition.y -= (SPEED * deltaTimeMs);
        } break;
        case RIGHT: {
            newPosition.x += (SPEED * deltaTimeMs);
        } break;
        case DOWN: {
            newPosition.y += (SPEED * deltaTimeMs);
        } break;
        case LEFT: {
            newPosition.x -= (SPEED * deltaTimeMs);
        } break;
    }

    //TODO: allow travel from end/start of screen to start/end of screen
    if (newPosition.x < 0) {
        newPosition.x = 0;
    } else if (newPosition.x + SQUARE_SIZE > width) {
        newPosition.x = width - SQUARE_SIZE;
    }

    if (newPosition.y < 0) {
        newPosition.y = 0;
    } else if (newPosition.y + SQUARE_SIZE > height) {
        newPosition.y = height - SQUARE_SIZE;
    }

    int changedDirection = gameState->player.direction != gameState->inputDirection;
    //  Notes:
    //  I think the best approach is to set a threshold and if is within that threshold of closeness to a square border then update
    //  the direction and set the position to be exactly on the square so it douesn't go out of grid
    //  we will need to handle each verticall and horizontal separatly, also I think if you move in the same axis the updates should be imediate
    //  without checking
    Direction playerDirection = gameState->player.direction;
    Direction inputDirection = gameState->inputDirection;
    // if same axis update without checking bounds
    if ((playerDirection == UP && inputDirection == DOWN) || (playerDirection == DOWN && inputDirection == UP) ||
       (playerDirection == LEFT && inputDirection == RIGHT) || (playerDirection == RIGHT && inputDirection == LEFT)) {
        gameState->player.direction = gameState->inputDirection;
    } else {
        //TODO: think about a good threshold, I don't think multipling by dt is a
        //good approach but I will leave it do remind me that the responsivness is
        //affected by refreshrate, need to find a better way to handle this
        const float threshold = 0.2 * deltaTimeMs;
        int update = 0;
        //float nextBoundary = -1;

        int currentXSquare = ((int)newPosition.x) / SQUARE_SIZE;
        int currentYSquare = ((int)newPosition.y) / SQUARE_SIZE;
        switch (playerDirection) {
            case RIGHT: {
                float nextBoundary = currentXSquare * SQUARE_SIZE;
                if (newPosition.x - nextBoundary <= threshold) {
                    update = 1;
                }
            } break;
            case LEFT: {
                float nextBoundary = (currentXSquare + 1) * SQUARE_SIZE;
                if (newPosition.x - nextBoundary <= threshold) {
                    update = 1;
                }
            } break;
            case UP: {
                float nextBoundary = (currentYSquare + 1) * SQUARE_SIZE;
                if (newPosition.y - nextBoundary <= threshold) {
                    update = 1;
                }
            } break;
            case DOWN: {
                float nextBoundary = (currentYSquare) * SQUARE_SIZE;
                if (newPosition.y - nextBoundary <= threshold) {
                    update = 1;
                }
            } break;
        }
        if (update) {
            // TODO: update location to be exactly inside of the square so we don't
            // go out of the grid
            gameState->player.direction = gameState->inputDirection;
        }
    }

    gameState->player.position = newPosition;
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
