/* Includes */
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Globals */
#define WINDOW_TITLE "My Window"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define IMAGE_FLAGS IMG_INIT_PNG

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *background;
};

/* Prototypes */
bool sdl__initialize(struct Game *game);
void game_cleanup(struct Game *game, int exit_status);
bool load_media(struct Game *game);

/* Main Logic */
int main(void) {
  struct Game game = {
      .window = NULL,
      .renderer = NULL,
      .background = NULL,
  };

  if (sdl__initialize(&game)) {
    game_cleanup(&game, EXIT_FAILURE);
  }
  if(load_media(&game)){
    game_cleanup(&game, EXIT_FAILURE);
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
        case SDL_SCANCODE_SPACE:
          SDL_SetRenderDrawColor(game.renderer, rand() % 256, rand() % 256, rand() % 256, 255);
          break;
        default:
          break;
        }
      default:
        break;
      }
    }
    SDL_RenderClear(game.renderer);
    SDL_RenderCopy(game.renderer, game.background, NULL, NULL);
    SDL_RenderPresent(game.renderer);
    SDL_Delay(16);
  }

  game_cleanup(&game, EXIT_SUCCESS);

  return EXIT_SUCCESS;
}

/* Functions */
void game_cleanup(struct Game *game, int exit_status) {
  SDL_DestroyTexture(game->background);
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  IMG_Quit();
  SDL_Quit();
  exit(exit_status);
}

bool sdl__initialize(struct Game *game) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "Error Initializing SDL: %s\n", SDL_GetError());
    return true;
  }
  int img_init = IMG_Init(IMAGE_FLAGS);
  if((img_init & IMAGE_FLAGS) != IMAGE_FLAGS){
    fprintf(stderr, "Error Initializing Image: %s\n", IMG_GetError());
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
  srand((unsigned)time(NULL));
  return false;
}

bool load_media(struct Game *game){
  game->background = IMG_LoadTexture(game->renderer, "images/background.png");
  if(!game->background){
    fprintf(stderr, "Error Initializing Texture: %s\n", IMG_GetError());
    return true;
  }
  return false;
}
