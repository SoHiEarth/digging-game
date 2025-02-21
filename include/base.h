#ifndef BASE_H
#define BASE_H

#include <SDL.h>
#include <player.h>
#include <humanoid.h>
#include <thread>
#include <map>
#include <level.h>
#include <application.h>
extern std::map<std::string, std::string> launch_args;
extern std::map<SDL_Keycode, bool> key_states;
extern Application* app;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event event;
extern std::thread fixed_thread;
extern bool running;
extern int global_brightness;
extern Humanoid* current_humanoid;
extern Player player;
extern Level level;

#endif // BASE_H
