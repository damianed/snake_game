#!/bin/bash

#mkdir -p ../build
#pushd ../build
gcc -std=c99 -pedantic -DSLOW -DX11 -g -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable ./code/x11_platform_layer.c -o snake_game ./code/game.c ./code/game.h -lX11
#popd
