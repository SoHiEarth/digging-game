#include <SDL_render.h>
#include <SDL_video.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <string>

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

class Player {
  public:
    int x, y;
    int health, energy, thirst;
    SDL_Texture* playerSprite;
    int moveSpeed = 5;
};

class Application {
  /* [APP VARIABLES] */
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Event event;
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
    TTF_Font* hintFont = TTF_OpenFont("assets/Inter.ttf", 20);
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


  void game() {
    SDL_SetWindowTitle(window, "Holes - Game");
    
    SDL_Surface* playerSprite = IMG_Load("assets/player.png");
    if (playerSprite == NULL) throw std::runtime_error("Error loading player sprite");
    player.playerSprite = SDL_CreateTextureFromSurface(renderer, playerSprite);

    TTF_Font* statusBarFont = TTF_OpenFont("assets/Inter.ttf", 16);
    if (statusBarFont == NULL) throw std::runtime_error("Error loading status bar font");

    bool player_Up = false, player_Down = false, player_Left = false, player_Right = false;

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
          }
        }
        if (event.type == SDL_WINDOWEVENT_RESIZED) {
          int w, h;
          SDL_GetWindowSize(window, &w, &h);
          windowRenderMultiplierX = w / 800;
          windowRenderMultiplierY = h / 600;
        }
      }

      if (player_Up) player.y -= player.moveSpeed; 
      if (player_Down) player.y += player.moveSpeed;
      if (player_Left) player.x -= player.moveSpeed;
      if (player_Right) player.x += player.moveSpeed;

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      // Render player
      SDL_Rect playerRect = { player.x, player.y, 32, 32 };
      SDL_RenderCopy(renderer, player.playerSprite, NULL, &playerRect);

      // Render hud (status bar)
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
      SDL_Rect hudRect = { 0, 600 - 32, 400, 32 };
      int statusBorderWidth = 2;
      SDL_RenderDrawLine(renderer, 0, (hudRect.y - statusBorderWidth) * windowRenderMultiplierY, hudRect.w * windowRenderMultiplierX, (hudRect.y - statusBorderWidth) * windowRenderMultiplierY);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
      SDL_RenderFillRect(renderer, &hudRect);

      // Render HP (red)
      SDL_Surface* hpSurface = TTF_RenderText_Blended_Wrapped(statusBarFont, ("HP: " + std::to_string(player.health)).c_str(), {255, 50, 50, 255}, 0);
      SDL_Texture* hpTexture = SDL_CreateTextureFromSurface(renderer, hpSurface);
      SDL_Rect hpRect = { 10, hudRect.y + 8, hpSurface->w, hpSurface->h };
      SDL_RenderCopy(renderer, hpTexture, NULL, &hpRect);
      SDL_FreeSurface(hpSurface);

      // Render Energy (green)
      SDL_Surface* energySurface = TTF_RenderText_Blended_Wrapped(statusBarFont, ("Energy: " + std::to_string(player.energy)).c_str(), {50, 255, 150, 255}, 0);
      SDL_Texture* energyTexture = SDL_CreateTextureFromSurface(renderer, energySurface);
      SDL_Rect energyRect = { 60, hudRect.y + 8, energySurface->w, energySurface->h };
      SDL_RenderCopy(renderer, energyTexture, NULL, &energyRect);
      SDL_FreeSurface(energySurface);

      // Render Thirst (blue)
      SDL_Surface* thirstSurface = TTF_RenderText_Blended_Wrapped(statusBarFont, ("Thirst: " + std::to_string(player.thirst)).c_str(), {50, 50, 255, 255}, 0);
      SDL_Texture* thirstTexture = SDL_CreateTextureFromSurface(renderer, thirstSurface);
      SDL_Rect thirstRect = { 140, hudRect.y + 8, thirstSurface->w, thirstSurface->h };
      SDL_RenderCopy(renderer, thirstTexture, NULL, &thirstRect);
      SDL_FreeSurface(thirstSurface);

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
