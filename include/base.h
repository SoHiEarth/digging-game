#ifndef BASE_H
#define BASE_H

#include <SDL.h>
#include <player.h>
#include <humanoid.h>
#include <safe_thread.h>
#include <map>
#include <level.h>
#include <application.h>
enum THREAD_STATE {
  THREAD_STATE_OPEN,
  THREAD_STATE_TERMINATE
};
extern std::map<std::string, std::string> launch_args;
extern std::map<SDL_Keycode, bool> key_states;
extern Application* app;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event event;
extern bool running;
extern int global_brightness;
extern Humanoid* current_humanoid;
extern Player player;
extern Level level;
extern THREAD_STATE thread_state;
#endif // BASE_H
