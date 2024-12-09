#ifndef BASE_H
#define BASE_H

#include <SDL.h>
#include <player.hpp>
#include <character.hpp>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event event;
extern std::thread renderThread;
extern std::thread gameThread;
extern bool running;
extern bool exception_thrown_thread_pause;
extern Humanoid* currentHumanoid;
extern Player player;

#endif // BASE_H
