#include <SDL_pixels.h>
#include <SDL_render.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <config.h>

#ifdef WIN32
#include <windows.h>
void errorWindow(const char* message) {
  MessageBox(NULL, message, "Error", MB_ICONERROR | MB_OK);
}

#else
#include <iostream>
void errorWindow(const char* message) {
  std::cout << "[Error] " << message << "\n";
}
#endif
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
std::thread renderThread;
std::thread gameThread;
bool running = false;

SDL_Texture *hpIconTexture = nullptr, *thirstIconTexture = nullptr, *energyIconTexture = nullptr;
TTF_Font *widgetFont = nullptr, *inventoryFont = nullptr;
SDL_Texture* mapTexture_Part_Hill = nullptr;
SDL_Rect mapRect = { 0, 0, 3200, 2400 };
bool player_Up = false, player_Down = false, player_Left = false, player_Right = false;
SDL_Rect playerRect = { 384, 284, 64, 64 }, itemRect = { 415, 300, 35, 35 };
SDL_Rect chargeRectBg = { 400 - 50 + 16, 300 - 50, 100, 10 };
SDL_Texture* hole_unifiedTexture = nullptr;
bool func_button_pressed = false;
float shovelDiggingChargeProgress = 0;

struct Item {
  public:
    std::string itemName = ITEM_NULL_NAME;
    std::string itemDescription = ITEM_NULL_DESCRIPTION;
    SDL_Texture* sprite = nullptr;
    std::string itemSpritePath = ITEM_NULL_SPRITE_PATH;
    virtual void func() { throw std::runtime_error("Item function not configured"); }
};

struct MapInteractable {
  public:
    std::string interactableName = INTERACTABLE_NULL_NAME;
    SDL_Rect rect;
    SDL_Texture* texture;
    std::string texturePath = INTERACTABLE_NULL_SPRITE_PATH;
    virtual void func() { throw std::runtime_error("Interactable function not configured"); }
};

class Player {
  public:
    int x = 384, y = 284;
    float health = 100, energy = 100, thirst = 100;
    SDL_Texture* playerSprite = nullptr;
    std::vector<Item*> inventory;
    int currentItem = 0;
    int moveSpeed = PLAYER_MOVE_SPEED;
};

Player player;
  
struct Hole {
  SDL_Rect holeRect;
};
std::vector<Hole*> holesVec;
void PreloadHoleTexture() {
  hole_unifiedTexture = IMG_LoadTexture(renderer, HOLE_UNIFIED_TEXTURE_PATH);
}

struct Shovel : public Item {
public:
    Shovel() {
        itemName = SHOVEL_ITEM_NAME;
        itemSpritePath = SHOVEL_ITEM_SPRITE_PATH;
        itemDescription = SHOVEL_ITEM_DESCRIPTION;
    }

    std::thread holdThread;

    void charge() {
        while (func_button_pressed && shovelDiggingChargeProgress <= 100) {
            if (player.energy > 0) shovelDiggingChargeProgress++;
            else shovelDiggingChargeProgress += 0.5;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        if (shovelDiggingChargeProgress >= 100) {
            Hole* hole = new Hole();
            hole->holeRect.x = 400;
            hole->holeRect.y = 300 + 30;
            holesVec.push_back(hole);
            player.energy -= 10;
            player.thirst -= 10;
            if (player.thirst < 0) player.thirst = 0;
            if (player.energy < 0) player.energy = 0;
        }
        shovelDiggingChargeProgress = 0;
    }

    void func() override {
        if (shovelDiggingChargeProgress == 0) {
            if (holdThread.joinable()) holdThread.join();
            holdThread = std::thread(&Shovel::charge, this);
        }
    }

    ~Shovel() {
        if (holdThread.joinable()) {
            holdThread.join();
        }
    }
};

struct Bottle : public Item {
  public:
    Bottle() {
      itemName = BOTTLE_ITEM_NAME;
      itemSpritePath = BOTTLE_ITEM_SPRITE_PATH;
      itemDescription = std::to_string(capacity) + "%";
    }
    
