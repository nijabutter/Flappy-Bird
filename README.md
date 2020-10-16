# Flappy Bird

Made using ![SDL2](https://www.libsdl.org/).
Click to flap your wings and don't hit the ground or pipes.

Dependencies
----------
- ![SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- ![SDL_image](https://www.libsdl.org/projects/SDL_image/)

Compiling
--------
`g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I PATH_TO_SDL2/include && g++ *.o -o bin/release/main -s -L PATH_TO_SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows`
