/* Includes */
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Globals */
#define WINDOW_TITLE "My Window"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

/* Prototypes */
bool sdl__initialize(struct Game *game);
void game_cleanup(struct Game *game, int exit_status);

/* Main Logic */
int main(void) {
  struct Game game = {
      .window = NULL,
      .renderer = NULL,
  };

  if (sdl__initialize(&game)) {
    game_cleanup(&game, EXIT_FAILURE);
    exit(1);
  }
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        game_cleanup(&game, EXIT_SUCCESS);
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          game_cleanup(&game, EXIT_SUCCESS);
          break;
        default:
          break;
        }
      default:
        break;
      }
    }
    SDL_RenderClear(game.renderer);
    SDL_RenderPresent(game.renderer);
    SDL_Delay(16);
  }

  game_cleanup(&game, EXIT_SUCCESS);

  return EXIT_SUCCESS;
}

/* Functions */
void game_cleanup(struct Game *game, int exit_status) {
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
  exit(exit_status);
}

bool sdl__initialize(struct Game *game) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "Error Initializing SDL: %s\n", SDL_GetError());
    return true;
  }
  game->window =
      SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!game->window) {
    fprintf(stderr, "Error Creating Window: %s\n", SDL_GetError());
    return true;
  }
  game->renderer = SDL_CreateRenderer(game->window, -1, 0);
  if (!game->renderer) {
    fprintf(stderr, "Error Creating Renderer: %s\n", SDL_GetError());
    return true;
  }
  return false;
}
