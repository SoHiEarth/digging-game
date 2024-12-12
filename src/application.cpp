#include <application.hpp>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <stdexcept>
#include <iostream>
#include <assetbundleloader.hpp>
#include <base.hpp>

void Application::Init() {
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
  assetBundle.LoadAssetBundle("assets/assetbundle.ab");
  state = APP_STATE_MAIN_MENU;
  running = true;
}

void Application::Run() {
    window = SDL_CreateWindow(assetBundle.APP_TITLE.c_str(), 0, 25, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) throw std::runtime_error("Error creating window");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) throw std::runtime_error("Error creating renderer");

    while (running) {
      std::cout << "Entering application state " << state << "\n";
      switch (state) {
        case APP_STATE_MAIN_MENU:
          mainMenu();
          break;
        case APP_STATE_GAME:
          game();
          break;
        case APP_STATE_DIALOUGE:
          dialouge();
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

void Application::Quit() {
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
  }