    int capacity = 100;
    void func() override {
      if (capacity > 0) {
        if (player.thirst >= 99) return;
        player.thirst += 10;
        if (player.thirst > 100) player.thirst = 100;
        capacity -= 20;
        itemDescription = std::to_string(capacity) + "%";
      } else {
        player.inventory.erase(player.inventory.begin() + player.currentItem);
      }
    }
};

struct Food : public Item {
  public:
    Food() {
      itemName = FOOD_ITEM_NAME;
      itemSpritePath = FOOD_ITEM_SPRITE_PATH;
      itemDescription = std::to_string(capacity) + "%";
    }

    int capacity = 100;
    void func() override {
      if (capacity > 0) {
        if (player.energy >= 100) return;
        player.energy += 10;
        if (player.energy > 100) player.energy = 100;
        capacity -= 20;
        itemDescription = std::to_string(capacity) + "%";
      } else {
        player.inventory.erase(player.inventory.begin() + player.currentItem);
      }
    }
};

struct WaterRefillStation : public MapInteractable {
  public:
    WaterRefillStation() {
      interactableName = WATER_REFILL_STATION_NAME;
      texturePath = WATER_REFILL_STATION_SPRITE_PATH;
      rect = { 0, 0, 64, 128 }; 
    }

    void func() override {
      for (int i = 0; i < player.inventory.size(); i++) {
        if (player.inventory[i]->itemName == "Bottle") {
          player.inventory.erase(player.inventory.begin() + i);
          break;
        }
      }
      player.inventory.push_back(new Bottle());
    }
};
WaterRefillStation* waterRefillStation = nullptr;
SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color color) {
  SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

void PreloadStatusBarIcons() {
  hpIconTexture = IMG_LoadTexture(renderer, PLAYERSTAT_HEALTH_ICON_PATH);
  thirstIconTexture = IMG_LoadTexture(renderer, PLAYERSTAT_THIRST_ICON_PATH);
  energyIconTexture = IMG_LoadTexture(renderer, PLAYERSTAT_ENERGY_ICON_PATH);
  widgetFont = TTF_OpenFont(PLAYERSTAT_FONT_PATH, 24);  
}

void PreloadPlayerSprite() {
  if (player.playerSprite != nullptr) SDL_DestroyTexture(player.playerSprite);
  player.playerSprite = IMG_LoadTexture(renderer, PLAYER_SPRITE_PATH);
  if (player.playerSprite == NULL) throw std::runtime_error("Error loading player sprite");
}

void ResetPlayerStats() {
  player.health = 100;
  player.energy = 100;
  player.thirst = 100;
  if (!player.inventory.empty()) {
    for (Item* item : player.inventory) {
      delete item;
    }
  }
  player.inventory.clear();
  player.inventory.push_back(new Shovel());
  player.inventory.push_back(new Bottle());
  player.inventory.push_back(new Food());
}

void PreloadMapTexture() {
  if (mapTexture_Part_Hill != nullptr) SDL_DestroyTexture(mapTexture_Part_Hill);
  mapTexture_Part_Hill = IMG_LoadTexture(renderer, MAP_PART_HILL_PATH);
  if (mapTexture_Part_Hill == NULL) throw std::runtime_error("Error loading map texture");
}

void RenderChargeBar() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &chargeRectBg);
  SDL_Rect chargeRect = { chargeRectBg.x, chargeRectBg.y, static_cast<int>(shovelDiggingChargeProgress), chargeRectBg.h };
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(renderer, &chargeRect);
}

void RenderWidget(SDL_Rect anchor, SDL_Texture* icon, int val, int maxVal, SDL_Color theme){
  SDL_Rect iconRect, valRect, barRect;
  SDL_Texture* valTexture = nullptr;
  iconRect = { anchor.x + 5, anchor.y + 2, 32, 32 };
  valTexture = renderText(std::to_string(val).c_str(), widgetFont, theme);
  SDL_QueryTexture(valTexture, NULL, NULL, &valRect.w, &valRect.h);
  valRect.x = iconRect.x + iconRect.w + 5;
  valRect.y = iconRect.y + (iconRect.h - valRect.h) * 0.5;
  barRect = { iconRect.x + 5, iconRect.y + iconRect.h - 1, static_cast<int>((anchor.w - 15) * (static_cast<float>(val) / maxVal)), 2 };

  SDL_SetRenderDrawColor(renderer, theme.r, theme.g, theme.b, 50);
  SDL_RenderFillRect(renderer, &anchor);
  SDL_RenderCopy(renderer, icon, NULL, &iconRect);
  SDL_RenderCopy(renderer, valTexture, NULL, &valRect);
  SDL_SetRenderDrawColor(renderer, theme.r, theme.g, theme.b, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &barRect);
  SDL_DestroyTexture(valTexture);
}

