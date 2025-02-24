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

SDL_Rect GetBackground(SDL_Rect rect) {
  return { rect.x - 10, rect.y - 10, rect.w + 20, rect.h + 20 };
}
Humanoid humanoid;

void Application::Dialouge() {
  if (current_humanoid == nullptr)
      state = APP_STATE_GAME;
  else
    humanoid = *current_humanoid;
  TTF_Font* font, *topFont;
  int brightness = 255;
  SDL_Texture* dialouge_texture = nullptr, *character_name_texture = nullptr, *control_texture = nullptr;
  SDL_GetWindowSize(window, &window_width, &window_height);
  SDL_Rect humanoid_rect = { 100, 50, window_width - 200, window_height - 100 - 50 },
    dialouge_rect = { 150, window_height - 150, 0, 0 },
    dialouge_bg_rect = { GetBackground(dialouge_rect).x - 5, GetBackground(dialouge_rect).y - 5, 500, 120 },
    name_rect = { dialouge_rect.x - 30, dialouge_rect.y - 30, 0, 0 };
  std::string current_dialouge_text = "";
  
  int currentDialougeIndex = 0;
  font = ResLoad::LoadFont(current_asset_bundle.FONT_GAME_DIALOUGE_PATH, 24);
  topFont = ResLoad::LoadFont(current_asset_bundle.FONT_GAME_DIALOUGE_NAME_PATH, 20);
  
  if (humanoid.name.empty())
    humanoid.name = "???";
  character_name_texture = ResLoad::RenderText(topFont, humanoid.name, {255, 255, 255}, 500);
  SDL_QueryTexture(character_name_texture, NULL, NULL, &name_rect.w, &name_rect.h);
  SDL_Rect name_bg_rect = GetBackground(name_rect);

  control_texture = ResLoad::RenderText(topFont, "Press [SPACE] to continue", {255, 255, 255}, 500);
  int control_w, control_h;
  SDL_QueryTexture(control_texture, NULL, NULL, &control_w, &control_h);
  SDL_Rect control_rect = { window_width - control_w - 10, window_height - control_h - 10, control_w, control_h };

  humanoid_rect = { 100, 50, window_width - 200, window_height - 100 };
  dialouge_rect = { 150, window_height - 150, dialouge_rect.w, dialouge_rect.h };
  dialouge_bg_rect.y = dialouge_rect.y - 5;
  dialouge_bg_rect.w = window_width - 290;
  dialouge_bg_rect.h = 120;
  name_rect = { dialouge_rect.x - 30, dialouge_rect.y - 30, name_rect.w, name_rect.h };
  name_bg_rect = GetBackground(name_rect);
  control_rect = { window_width - control_w - 10, window_height - control_h - 10, control_w, control_h };

  while (state == APP_STATE_DIALOUGE) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        state = APP_STATE_QUIT;
      }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          SDL_GetWindowSize(window, &window_width, &window_height);
          humanoid_rect = { 100, 50, window_width - 200, window_height - 100 };
          dialouge_rect = { 150, window_height - 150, dialouge_rect.w, dialouge_rect.h };
          dialouge_bg_rect.y = dialouge_rect.y - 5;
          dialouge_bg_rect.w = window_width - 290;
          dialouge_bg_rect.h = 120;
          name_rect = { dialouge_rect.x - 30, dialouge_rect.y - 30, name_rect.w, name_rect.h };
          name_bg_rect = GetBackground(name_rect);
          control_rect = { window_width - control_w - 10, window_height - control_h - 10, control_w, control_h };
        }
      }
      if (event.type == SDL_KEYDOWN) {    
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          state = APP_STATE_MAIN_MENU;
        }
        currentDialougeIndex++;
        current_dialouge_text = "";
      }
    }
    if (currentDialougeIndex == humanoid.messages.size()) {
      std::cout << "Reached end of dialouge queue\n";
      state = APP_STATE_GAME;
    }
    if (currentDialougeIndex < humanoid.messages.size() && current_dialouge_text.size() != humanoid.messages[currentDialougeIndex].size()) {
      SDL_DestroyTexture(dialouge_texture);
      if (current_dialouge_text.size() != humanoid.messages[currentDialougeIndex].size()) {
        current_dialouge_text += humanoid.messages[currentDialougeIndex][std::clamp(static_cast<int>(current_dialouge_text.size()), 0, static_cast<int>(humanoid.messages[currentDialougeIndex].size() - 1))];
      }
      dialouge_texture = ResLoad::RenderText(font, current_dialouge_text, {255, 255, 255}, window_width - 200);
      SDL_QueryTexture(dialouge_texture, NULL, NULL, &dialouge_rect.w, &dialouge_rect.h);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, humanoid.texture, NULL, &humanoid_rect);
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
  std::cout << "Configuring humanoid\n";
  if (current_humanoid != nullptr) {
    current_humanoid->interacted = true;
    current_humanoid->has_critical_update = false;
  }
  
  std::cout << "Unloading assets\n";
  SDL_DestroyTexture(dialouge_texture);
  SDL_DestroyTexture(character_name_texture);
  ResLoad::FreeFont(topFont);
  ResLoad::FreeFont(font);
  state = APP_STATE_GAME;
}
