#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>

#include "chip8.h"

const int MOD = 15;

SDL_Window *window;
SDL_Renderer *renderer; 

int initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  } 

  window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH*MOD, HEIGHT*MOD, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("No window created");
    SDL_Quit();
    return -1;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    printf("No renderer created");
    SDL_Quit();
    return -1;
  }

  return 0;
}

int render(chip8 *c) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      if (c->display[j][i] != 0) {
        SDL_Rect rect = {i*MOD, j*MOD, MOD, MOD,};
        SDL_RenderFillRect(renderer, &rect);
        printf("debug checkpoint: rect drawn");
      }
    }
  }

  SDL_RenderPresent(renderer);
  return 0;
}

int endSDL() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  printf("debug checkpoint: quitted sdl");
  return 0;
}