void RenderPlayerStats() {
  SDL_Rect playerStat_Background = { 20, 480, 200, 95 };
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
  SDL_RenderFillRect(renderer, &playerStat_Background);

  SDL_Rect playerStat_HP_Anchor = { playerStat_Background.x + 5,
    playerStat_Background.y + 5,
    190, 40 };
  SDL_Rect playerStat_Energy_Anchor = { playerStat_HP_Anchor.x,
    playerStat_HP_Anchor.y + playerStat_HP_Anchor.h + 5,
    95, 40 };
  SDL_Rect playerStat_Thirst_Anchor = { playerStat_Background.x + playerStat_Background.w / 2,
    playerStat_Energy_Anchor.y,
    95, 40 };
  
  RenderWidget(playerStat_HP_Anchor, hpIconTexture, static_cast<int>(player.health), 100, {255, 0, 0, 255});
  RenderWidget(playerStat_Energy_Anchor, energyIconTexture, static_cast<int>(player.energy), 100, {0, 255, 0, 255});
  RenderWidget(playerStat_Thirst_Anchor, thirstIconTexture, static_cast<int>(player.thirst), 100, {0, 0, 255, 255});
  
}

Item* inv_prevItem = nullptr, *inv_currItem = nullptr, *inv_nextItem = nullptr, *prevItem = nullptr;
void RenderItem(Item* item, SDL_Rect anchor, int alpha) {
  SDL_Rect itemIconRect = { anchor.x + 5, anchor.y, 32, 32 };
  SDL_Texture* itemNameTexture = renderText(item->itemName.c_str(), inventoryFont, {255, 255, 255, static_cast<Uint8>(alpha)});
  SDL_Texture* itemDescTexture = renderText(item->itemDescription.c_str(), inventoryFont, {255, 255, 255, static_cast<Uint8>(alpha)});

  SDL_Rect itemNameRect, itemDescRect;
  SDL_QueryTexture(itemNameTexture, NULL, NULL, &itemNameRect.w, &itemNameRect.h);
  SDL_QueryTexture(itemDescTexture, NULL, NULL, &itemDescRect.w, &itemDescRect.h);

  itemNameRect = { itemIconRect.x + itemIconRect.w + 5, itemIconRect.y, itemNameRect.w, itemNameRect.h };
  itemDescRect = { itemIconRect.x + itemIconRect.w + itemNameRect.w + 10, itemIconRect.y, itemDescRect.w, itemDescRect.h };

  SDL_RenderCopy(renderer, item->sprite, NULL, &itemIconRect);
  SDL_RenderCopy(renderer, itemNameTexture, NULL, &itemNameRect);
  SDL_RenderCopy(renderer, itemDescTexture, NULL, &itemDescRect);
}

void RenderInventory() {
  SDL_Rect anchorRect = { 230, 480, 180, 95 };
  if (player.currentItem - 1 >= 0) inv_prevItem = player.inventory[player.currentItem - 1];
  else inv_prevItem = nullptr;
  inv_currItem = player.inventory[player.currentItem];
  if (player.currentItem + 1 < player.inventory.size()) inv_nextItem = player.inventory[player.currentItem + 1];
  else inv_nextItem = nullptr;

  SDL_Rect prevItemRect = { anchorRect.x + 5, anchorRect.y, anchorRect.w, 30 };
  SDL_Rect currItemRect = { prevItemRect.x, anchorRect.y + 30, anchorRect.w, 30 };
  SDL_Rect nextItemRect = { prevItemRect.x, anchorRect.y + 60, anchorRect.w, 30 };

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
  SDL_RenderFillRect(renderer, &anchorRect);
  if (inv_prevItem != nullptr) RenderItem(inv_prevItem, prevItemRect, 150);
  RenderItem(inv_currItem, currItemRect, 255);
  if (inv_nextItem != nullptr) RenderItem(inv_nextItem, nextItemRect, 150);
}

