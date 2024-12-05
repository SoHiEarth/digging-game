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

struct Item {
  public:
    std::string itemName = "null_item_name";
    SDL_Texture* sprite = nullptr;
    std::string itemSpritePath = "assets/null.png";
    virtual void func() { throw std::runtime_error("Item function not configured"); }
};

class Player {
  public:
    int x, y;
    float health, energy, thirst;
    SDL_Texture* playerSprite;
    std::vector<Item*> inventory;
    int currentItem = 0;
    int moveSpeed = 2;
};

Player player;
  
SDL_Texture* hole_unifiedTexture;
struct Hole {
  SDL_Rect holeRect;
};
std::vector<Hole*> holesVec;
void PreloadHoleTexture() {
  SDL_Surface* holeSurface = IMG_Load("assets/uni_tex_hole.png");
  hole_unifiedTexture = SDL_CreateTextureFromSurface(renderer, holeSurface);
  SDL_FreeSurface(holeSurface);
}

bool func_button_pressed;
int shovelDiggingChargeProgress = 0;
struct Shovel : public Item {
public:
    Shovel() {
        itemName = "Shovel";
        itemSpritePath = "assets/char_item_0.png";
    }

    std::thread holdThread;

    void charge() {
        while (func_button_pressed && shovelDiggingChargeProgress <= 100) {
            shovelDiggingChargeProgress++;
            std::cout << "Charging shovel " << shovelDiggingChargeProgress << "%\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        if (shovelDiggingChargeProgress >= 100) {
            Hole* hole = new Hole();
            hole->holeRect.x = 400 - 16;
            hole->holeRect.y = 300 + 16;
            holesVec.push_back(hole);
        }
        player.energy -= 10;
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
      itemName = "Bottle";
      itemSpritePath = "assets/char_item_1.png";
    }
    
    void func() override {
      player.thirst = 100;
    }
};

struct Food : public Item {
  public:
    Food() {
      itemName = "Food";
      itemSpritePath = "assets/char_item_2.png";
    }

    void func() override {
      player.energy = 100;
    }
};

class Application {
  /* [APP VARIABLES] */
  bool running;
  float windowRenderMultiplierX = 1, windowRenderMultiplierY = 1;

  SDL_Rect TranslateRect(SDL_Rect& rect) {
    return {
        static_cast<int>(rect.x * windowRenderMultiplierX),
        static_cast<int>(rect.y * windowRenderMultiplierY),
        static_cast<int>(rect.w * windowRenderMultiplierX),
        static_cast<int>(rect.h * windowRenderMultiplierY)
    };
}

  enum ApplicationState {
    APP_STATE_MAIN_MENU,
    APP_STATE_GAME,
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
    TTF_Font* hintFont = TTF_OpenFont("assets/font_hint_menu.ttf", 20);
    if (hintFont == NULL) throw std::runtime_error("Error loading hint font");
    
    SDL_Surface* hintSurface = TTF_RenderText_Blended_Wrapped(hintFont, "Press Any Key to start\nPress [Esc] to quit", {255, 255, 255, 255}, 0);
    SDL_Texture* hintTexture = SDL_CreateTextureFromSurface(renderer, hintSurface);
    if (hintTexture == NULL) throw std::runtime_error("Error loading hint texture");
    SDL_FreeSurface(hintSurface);

    SDL_Rect hintRect;
    SDL_QueryTexture(hintTexture, NULL, NULL, &hintRect.w, &hintRect.h);
    hintRect = { 80, (600 - hintRect.h) / 2, hintRect.w, hintRect.h };

    SDL_Rect titleRect = { 0, 0, 800, 600};
    SDL_Surface* titleBg = IMG_Load("assets/title_bg.png");
    if (titleBg == NULL) throw std::runtime_error("Error loading title background");
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleBg);
    if (titleTexture == NULL) throw std::runtime_error("Error loading title texture");
    SDL_FreeSurface(titleBg);

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
            if (event.type == SDL_WINDOWEVENT_RESIZED) {
                int w, h;
                SDL_GetWindowSize(window, &w, &h);
                windowRenderMultiplierX = static_cast<float>(w) / 800.0f;
                windowRenderMultiplierY = static_cast<float>(h) / 600.0f;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect titleRenderRect = TranslateRect(titleRect);  // Apply scaling here
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRenderRect);

