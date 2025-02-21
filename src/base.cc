#include <base.h>
#include <humanoid.h>
#include <player.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <mutex>
#include <vector>
#include <thread>
#include <map>
#include <assetbundleloader.h>
#include <level.h>
std::map<std::string, std::string> launch_args;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
std::thread renderThread, fixed_thread;
bool running = false;
bool exception_thrown_thread_pause = false;
int global_brightness = 100;
std::mutex humanoidsMutex;
std::vector<Humanoid*> humanoidsVec;
Humanoid* currentHumanoid = nullptr;
Player player;
Level level;
