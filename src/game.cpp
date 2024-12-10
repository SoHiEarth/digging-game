#include "character.hpp"
#include "items.hpp"
#include <SDL_image.h>
#include <SDL_keycode.h>
#include <SDL_rect.h>
#include <application.hpp>

#include <base.hpp>
#include <renderer_temp.hpp>
#include <SDL.h>
#include <iostream>
#include <mutex>

#ifdef WIN32

#include <Windows.h>

void SetConsoleColor(int attr) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, attr);
}
#endif

void Application::game_fixed() {
  while (state == APP_STATE_GAME) {
    if (player_Up || player_Down || player_Left || player_Right) {
      player.energy -= 0.01;
    }
    if (player.energy < 0) player.energy = 0;
    if (player.thirst <= 0) player.health -= 0.05;
    else player.thirst -= 0.01;
    if (player.energy <= 0) player.health -= 0.075;
    
    if (player.thirst > 90 && player.energy > 90 && player.health < 100) {
      player.health += 0.05;
    }
    SDL_Point playerPos = {player.x, player.y};
    int playerW, playerH;
    SDL_QueryTexture(player.playerSprite, NULL, NULL, &playerW, &playerH);
    SDL_Point playerPos_Max = {player.x + playerW, player.y + playerH};
    if (SDL_PointInRect(&playerPos, &waterRefillStation->rect) == SDL_TRUE || SDL_PointInRect(&playerPos_Max, &waterRefillStation->rect)) {
      waterRefillStation->func();
    }
    if (player.x < 0) player.x = 0;
    if (player.y < 0) player.y = 0;
    if (player.x > 800 - 64) player.x = 800 - 64;
    if (player.y > 600 - 64) player.y = 600 - 64;
    if (player_Up) {
      if (player.energy > 0) mapRect.y += player.moveSpeed;
      else mapRect.y += player.moveSpeed / 2;
    }
    if (player_Down) {
      if (player.energy > 0) mapRect.y -= player.moveSpeed;
      else mapRect.y -= player.moveSpeed / 2;
    }
    if (player_Left) {
      if (player.energy > 0) mapRect.x += player.moveSpeed;
      else mapRect.x += player.moveSpeed / 2;
    }
    if (player_Right) {
      if (player.energy > 0) mapRect.x -= player.moveSpeed;
      else mapRect.x -= player.moveSpeed / 2;
    }
    for (Hole* hole : holesVec) {
      if (player_Up) {
        if (player.energy > 0) hole->holeRect.y += player.moveSpeed;
        else hole->holeRect.y += player.moveSpeed / 2;
      }
      if (player_Down) {
        if (player.energy > 0) hole->holeRect.y -= player.moveSpeed;
        else hole->holeRect.y -= player.moveSpeed / 2;
      }
      if (player_Left) {
        if (player.energy > 0) hole->holeRect.x += player.moveSpeed;
        else hole->holeRect.x += player.moveSpeed / 2;
      }
      if (player_Right) {
        if (player.energy > 0) hole->holeRect.x -= player.moveSpeed;
        else hole->holeRect.x -= player.moveSpeed / 2;
      }
    }
    // Render map interactables
    
    if (player_Up) {
      if (player.energy > 0) waterRefillStation->rect.y += player.moveSpeed;
      else waterRefillStation->rect.y += player.moveSpeed / 2; 
    }
    if (player_Down) {
      if (player.energy > 0) waterRefillStation->rect.y -= player.moveSpeed;
      else waterRefillStation->rect.y -= player.moveSpeed / 2;
    }
    if (player_Left) {
      if (player.energy > 0) waterRefillStation->rect.x += player.moveSpeed;
      else waterRefillStation->rect.x += player.moveSpeed / 2;
    }
    if (player_Right) {
      if (player.energy > 0) waterRefillStation->rect.x -= player.moveSpeed;
      else waterRefillStation->rect.x -= player.moveSpeed / 2;
    }
            
    {
      std::lock_guard<std::mutex> lock(humanoidsMutex);
      for (Humanoid* humanoid : humanoidsVec) {
        if (player_Up) {
          if (player.energy > 0) humanoid->characterRect.y += player.moveSpeed;
          else humanoid->characterRect.y += player.moveSpeed / 2;
        }

        if (player_Down) {
          if (player.energy > 0) humanoid->characterRect.y -= player.moveSpeed;
          else humanoid->characterRect.y -= player.moveSpeed / 2;
        }

        if (player_Left) {
          if (player.energy > 0) humanoid->characterRect.x += player.moveSpeed;
          else humanoid->characterRect.x += player.moveSpeed / 2;
        }

        if (player_Right) {
          if (player.energy > 0) humanoid->characterRect.x -= player.moveSpeed;
          else humanoid->characterRect.x -= player.moveSpeed / 2;
        }
      }
    }

    SDL_Delay(1000/60);
  }
  return;
}

