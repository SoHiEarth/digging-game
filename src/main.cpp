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
            std::cout << "Charging shovel" << shovelDiggingChargeProgress << "%\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        if (shovelDiggingChargeProgress >= 100) {
            Hole* hole = new Hole();
            hole->holeRect.x = 400 - 16;
            hole->holeRect.y = 300 + 16;
            holesVec.push_back(hole);
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
      itemName = "Bottle";
      itemSpritePath = "assets/char_item_1.png";
    }
    
    void func() override {
      player.thirst = 100;
    }
};

class Application {
  /* [APP VARIABLES] */
  bool running;
  Player player;
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

    if (Mix_Init(MIX_INIT_WAVPACK) != 0) {
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
  SDL_Rect hpIconRect, thirstIconRect, energyIconRect;
  void PreloadStatusBarIcons() {
    SDL_Surface* hpIconSurface = IMG_Load("assets/hp_icon_32.png");
    SDL_Surface* thirstIconSurface = IMG_Load("assets/thirst_icon_32.png");
    SDL_Surface* energyIconSurface = IMG_Load("assets/energy_icon_32.png");
    hpIconTexture = SDL_CreateTextureFromSurface(renderer, hpIconSurface);
    thirstIconTexture = SDL_CreateTextureFromSurface(renderer, thirstIconSurface);
    energyIconTexture = SDL_CreateTextureFromSurface(renderer, energyIconSurface);
  SDL_QueryTexture(hpIconTexture, NULL, NULL, &hpIconRect.w, &hpIconRect.h);
  SDL_QueryTexture(thirstIconTexture, NULL, NULL, &thirstIconRect.w, &thirstIconRect.h);
  SDL_QueryTexture(energyIconTexture, NULL, NULL, &energyIconRect.w, &energyIconRect.h);
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

    TTF_Font* statusBarFont = TTF_OpenFont("assets/Inter.ttf", 16);
    if (statusBarFont == NULL) throw std::runtime_error("Error loading status bar font");

    bool player_Up = false, player_Down = false, player_Left = false, player_Right = false;

    player.health = 100;
    player.thirst = 100;
    player.energy = 100;
    Shovel* shovel = new Shovel();
    player.inventory.push_back(shovel);

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
              try {
                player.inventory[player.currentItem]->func();
              }
              catch (...) {
                player.currentItem = player.inventory.size();
              }
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
        if (event.type == SDL_WINDOWEVENT_RESIZED) {
          int w, h;
          SDL_GetWindowSize(window, &w, &h);
          windowRenderMultiplierX = w / 800;
          windowRenderMultiplierY = h / 600;
        }
      }

      if (player_Up) 
      {
        player.y -= player.moveSpeed; 
        player.thirst -= 0.1;
        player.energy -= 0.05;
      }
      if (player_Down)
      {
        player.y += player.moveSpeed;
        player.thirst -= 0.1;
        player.energy -= 0.05;
      }
      if (player_Left) 
      {
        player.x -= player.moveSpeed;
        player.thirst -= 0.1;
        player.energy -= 0.05;
      }
      if (player_Right)
      {
        player.x += player.moveSpeed;
        player.thirst -= 0.1;
        player.energy -= 0.05;
      }

      if (player.thirst < 0) player.health -= 0.05;
      if (player.energy < 0) player.energy -= 0.1;

      SDL_SetRenderDrawColor(renderer, 224, 172, 105, 255);
      SDL_RenderClear(renderer);

      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

      // Render player itemName
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

      // Render hud (status bar)
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
      SDL_Rect hudRect = { 0, 600 - 32, 800, 32 };
      int statusBorderWidth = 2;
      SDL_RenderDrawLine(renderer, 0, (hudRect.y - statusBorderWidth) * windowRenderMultiplierY, hudRect.w * windowRenderMultiplierX, (hudRect.y - statusBorderWidth) * windowRenderMultiplierY);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
      SDL_RenderFillRect(renderer, &hudRect);

      // Render HP (red)
      int hpTextW, hpTextH;
      hpIconRect.x = 50; hpIconRect.y = 600 - 32; hpIconRect.w = 32; hpIconRect.h = 32;
      SDL_RenderCopy(renderer, hpIconTexture, NULL, &hpIconRect);
      SDL_Surface* hpSurface = TTF_RenderText_Blended_Wrapped(statusBarFont, ("HP: " + std::to_string(static_cast<int>(player.health))).c_str(), {255, 50, 50, 255}, 0);
      SDL_Texture* hpTexture = SDL_CreateTextureFromSurface(renderer, hpSurface);
      SDL_Rect hpRect = { hpIconRect.x + hpIconRect.w + 10, hudRect.y + 8, hpSurface->w, hpSurface->h };
      SDL_QueryTexture(hpTexture, NULL, NULL, &hpTextW, &hpTextH);
      SDL_RenderCopy(renderer, hpTexture, NULL, &hpRect);
      SDL_FreeSurface(hpSurface);
      SDL_DestroyTexture(hpTexture);

      const int hpHUDoffset = hpIconRect.x + hpIconRect.w + hpTextW; 
      // Render Energy (green)
      energyIconRect.x = hpHUDoffset + 50; energyIconRect.y = 600 - 32; energyIconRect.w = 32; energyIconRect.h = 32;
      SDL_RenderCopy(renderer, energyIconTexture, NULL, &energyIconRect);
      SDL_Surface* energySurface = TTF_RenderText_Blended_Wrapped(statusBarFont, ("Energy: " + std::to_string(static_cast<int>(player.energy))).c_str(), {50, 255, 150, 255}, 0);
      SDL_Texture* energyTexture = SDL_CreateTextureFromSurface(renderer, energySurface);
      SDL_Rect energyRect = { hpHUDoffset + 50 + energyIconRect.w + 10, hudRect.y + 8, energySurface->w, energySurface->h };
      SDL_RenderCopy(renderer, energyTexture, NULL, &energyRect);
      SDL_FreeSurface(energySurface);
      SDL_DestroyTexture(energyTexture);

      const int energyHUDoffset = hpHUDoffset + 50 + energyIconRect.w + 10 + energyRect.w;
      // Render Thirst (blue)
      thirstIconRect.x = energyHUDoffset + 50; thirstIconRect.y = 600 - 32; thirstIconRect.w = 32; thirstIconRect.h = 32;
      SDL_RenderCopy(renderer, thirstIconTexture, NULL, &thirstIconRect);
      SDL_Surface* thirstSurface = TTF_RenderText_Blended_Wrapped(statusBarFont, ("Thirst: " + std::to_string(static_cast<int>(player.thirst))).c_str(), {50, 50, 255, 255}, 0);
      SDL_Texture* thirstTexture = SDL_CreateTextureFromSurface(renderer, thirstSurface);
      SDL_Rect thirstRect = { energyHUDoffset + 50 + thirstIconRect.w + 10, hudRect.y + 8, thirstSurface->w, thirstSurface->h };
      SDL_RenderCopy(renderer, thirstTexture, NULL, &thirstRect);
      SDL_FreeSurface(thirstSurface);
      SDL_DestroyTexture(thirstTexture);

      // Red tint if hp is low
      if (player.health <= 50) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, static_cast<Uint8>(255 * ((100 - player.health) / 100)) - 100);
        SDL_RenderFillRect(renderer, NULL);
      }
      if (player.health <= 0) state = APP_STATE_MAIN_MENU;

      SDL_RenderPresent(renderer);
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