class Application {

  enum ApplicationState {
    APP_STATE_MAIN_MENU,
    APP_STATE_GAME,
    APP_STATE_PAUSE,
    APP_STATE_GAME_OVER,
    APP_STATE_QUIT
  };

  ApplicationState state = APP_STATE_QUIT;

  public:
  void Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      throw std::runtime_error("SDL_Init failed");
    }

    if (TTF_Init() != 0) {
      throw std::runtime_error("TTF_Init failed");
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
      throw std::runtime_error("IMG_Init failed");
    }

    if (Mix_Init(MIX_INIT_WAVPACK) == 0) {
      throw std::runtime_error("Mix_Init failed");
    }

    state = APP_STATE_MAIN_MENU;
    running = true;
  }

  void mainMenu() {
    SDL_SetWindowTitle(window, "Holes - Main Menu");
    TTF_Font* hintFont = TTF_OpenFont(FONT_MENU_HINT_PATH, 20);
    if (hintFont == NULL) throw std::runtime_error("Error loading hint font");
    
    SDL_Surface* hintSurface = TTF_RenderText_Blended_Wrapped(hintFont, "Press Any Key to start\nPress [Esc] to quit", {255, 255, 255, 255}, 0);
    SDL_Texture* hintTexture = SDL_CreateTextureFromSurface(renderer, hintSurface);
    if (hintTexture == NULL) throw std::runtime_error("Error loading hint texture");
    SDL_FreeSurface(hintSurface);

    SDL_Rect hintRect;
    SDL_QueryTexture(hintTexture, NULL, NULL, &hintRect.w, &hintRect.h);
    hintRect = { 80, (600 - hintRect.h) / 2, hintRect.w, hintRect.h };

    SDL_Rect titleRect = { 0, 0, 800, 600};
    SDL_Texture* titleTexture = IMG_LoadTexture(renderer, MENU_BACKGROUND_PATH);
    if (titleTexture == NULL) throw std::runtime_error("Error loading title texture");

    while (state == APP_STATE_MAIN_MENU) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                state = APP_STATE_QUIT;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        state = APP_STATE_QUIT;
                        break;
                    default:
                        state = APP_STATE_GAME;
                        break;
                }
            }
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect hintBgRect = { hintRect.x - 10, hintRect.y - 10, hintRect.w + 20, hintRect.h + 20 };
        SDL_RenderFillRect(renderer, &hintBgRect);
        SDL_RenderCopy(renderer, hintTexture, NULL, &hintRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(hintTexture);
    TTF_CloseFont(hintFont);
  }

  void game_fixed() {
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
      if (SDL_PointInRect(&playerPos, &waterRefillStation->rect) == SDL_TRUE) {
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
      SDL_Delay(1000/60);
    }
    return;
  }

  void game() {
    SDL_SetWindowTitle(window, "Holes - Game");
    PreloadHoleTexture();
    PreloadStatusBarIcons();
    PreloadPlayerSprite();
    ResetPlayerStats();
    PreloadMapTexture();
    inventoryFont = TTF_OpenFont("assets/Inter.ttf", 16);

    if (waterRefillStation == nullptr)
      waterRefillStation = new WaterRefillStation();

    gameThread = std::thread(&Application::game_fixed, this);

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
          }
        }
      }

      if (player.currentItem > player.inventory.size() - 1) player.currentItem = 0;

      SDL_SetRenderDrawColor(renderer, 224, 172, 105, 255);
      SDL_RenderClear(renderer);
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_RenderCopy(renderer, mapTexture_Part_Hill, NULL, &mapRect);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);

      if (waterRefillStation->texture == nullptr) {
        waterRefillStation->texture = IMG_LoadTexture(renderer, waterRefillStation->texturePath.c_str());
      }
      SDL_RenderFillRect(renderer, &waterRefillStation->rect);

      for (Hole* hole : holesVec) {
        SDL_Rect holeRect = { hole->holeRect.x, hole->holeRect.y, 32, 32 };
        SDL_RenderCopy(renderer, hole_unifiedTexture, NULL, &holeRect);
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

  TTF_Font* gameOverFont = nullptr, *gameOverHintFont = nullptr;
  SDL_Texture* gameOverTexture;
  SDL_Texture* gameOverHintTexture;
  SDL_Rect gameOverRect = { 0, 0, 800, 600 }, gameOverHintRect = { 0, 0, 800, 600 };
  void gameOver() {
    if (gameOverFont == nullptr) {
      gameOverFont = TTF_OpenFont(FONT_GAMEOVER_PATH, 32);
      if (gameOverFont == NULL) throw std::runtime_error("Error loading game over font");
    }

    if (gameOverHintFont == nullptr) {
      gameOverHintFont = TTF_OpenFont(FONT_GAMEOVER_HINT_PATH, 16);
      if (gameOverHintFont == NULL) throw std::runtime_error("Error loading game over hint font");
    }

    gameOverTexture = renderText("Game Over", gameOverFont, {255, 0, 0, 255});
    SDL_QueryTexture(gameOverTexture, NULL, NULL, &gameOverRect.w, &gameOverRect.h);
    gameOverRect = { (800 - gameOverRect.w) / 2, (600 - gameOverRect.h) / 2, gameOverRect.w, gameOverRect.h };

    gameOverHintTexture = renderText("[Press Any Key to Continue]", gameOverHintFont, {255, 255, 255, 255});
    SDL_QueryTexture(gameOverHintTexture, NULL, NULL, &gameOverHintRect.w, &gameOverHintRect.h);
    gameOverHintRect = { (800 - gameOverHintRect.w) / 2, 600 - gameOverHintRect.h - 10, gameOverHintRect.w, gameOverHintRect.h };

    while (state == APP_STATE_GAME_OVER) {
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          state = APP_STATE_QUIT;
        }
        if (event.type == SDL_KEYDOWN) {
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              state = APP_STATE_QUIT;
              break;
            default:
              state = APP_STATE_MAIN_MENU;
              break;
          }
        }
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
      SDL_RenderCopy(renderer, gameOverHintTexture, NULL, &gameOverHintRect);

      SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(gameOverHintTexture);
    TTF_CloseFont(gameOverFont);
    TTF_CloseFont(gameOverHintFont);
  }

  void Run() {
    window = SDL_CreateWindow(APP_TITLE, 0, 25, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) throw std::runtime_error("Error creating window");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) throw std::runtime_error("Error creating renderer");

    while (running) {
      switch (state) {
        case APP_STATE_MAIN_MENU:
          mainMenu();
          break;
        case APP_STATE_GAME:
          game();
          break;
        case APP_STATE_QUIT:
          running = false;
          break;
        case APP_STATE_PAUSE:
          break;
        case APP_STATE_GAME_OVER:
          gameOver();
          break;
        default:
          throw std::runtime_error("Unknown Application State encountered!");
          break;
      }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }

  void Quit() {
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
  }
};
Application* app;
int main() {

  app = new Application();

  std::cout << "Initializing application\n";

  try {
    app->Init();
  }
  catch (std::runtime_error& e) {
    std::string msg = std::string(e.what()) + "\nSDL: " + SDL_GetError();
    errorWindow(msg.c_str());
    delete app;
    return -1;
  }

  std::cout << "Running application\n";

  try {
    app->Run();
  }
  catch (std::runtime_error& e) {
    std::string msg = std::string(e.what()) + "\nSDL: " + SDL_GetError();
    errorWindow(msg.c_str());
    delete app;
    return -1;
  }

  std::cout << "Quitting application\n";

  try {
    app->Quit();
  }
  catch (std::runtime_error& e) {
    std::string msg = std::string(e.what()) + "\nSDL: " + SDL_GetError();
    errorWindow(msg.c_str());
    delete app;
    return -1;
  }

  return 0;
}
