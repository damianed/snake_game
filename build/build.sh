#!/bin/bash

#mkdir -p ../build
#pushd ../build
gcc -std=c99 -pedantic  -DSLOW -g -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable ../snake_game/code/x11_platform_layer.c -o snake_game -lX11
#popd
