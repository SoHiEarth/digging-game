#include "config.h"
#include "items.hpp"
#include <SDL_image.h>
#include <SDL_keyboard.h>
#include <SDL_render.h>
#include <application.hpp>
#include <animate.hpp>
#include <base.hpp>
#include <renderer_temp.hpp>
#include <SDL.h>
#include <algorithm>
#include <humanoid.h>
#include <cstdlib>
std::map<SDL_Keycode, bool> key_states, prev_key_states;
void Application::game_fixed() {
  while (state == APP_STATE_GAME) {
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
    int playerW, playerH;
    SDL_QueryTexture(player.playerSprite, NULL, NULL, &playerW, &playerH);
    SDL_Point playerPos_Max = {player.position.x + playerW, player.position.y + playerH};
    player.position.x = std::clamp(player.position.x, 0, 800 - 64);
    player.position.y = std::clamp(player.position.y, 0, 600 - 64);
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
    for (Hole* hole : holesVec) {
      if (player_up) {
        if (player.energy > 0) hole->holeRect.y += player.move_speed;
        else hole->holeRect.y += player.move_speed / 2;
      }
      if (player_down) {
        if (player.energy > 0) hole->holeRect.y -= player.move_speed;
        else hole->holeRect.y -= player.move_speed / 2;
      }
      if (player_left) {
        if (player.energy > 0) hole->holeRect.x += player.move_speed;
        else hole->holeRect.x += player.move_speed / 2;
      }
      if (player_right) {
        if (player.energy > 0) hole->holeRect.x -= player.move_speed;
        else hole->holeRect.x -= player.move_speed / 2;
      }
    }
    SDL_Delay(1000/30);
  }
  return;
}

void Application::game() {
  SDL_SetWindowTitle(window, "Holes - Game");
  key_states.clear();
  PreloadHoleTexture();
  PreloadStatusBarIcons();
  PreloadPlayerSprite(); 
  ResetPlayerStats();
  PreloadMapTexture();
  func_button_pressed = false;
  talk_button_pressed = false;
  inventoryFont = TTF_OpenFont(current_asset_bundle.FONT_GAME_INVENTORY_PATH.c_str(), 16);
  player_up = false, player_down = false, player_left = false, player_right = false;
  gameThread = std::thread(&Application::game_fixed, this);
  if (player.move_speed == 0) player.move_speed = _PLAYER_MOVE_SPEED;
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
    }

    if (key_states[SDLK_ESCAPE]) state = APP_STATE_QUIT;
    player_up = key_states[SDLK_w];
    player_down = key_states[SDLK_s];
    player_left = key_states[SDLK_a];
    player_right = key_states[SDLK_d];
    if (key_states[SDLK_e] && !prev_key_states[SDLK_e]) {
      func_button_pressed = true;
      if (!player.inventory.empty()) {
        player.inventory[std::clamp(player.currentItem, 0, (int)player.inventory.size())]->func();
      }
    }
    else func_button_pressed = false;
    talk_button_pressed = key_states[SDLK_f];
    for (int i = 1; i < 10; i++) {
      if (key_states[SDL_GetKeyFromName(std::to_string(i).c_str())]) {
        player.currentItem = std::clamp(i - 1, 0, static_cast<int>(player.inventory.size()));
      }
    }
    if (player.currentItem > player.inventory.size() - 1) player.currentItem = 0;
    SDL_SetRenderDrawColor(renderer, 224, 172, 105, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, mapTexture_Part_Hill, NULL, &mapRect);
    for (Hole* hole : holesVec) {
      RenderHole(*hole);
    }
    SDL_RenderCopy(renderer, player.playerSprite, NULL, &playerRect);
    if (!player.inventory.empty()) {
      if (player.inventory[player.currentItem]->sprite == nullptr) {
        player.inventory[player.currentItem]->sprite = IMG_LoadTexture(renderer, player.inventory[player.currentItem]->itemSpritePath.c_str());
      }
      SDL_RenderCopy(renderer, player.inventory[player.currentItem]->sprite, NULL, &itemRect);
    }
    // Render Player RenderPlayerStats
    RenderPlayerStats();
    RenderInventory();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 - (255 * (globalBrightness / 100.0)));
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
  }
  gameThread.join();
}
