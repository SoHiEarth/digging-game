#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <macro.h>
#include <renderer_temp.h>
#include <application.h>
#include <base.h>

#ifdef WIN32
#include <windows.h>
void errorWindow(const char* message) {
  MessageBox(NULL, message, "Error", MB_ICONERROR | MB_OK);
}

#else
void errorWindow(const char* message) {
  std::cout << "[MAIN] [Error] " << message << "\n";
}
#endif

Application* app;
int main(int argc, char* argv[]) {

  std::cout << "Allocating application\n";

  app = new Application();

  std::cout << "Parsing command line arguments\n";
  
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg[0] == '-') {
      if (i + 1 < argc) {
        std::string value = argv[i + 1];
        launch_args[arg] = value;
        ++i;
      } else {
      std::cerr << "Error: Parameter " << arg << " is missing a value.\n";
      }
    }
  }

  std::cout << "Command launched with following parameters:\n";
  for (const auto& param : launch_args) {
    std::cout << "\t" << param.first << " " << param.second << "\n";
  }

  if (launch_args.empty()) std::cout << "\tNone\n";

  try {
    std::cout << "Initializing application\n";
    app->Init();
    std::cout << "Running application\n";
    app->Run();
    std::cout << "Quitting application\n";
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