        // Render the hint with the translated coordinates
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect hintRenderRect = TranslateRect(hintRect);
        SDL_Rect hintBgRenderRect = { hintRenderRect.x - 10, hintRenderRect.y - 10, hintRenderRect.w + 20, hintRenderRect.h + 20 };
        SDL_RenderFillRect(renderer, &hintBgRenderRect);
        SDL_RenderCopy(renderer, hintTexture, NULL, &hintRenderRect);
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(hintTexture);
    TTF_CloseFont(hintFont);
  }

  SDL_Texture* hpIconTexture, *thirstIconTexture, *energyIconTexture;
  void PreloadStatusBarIcons() {
    SDL_Surface* hpIconSurface = IMG_Load("assets/hp_icon_32.png");
    SDL_Surface* thirstIconSurface = IMG_Load("assets/thirst_icon_32.png");
    SDL_Surface* energyIconSurface = IMG_Load("assets/energy_icon_32.png");
    hpIconTexture = SDL_CreateTextureFromSurface(renderer, hpIconSurface);
    thirstIconTexture = SDL_CreateTextureFromSurface(renderer, thirstIconSurface);
    energyIconTexture = SDL_CreateTextureFromSurface(renderer, energyIconSurface);
    SDL_FreeSurface(hpIconSurface);
    SDL_FreeSurface(thirstIconSurface);
    SDL_FreeSurface(energyIconSurface);
  }

  void game() {
    SDL_SetWindowTitle(window, "Holes - Game");
    PreloadHoleTexture();
    PreloadStatusBarIcons();
    SDL_Surface* playerSprite = IMG_Load("assets/player.png");
    if (playerSprite == NULL) throw std::runtime_error("Error loading player sprite");
    player.playerSprite = SDL_CreateTextureFromSurface(renderer, playerSprite);

    SDL_Texture* mapTexture_Part_Hill = IMG_LoadTexture(renderer, "assets/map_part_hill.png");
    SDL_Rect mapRect = { 0, 0, 3200, 2400 };

    TTF_Font* statusBarFont = TTF_OpenFont("assets/Inter.ttf", 16);
    if (statusBarFont == NULL) throw std::runtime_error("Error loading status bar font");

    bool player_Up = false, player_Down = false, player_Left = false, player_Right = false;

    player.health = 100;
    player.thirst = 100;
    player.energy = 100;
    Shovel* shovel = new Shovel();
    player.inventory.push_back(shovel);
    Bottle* bottle = new Bottle();
    player.inventory.push_back(bottle);
    Food* food = new Food();
    player.inventory.push_back(food);
    int motionBlurFrame = 0;

    while (state == APP_STATE_GAME) {
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          state = APP_STATE_QUIT;
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

      if (player_Up) 
      {
        player.y -= player.moveSpeed; 
      }
      if (player_Down)
      {
        player.y += player.moveSpeed;
      }
      if (player_Left) 
      {
        player.x -= player.moveSpeed;
      }
      if (player_Right)
      {
        player.x += player.moveSpeed;
      }

      if (player_Up || player_Down || player_Left || player_Right) {
        player.energy -= 0.05;
      }

      if (player.thirst < 0) player.health -= 0.05;
      else player.thirst -= 0.075;
      if (player.energy < 0) player.health -= 0.1;
      
      if (player.thirst > 90 && player.energy > 90 && player.health < 100) {
        player.health += 0.05;
      }
#ifdef WIN32
      if (motionBlurFrame == 0) {
        motionBlurFrame = 5;
#endif
        SDL_SetRenderDrawColor(renderer, 224, 172, 105, 255);
        SDL_RenderClear(renderer);
#ifdef WIN32
      }
      else {
        motionBlurFrame--;
      }
#endif
      
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

      if (player.x < 0) player.x = 0;
      if (player.y < 0) player.y = 0;
      if (player.x > 800 - 64) player.x = 800 - 64;
      if (player.y > 600 - 64) player.y = 600 - 64;
      if (player_Up) mapRect.y += player.moveSpeed; 
      if (player_Down) mapRect.y -= player.moveSpeed;
      if (player_Left) mapRect.x += player.moveSpeed;
      if (player_Right) mapRect.x -= player.moveSpeed;
      SDL_RenderCopy(renderer, mapTexture_Part_Hill, NULL, &mapRect);

      for (Hole* hole : holesVec) {
        if (player_Up) hole->holeRect.y += player.moveSpeed;
        if (player_Down) hole->holeRect.y -= player.moveSpeed;
        if (player_Left) hole->holeRect.x += player.moveSpeed;
        if (player_Right) hole->holeRect.x -= player.moveSpeed;
        SDL_Rect holeRect = { hole->holeRect.x, hole->holeRect.y, 64, 64 };
        if (SDL_RenderCopy(renderer, hole_unifiedTexture, NULL, &holeRect) != 0) {
          throw std::runtime_error("Failed to render hole");
        }
      }

      // Render player
      SDL_Rect playerRect = { 384, 284, 64, 64 };
      SDL_RenderCopy(renderer, player.playerSprite, NULL, &playerRect);

      // Render player item
      if (!player.inventory.empty()) {
        SDL_Rect itemRect = { 400, 300, 50, 50 };
        if (player.inventory[player.currentItem]->sprite == nullptr) {
          SDL_Surface* itemSurface = IMG_Load(player.inventory[player.currentItem]->itemSpritePath.c_str());
          //if (itemSurface == NULL) throw std::runtime_error("Failed to render item surface");
          player.inventory[player.currentItem]->sprite = SDL_CreateTextureFromSurface(renderer, itemSurface);
          SDL_FreeSurface(itemSurface);
        }
        SDL_RenderCopy(renderer, player.inventory[player.currentItem]->sprite, NULL, &itemRect);
      }

      if (shovelDiggingChargeProgress != 0) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect chargeRectBg = { 400 - 50 + 16, 300 - 50, 100, 10 };
        SDL_RenderFillRect(renderer, &chargeRectBg);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect chargeRect = { chargeRectBg.x, chargeRectBg.y, shovelDiggingChargeProgress, chargeRectBg.h };
        SDL_RenderFillRect(renderer, &chargeRect);
      }

      // Render HP (red)
      SDL_Rect hpIconRect = { 20, 600 - 32 * 3, 32, 32 };
      SDL_RenderCopy(renderer, hpIconTexture, NULL, &hpIconRect);
      SDL_Texture* hpTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended_Wrapped(statusBarFont, ("Health: " + std::to_string(static_cast<int>(player.health))).c_str(), {255, 50, 50, 255}, 0));
      SDL_Rect hpRect = { 57, hpIconRect.y, 32, 32 };
      SDL_QueryTexture(hpTexture, NULL, NULL, &hpRect.w, &hpRect.h);
      SDL_RenderCopy(renderer, hpTexture, NULL, &hpRect);
      SDL_DestroyTexture(hpTexture);

      // Render Energy (green)
      SDL_Rect energyIconRect = { 20, 600 - 32 * 2, 32, 32 };
      SDL_RenderCopy(renderer, energyIconTexture, NULL, &energyIconRect);
      SDL_Texture* energyTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended_Wrapped(statusBarFont, ("Energy: " + std::to_string(static_cast<int>(player.energy))).c_str(), {50, 255, 150, 255}, 0));
      SDL_Rect energyRect = { 57, energyIconRect.y, 32, 32 };
      SDL_QueryTexture(energyTexture, NULL, NULL, &energyRect.w, &energyRect.h);
      SDL_RenderCopy(renderer, energyTexture, NULL, &energyRect);
      SDL_DestroyTexture(energyTexture);

      // Render Thirst (blue)
      SDL_Rect thirstIconRect = { 20, 600 - 32, 32, 32 };
      SDL_RenderCopy(renderer, thirstIconTexture, NULL, &thirstIconRect);
      
      SDL_Texture* thirstTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended_Wrapped(statusBarFont, ("Thirst: " + std::to_string(static_cast<int>(player.thirst))).c_str(), {50, 50, 255, 255}, 0));
      SDL_Rect thirstRect = { 57, thirstIconRect.y, 32, 32 };
      SDL_QueryTexture(thirstTexture, NULL, NULL, &thirstRect.w, &thirstRect.h);
      SDL_RenderCopy(renderer, thirstTexture, NULL, &thirstRect);
      SDL_DestroyTexture(thirstTexture);

      // Render inventory
      int xOffset = 0;
      for (int i = player.inventory.size() - 1; i >= 0; i--) {
        SDL_Rect itemRect = { (800 - 50 * static_cast<int>(player.inventory.size() - 1))/2 + xOffset, 600 - 32, 32, 32 };
        if (player.inventory[i]->sprite == nullptr) {
          player.inventory[i]->sprite = IMG_LoadTexture(renderer, player.inventory[i]->itemSpritePath.c_str());
        }
        if (player.currentItem == i) {
          itemRect.w = 50;
          itemRect.h = 50;
          itemRect.y = 600 - 50;
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
          SDL_RenderFillRect(renderer, &itemRect);
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          SDL_RenderDrawRect(renderer, &itemRect);

          // Render item description
          SDL_Texture* itemDescTexture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended_Wrapped(statusBarFont, player.inventory[i]->itemName.c_str(), {255, 255, 255, 255}, 0));
          SDL_Rect itemDescRect = { 400 - 50 + 10, 600 - 50 - 32, 100, 32 };
          SDL_QueryTexture(itemDescTexture, NULL, NULL, &itemDescRect.w, &itemDescRect.h);
          itemDescRect.x = itemRect.x + (itemRect.w - itemDescRect.w) / 2;

          SDL_RenderCopy(renderer, itemDescTexture, NULL, &itemDescRect);
          SDL_DestroyTexture(itemDescTexture);
        }
        SDL_RenderCopy(renderer, player.inventory[i]->sprite, NULL, &itemRect);
        xOffset -= 50;
      }

      // Red tint if hp is low
      if (player.health <= 30) {
        Uint8 redAlpha = static_cast<Uint8>(255 * ((30 - player.health) / 30.0));
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, redAlpha);
        SDL_RenderFillRect(renderer, NULL);
      }
      
      // Gradually fade to black
      if (player.health <= 15) {
        Uint8 blackAlpha = static_cast<Uint8>(255 * (1 - (player.health / 15.0)));
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, blackAlpha);
        SDL_RenderFillRect(renderer, NULL);
      }
      
      // Transition to main menu if health is too low
      if (player.health <= 0) {
        state = APP_STATE_MAIN_MENU;
      }

      SDL_RenderPresent(renderer);
      SDL_Delay(1000/60);
    }
  }

  void Run() {
    window = SDL_CreateWindow("Holes", 0, 25, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
