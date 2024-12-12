#include <base.hpp>
#include <character.hpp>
#include <player.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <mutex>
#include <vector>
#include <thread>
#include <map>
#include <assetbundleloader.hpp>

std::map<std::string, std::string> launchParams;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
std::thread renderThread, gameThread;
bool running = false;
bool exception_thrown_thread_pause = false;
std::mutex humanoidsMutex;
std::vector<Humanoid*> humanoidsVec;
Humanoid* currentHumanoid = nullptr;
Player player;
