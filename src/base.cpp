#include <base.hpp>
#include <character.hpp>
#include <exception>
#include <player.hpp>
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
std::thread renderThread, gameThread;
bool running = false;
bool exception_thrown_thread_pause = false;
Humanoid* currentHumanoid = nullptr;
Player player;
