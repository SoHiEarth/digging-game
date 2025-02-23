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
#include <algorithm>
TTF_Font* font, *topFont;
SDL_Texture* dialouge_texture, *character_name_texture, *control_texture;
SDL_Rect humanoid_rect = { 100, 50, 600, 450 },
  dialouge_bg_rect = { 
    150 - current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS, 
    450 - current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS, 
    500 + current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS * 2, 
    100 + current_asset_bundle.DIALOUGE_BG_BORDER_THICKNESS * 2 },
  dialouge_rect = { 150 + 20, 450 + 20, 0, 0 },
  name_rect = { 140, 440, 0, 0 },
  name_bg_rect = { 0, 0, 0, 0 };

void Application::Dialouge() {
  int brightness = 255;
  std::string current_dialouge_text = "";
  if (current_humanoid == nullptr) {
    state = APP_STATE_GAME;
  }
  int currentDialougeIndex = 0;
  font = ResLoad::LoadFont(current_asset_bundle.FONT_GAME_DIALOUGE_PATH, 24);
  topFont = ResLoad::LoadFont(current_asset_bundle.FONT_GAME_DIALOUGE_NAME_PATH, 20);
  
  if (current_humanoid->name == "") {
    current_humanoid->name = "???";
  }
  character_name_texture = ResLoad::RenderText(topFont, current_humanoid->name, {255, 255, 255}, 500);
  SDL_QueryTexture(character_name_texture, NULL, NULL, &name_rect.w, &name_rect.h);
  name_bg_rect = {
    name_rect.x - 5,
    name_rect.y - 5,
    name_rect.w + 10,
    name_rect.h + 10
  };

  control_texture = ResLoad::RenderText(topFont, "Press [SPACE] to continue", {255, 255, 255}, 500);
  int control_w, control_h;
  SDL_QueryTexture(control_texture, NULL, NULL, &control_w, &control_h);
  SDL_Rect control_rect = { 800 - control_w - 10, 600 - control_h - 10, control_w, control_h };

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
        current_dialouge_text = "";
      }
    }
    if (currentDialougeIndex == current_humanoid->messages.size()) {
      std::cout << "Reached end of dialouge queue\n";
      state = APP_STATE_GAME;
    }
    // Render the dialogue text
    if (currentDialougeIndex < current_humanoid->messages.size() || current_dialouge_text.size() != current_humanoid->messages[currentDialougeIndex].size()) {
      SDL_DestroyTexture(dialouge_texture);
      if (current_dialouge_text.size() != current_humanoid->messages[currentDialougeIndex].size()) {
        current_dialouge_text += current_humanoid->messages[currentDialougeIndex][current_dialouge_text.size()];
      }
      dialouge_texture = ResLoad::RenderText(font, current_dialouge_text, {255, 255, 255}, 500);
      SDL_QueryTexture(dialouge_texture, NULL, NULL, &dialouge_rect.w, &dialouge_rect.h);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, current_humanoid->texture, NULL, &humanoid_rect);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &dialouge_bg_rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &dialouge_bg_rect);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &name_bg_rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawRect(renderer, &name_bg_rect);
    SDL_RenderCopy(renderer, character_name_texture, NULL, &name_rect);
    SDL_RenderCopy(renderer, dialouge_texture, NULL, &dialouge_rect);
    SDL_RenderCopy(renderer, control_texture, NULL, &control_rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, std::clamp(brightness, 0, 255));
    SDL_RenderFillRect(renderer, NULL);
    brightness -= 50;
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
  }
  
  std::cout << "Unloading assets\n";
  SDL_DestroyTexture(dialouge_texture);
  SDL_DestroyTexture(character_name_texture);
  ResLoad::FreeFont(topFont);
  ResLoad::FreeFont(font);
  state = APP_STATE_GAME;
}
