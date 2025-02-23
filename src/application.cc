#include <application.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdexcept>
#include <iostream>
#include "safe_thread.h"
#include <assetbundleloader.h>
#include <base.h>
#include <resload.h>
#include <error.h>
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
  window = SDL_CreateWindow(current_asset_bundle.APP_TITLE.c_str(), 0, 25, 800, 600, SDL_WINDOW_SHOWN);
  if (window == NULL) throw std::runtime_error("Error creating window");
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) throw std::runtime_error("Error creating renderer");
  ResLoad::SetRenderer(renderer);
  current_asset_bundle.LoadAssetBundle("./assets/assetbundle.ab");
  state = APP_STATE_MAIN_MENU;
  running = true;
}

void Application::Run() {
  while (running) {
    std::cout << "Entering application state " << state << "\n";
    switch (state) {
      case APP_STATE_MAIN_MENU:
        MainMenu();
        break;
      case APP_STATE_GAME:
        Game();
        break;
      case APP_STATE_DIALOUGE:
        Dialouge();
        break;
      case APP_STATE_QUIT:
        running = false;
        break;
      case APP_STATE_GAME_OVER:
        GameOver();
        break;
      default:
        throw std::runtime_error("Unknown Application State encountered!");
        break;
    }
  }
  level.Unload();
}

void Application::Quit() {
  CloseAllThreads();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
  }
