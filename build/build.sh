#!/bin/bash
commonCompilerFlags="-std=gnu99 -pedantic -DSLOW -DX11 -g -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable"
#mkdir -p ../build
#pushd ../build
gcc $commonCompilerFlags ./code/x11_platform_layer.c -o snake_game -lX11
gcc -shared $commonCompilerFlags ./code/game.c -o game.so
#popd