void Application::game() {
  SDL_SetWindowTitle(window, "Holes - Game");
  PreloadHoleTexture();
  PreloadStatusBarIcons();
  PreloadPlayerSprite(); 
  ResetPlayerStats();
  PreloadMapTexture();
  func_button_pressed = false;
  talk_button_pressed = false;
  inventoryFont = TTF_OpenFont(FONT_GAME_INVENTORY_PATH, 16);

  player_Up = false, player_Down = false, player_Left = false, player_Right = false;
  
  {
    std::lock_guard<std::mutex> lock(humanoidsMutex);
    if (humanoidsVec.empty()) humanoidsVec.push_back(new TestHumanoid());
  }

  if (waterRefillStation == nullptr)
    waterRefillStation = new WaterRefillStation();
  
  gameThread = std::thread(&Application::game_fixed, this);

  std::cout << "Preload complete\n";

  while (state == APP_STATE_GAME) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        state = APP_STATE_MAIN_MENU;
      }
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            state = APP_STATE_QUIT;
            break;
          case SDLK_w:
            player_Up = true;
            break;
          case SDLK_s:
            player_Down = true;
            break;
          case SDLK_a:
            player_Left = true;
            break;
          case SDLK_d:
            player_Right = true;
            break;
          case SDLK_e:
            func_button_pressed = true;
            if (!player.inventory.empty()) player.inventory[player.currentItem]->func();
            break;
          case SDLK_f:
            talk_button_pressed = true;
            break;
          case SDLK_1:
            player.currentItem = 0;
            break;
          case SDLK_2:
            player.currentItem = 1;
            if (player.inventory.size() < 2) player.currentItem = 0;
            break;
          case SDLK_3:
            player.currentItem = 2;
            if (player.inventory.size() < 3) player.currentItem = 0;
            break;
          case SDLK_4:
            player.currentItem = 3;
            if (player.inventory.size() < 4) player.currentItem = 0;
            break;
        }
      }
      if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
          case SDLK_w:
            player_Up = false;
            break;
          case SDLK_s:
            player_Down = false;
            break;
          case SDLK_a:
            player_Left = false;
            break;
          case SDLK_d:
            player_Right = false;
            break; 
          case SDLK_e:
            func_button_pressed = false;
            break;
          case SDLK_f:
            talk_button_pressed = false;
            break;
        }
      }
    }
    
    SDL_Point playerPos = {player.x, player.y};

    if (player.currentItem > player.inventory.size() - 1) player.currentItem = 0;
    SDL_SetRenderDrawColor(renderer, 224, 172, 105, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, mapTexture_Part_Hill, NULL, &mapRect);

    // Temporary render of water refill station
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    if (waterRefillStation->texture == nullptr) {
      waterRefillStation->texture = IMG_LoadTexture(renderer, waterRefillStation->texturePath.c_str());
    }
    SDL_RenderFillRect(renderer, &waterRefillStation->rect);

    for (Hole* hole : holesVec) {
      SDL_Rect holeRect = { hole->holeRect.x, hole->holeRect.y, 32, 32 };
      SDL_RenderCopy(renderer, hole_unifiedTexture, NULL, &holeRect);
    }

    bool switch_to_dialouge = false;
    {
      std::lock_guard<std::mutex> lock(humanoidsMutex);
      for (Humanoid* humanoid : humanoidsVec) {
        RenderHumanoid(humanoid);
        if (SDL_PointInRect(&playerPos, &humanoid->characterRect) && talk_button_pressed) {
          currentHumanoid = humanoid;
          switch_to_dialouge = true;
        }
      }
    }

    if (switch_to_dialouge) {
      state = APP_STATE_DIALOUGE;
    }

    SDL_RenderCopy(renderer, player.playerSprite, NULL, &playerRect);
    if (!player.inventory.empty()) {
      if (player.inventory[player.currentItem]->sprite == nullptr) {
        player.inventory[player.currentItem]->sprite = IMG_LoadTexture(renderer, player.inventory[player.currentItem]->itemSpritePath.c_str());
      }
      SDL_RenderCopy(renderer, player.inventory[player.currentItem]->sprite, NULL, &itemRect);
    }
    if (shovelDiggingChargeProgress != 0) {
      RenderChargeBar();
    }
    // Render Player RenderPlayerStats
    RenderPlayerStats();
    RenderInventory();

    // Red tint if hp is low
    if (player.health <= 20) {
      Uint8 redAlpha = static_cast<Uint8>(255 * ((20 - player.health) / 20.0));
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, redAlpha);
      SDL_RenderFillRect(renderer, NULL);
    }
    
    // Gradually fade to black
    if (player.health <= 10) {
      Uint8 blackAlpha = static_cast<Uint8>(255 * (1 - (player.health / 10.0)));
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, blackAlpha);
      SDL_RenderFillRect(renderer, NULL);
    }
    
    // Transition to main menu if health is too low
    if (player.health <= 0) {
      state = APP_STATE_GAME_OVER;
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(1000/60);
  }
  gameThread.join();
}
