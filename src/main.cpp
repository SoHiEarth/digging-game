#include <SDL_events.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <config.h>
#include <items.hpp>
#include <interactable.hpp>
#include <base.hpp>
#include <hole.hpp>
#include <player.hpp>
#include <renderer_temp.hpp>
#include <application.hpp>

#ifdef WIN32
#include <windows.h>
void errorWindow(const char* message) {
  MessageBox(NULL, message, "Error", MB_ICONERROR | MB_OK);
}

#else
void errorWindow(const char* message) {
  std::cout << "[Error] " << message << "\n";
}
#endif

Application* app;
int main() {

  std::cout << "Allocating application\n";

  app = new Application();

  try {
    std::cout << "Initializing application\n";
    app->Init();
    std::cout << "Running application\n";
    app->Run();
    std::cout << "Quitting application\n";
    app->Quit();
  }
  catch (std::runtime_error& e) {
    exception_thrown_thread_pause = true;
    std::string msg = std::string(e.what()) + "\nSDL: " + SDL_GetError();
    errorWindow(msg.c_str());
    exception_thrown_thread_pause = false;
    delete app;
    return -1;
  }

  return 0;
}
