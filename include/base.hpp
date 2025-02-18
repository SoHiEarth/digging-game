#ifndef BASE_H
#define BASE_H

#include <SDL.h>
#include <player.hpp>
#include <humanoid.h>
#include <thread>
#include <mutex>
#include <map>
#include <level.h>
#include <application.hpp>
extern std::map<std::string, std::string> launchParams;
extern std::map<SDL_Keycode, bool> key_states;
extern Application* app;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event event;
extern std::thread renderThread;
extern std::thread gameThread;
extern bool running;
extern bool exception_thrown_thread_pause;
extern int globalBrightness;
extern std::mutex humanoidsMutex;
extern std::vector<Humanoid*> humanoidsVec;
extern Humanoid* currentHumanoid;
extern Player player;
extern Level level;

#endif // BASE_H
