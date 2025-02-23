#include <SDL_image.h>
#include <application.h>
#include <animate.h>
#include <base.h>
#include <renderer_temp.h>
#include <SDL.h>
#include <algorithm>
#include <humanoid.h>
#include <cstdlib>
#include <error.h>
#include <iostream>
#include <level.h>
#include <resload.h>
std::map<SDL_Keycode, bool> key_states, prev_key_states;
void Application::Fixed(std::atomic<bool>& running) {
  while (running && state == APP_STATE_GAME) {
    player.energy = std::clamp(player.energy, 0.0f, 100.0f);
    if (player_up || player_down || player_left || player_right) {
      player.energy -= 0.01;
    }
    if (player.thirst <= 0) player.health -= 0.05;
    else player.thirst -= 0.01;
    if (player.energy <= 0) player.health -= 0.075;
    if (player.thirst > 90 && player.energy > 90 && player.health < 100) {
      player.health += 0.05;
    }
    player.rect.x = std::clamp(player.rect.x, 0, window_width - 64);
    player.rect.y = std::clamp(player.rect.y, 0, window_height - 64);
    if (player_up) {
      if (player.energy > 0) mapRect.y += player.move_speed;
      else mapRect.y += player.move_speed / 2;
    }
    if (player_down) {
      if (player.energy > 0) mapRect.y -= player.move_speed;
      else mapRect.y -= player.move_speed / 2;
    }
    if (player_left) {
      if (player.energy > 0) mapRect.x += player.move_speed;
      else mapRect.x += player.move_speed / 2;
    }
    if (player_right) {
      if (player.energy > 0) mapRect.x -= player.move_speed;
      else mapRect.x -= player.move_speed / 2;
    }
    for (Object* object : level.objects) {
      object->Fixed();
    }
    for (Hole* hole : holes_vector) {
      if (player_up) {
        if (player.energy > 0) hole->rect.y += player.move_speed;
        else hole->rect.y += player.move_speed / 2;
      }
      if (player_down) {
        if (player.energy > 0) hole->rect.y -= player.move_speed;
        else hole->rect.y -= player.move_speed / 2;
      }
      if (player_left) {
        if (player.energy > 0) hole->rect.x += player.move_speed;
        else hole->rect.x += player.move_speed / 2;
      }
      if (player_right) {
        if (player.energy > 0) hole->rect.x -= player.move_speed;
        else hole->rect.x -= player.move_speed / 2;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
  }
}

void Application::Game() {
  SDL_SetWindowTitle(window, "Holes - Game");
  LoadHoleTexture();
  LoadStatusBarIcons();
  LoadPlayerSprite();
  PreloadMapTexture();
  func_button_pressed = false;
  talk_button_pressed = false;
  key_states.clear();
  inventoryFont = ResLoad::LoadFont(current_asset_bundle.FONT_GAME_INVENTORY_PATH.c_str(), 16);
  player_up = false, player_down = false, player_left = false, player_right = false;
  if (player.move_speed == 0) player.move_speed = _PLAYER_MOVE_SPEED;
  if (!level.loaded) {
    level.Load("assets/1.lvl");
    ResetPlayerStats();
  }
  level.fixed_thread.Start(std::bind(&Application::Fixed, this, std::placeholders::_1), "Fixed");
  while (state == APP_STATE_GAME) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        state = APP_STATE_MAIN_MENU;
      }
      if (event.type == SDL_KEYDOWN) {
        key_states[event.key.keysym.sym] = true;
      }
      if (event.type == SDL_KEYUP) {
        key_states[event.key.keysym.sym] = false;
      }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          SDL_GetWindowSize(window, &window_width, &window_height);
          player.rect.x = (window_width - player.rect.w) / 2;
          player.rect.y = (window_height - player.rect.h) / 2;
        }
      }
    }
    if (key_states[SDLK_ESCAPE]) state = APP_STATE_QUIT;
    player_up = key_states[SDLK_w];
    player_down = key_states[SDLK_s];
    player_left = key_states[SDLK_a];
    player_right = key_states[SDLK_d];
    if (key_states[SDLK_e] && !prev_key_states[SDLK_e]) {
      func_button_pressed = true;
      if (!player.inventory.empty()) {
        player.inventory[std::clamp(player.current_item, 0, (int)player.inventory.size())]->func();
      }
    }
    else func_button_pressed = false;
    talk_button_pressed = key_states[SDLK_f];
    for (int i = 1; i < 10; i++) {
      if (key_states[SDL_GetKeyFromName(std::to_string(i).c_str())]) {
        player.current_item = std::clamp(i - 1, 0, static_cast<int>(player.inventory.size()));
      }
    }
    if (player.current_item > player.inventory.size() - 1) player.current_item = 0;
    SDL_SetRenderDrawColor(renderer, 224, 172, 105, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, map_texture_Part_Hill, NULL, &mapRect);
    for (Object* object : level.objects) {
      object->Update();
    }
    level.camera.Render();
    SDL_RenderCopy(renderer, player.texture, NULL, &player.rect);
    if (!player.inventory.empty()) {
      if (player.inventory[player.current_item]->sprite == nullptr) {
        player.inventory[player.current_item]->sprite = IMG_LoadTexture(renderer, player.inventory[player.current_item]->itemSpritePath.c_str());
      }
      SDL_Rect item_rect = {
        player.rect.x + 5,
        player.rect.y + 10,
        player.rect.w - 10,
        player.rect.h - 15
      };
      SDL_RenderCopy(renderer, player.inventory[player.current_item]->sprite, NULL, &item_rect);
    }
    RenderPlayerStats();
    RenderInventory();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 * (100 - global_brightness) / 100);
    SDL_RenderFillRect(renderer, NULL);
    // Red tint if hp is low
    if (player.health <= 10) {
      Uint8 redAlpha = static_cast<Uint8>(255 * ((10 - player.health) / 10.0));
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, redAlpha);
      SDL_RenderFillRect(renderer, NULL);
    }
    // Gradually fade to black
    if (player.health <= 5) {
      Uint8 blackAlpha = static_cast<Uint8>(255 * (5 - (player.health / 5.0)));
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, blackAlpha);
      SDL_RenderFillRect(renderer, NULL);
    }
    if (player.health <= 0) {
      state = APP_STATE_GAME_OVER;
    }
    SDL_RenderPresent(renderer);
    prev_key_states = key_states;
    SDL_Delay(1000/60);
    if (level.HasNextFramePath()) {
      level.LoadNextFramePath();
    }
    std::cout << "Frame end.\r";
  }
}
