#include <SDL_render.h>
#include <application.h>
#include <iostream>
#include "resload.h"
#include <SDL.h>
#include <base.h>
#include <renderer_temp.h>
#include <config.h>
#include <humanoid.h>
#include <assetbundleloader.h>

void Application::Dialouge() {
  int currentDialougeIndex = 0;
  TTF_Font* font = ResLoad::LoadFont(current_asset_bundle.FONT_GAME_DIALOUGE_PATH.c_str(), 24);
  TTF_Font* topFont = ResLoad::LoadFont(current_asset_bundle.FONT_GAME_DIALOUGE_NAME_PATH.c_str(), 18);
  auto humanoid_texture_data = ResLoad::LoadImage(current_humanoid->texture_path);
  current_humanoid->texture = humanoid_texture_data.texture;

  SDL_Rect humanoid_rect = { 100, 50, 600, 450 };
  SDL_Rect dialouge_rect = { 150, 450, 500, 100 };
  SDL_Rect dialouge_bg_rect = { 
    dialouge_rect.x - current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS, 
    dialouge_rect.y - current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS, 
    dialouge_rect.w + current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS * 2, 
    dialouge_rect.h + current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS * 2
  };
  
  auto character_name_data = ResLoad::RenderText(topFont, current_humanoid->name);
  SDL_Texture* character_name_texture = character_name_data.texture;
  SDL_Rect character_name_rect = {0, 0, 0, 0}, character_name_bg_rect = {0, 0, 0, 0};
  SDL_Texture* dialouge_texture = nullptr;
  SDL_Rect dialouge_text_rect = {0, 0, 0, 0};
  while (state == APP_STATE_DIALOUGE) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        state = APP_STATE_QUIT;
      }
      if (event.type == SDL_KEYDOWN) {    
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          state = APP_STATE_MAIN_MENU;
        }
        currentDialougeIndex++; 
      }
    }
    if (currentDialougeIndex == current_humanoid->messages.size()) {
      std::cout << "Reached end of dialouge queue\n";
      break;
    }
    character_name_rect.x = dialouge_rect.x + 5;
    character_name_rect.y = dialouge_rect.y - (character_name_rect.h * 0.5);
    character_name_bg_rect = character_name_rect;
    character_name_bg_rect.x -= current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS;
    character_name_bg_rect.y -= current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS;
    character_name_bg_rect.w += current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS * 2;
    character_name_bg_rect.h += current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS * 2;

    // Render the dialogue text
    if (currentDialougeIndex < current_humanoid->messages.size()) {
      auto dialouge_data = ResLoad::RenderText(font, current_humanoid->messages[currentDialougeIndex], {255, 255, 255}, 500);
      dialouge_texture = dialouge_data.texture;
      dialouge_text_rect.x = dialouge_data.w + 10;
      dialouge_text_rect.y = dialouge_data.h + 10;
    }
    if (current_humanoid->texture == nullptr) {
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, current_humanoid->texture, NULL, &humanoid_rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderFillRect(renderer, &dialouge_bg_rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &dialouge_rect);
    if (character_name_texture != NULL) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderFillRect(renderer, &character_name_bg_rect);
      SDL_RenderCopy(renderer, character_name_texture, NULL, &character_name_rect);
      SDL_DestroyTexture(character_name_texture);
    }
    if (dialouge_texture != NULL) {
      SDL_RenderCopy(renderer, dialouge_texture, NULL, &dialouge_text_rect);
      SDL_DestroyTexture(dialouge_texture);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60);
  }
  
  std::cout << "Unloading assets\n";
  TTF_CloseFont(font);
  state = APP_STATE_GAME;
}
