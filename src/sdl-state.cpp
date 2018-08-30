#include "sdl-state.h"


namespace SDL_State
{

#include <SDL2/SDL.h>


bool
init(SDL_State& sdl_state, const char *title, u32 initial_width, u32 initial_height)
{
  bool success = true;
  sdl_state.sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initial_width, initial_height, SDL_WINDOW_RESIZABLE);

  if (sdl_state.sdl_window == NULL)
  {
    printf("Failed to create SDL_Window: %s\n", SDL_GetError());
    success &= false;
  }
  else
  {
    sdl_state.sdl_renderer = SDL_CreateRenderer(sdl_state.sdl_window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (sdl_state.sdl_renderer == NULL)
    {
      printf("Failed to create SDL_Renderer: %s\n", SDL_GetError());
      success &= false;
    }
  }

  success &= SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

  return success;
}


bool
set_render_texture(SDL_State& sdl_state, Texture::Texture& texture)
{
  bool success = true;

  if (0 != SDL_RenderSetLogicalSize(sdl_state.sdl_renderer, texture.width, texture.height))
  {
    printf("Failed to set renderer logical size: %s\n", SDL_GetError());
    success &= false;
  }

  if (sdl_state.sdl_texture != NULL)
  {
    SDL_DestroyTexture(sdl_state.sdl_texture);
  }

  sdl_state.sdl_texture = SDL_CreateTexture(sdl_state.sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texture.width, texture.height);
  if (sdl_state.sdl_texture == NULL)
  {
    printf("Failed to create SDL_Texture: %s\n", SDL_GetError());
    success &= false;
  }

  return success;
}


bool
render(SDL_State& sdl_state, Texture::Texture& texture)
{
  bool success = true;

  success &= 0 == SDL_UpdateTexture(sdl_state.sdl_texture, NULL, texture.pixels, texture.width * sizeof(u32));
  success &= 0 == SDL_SetRenderDrawColor(sdl_state.sdl_renderer, 0, 0, 0, 0xff);
  success &= 0 == SDL_RenderClear(sdl_state.sdl_renderer);
  success &= 0 == SDL_RenderCopy(sdl_state.sdl_renderer, sdl_state.sdl_texture, NULL, NULL);
  if (success)
  {
    SDL_RenderPresent(sdl_state.sdl_renderer);
  }

  return success;
}


bool
set_fullscreen(SDL_State& sdl_state, bool on)
{
  bool success = true;
  success &= 0 == SDL_SetWindowFullscreen(sdl_state.sdl_window, on ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
  return success;
}


bool
is_fullscreen(SDL_State& sdl_state)
{
  bool result;
  result = SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(sdl_state.sdl_window);
  return result;
}


void
destroy(SDL_State& sdl_state)
{
  SDL_DestroyWindow(sdl_state.sdl_window);
  SDL_DestroyTexture(sdl_state.sdl_texture);
  SDL_DestroyRenderer(sdl_state.sdl_renderer);
  SDL_Quit();
}

} // namespace SDL_State