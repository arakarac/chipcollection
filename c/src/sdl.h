#ifndef SDL 
#define SDL 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

#include "chip8.h"

int initSDL();
int render(chip8 *c);
int endSDL();
int getInput(chip8 *c);

#endif 
