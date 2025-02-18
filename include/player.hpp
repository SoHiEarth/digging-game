#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>
#include <items.hpp>
#include <assetbundleloader.hpp>

struct vec2 {
  int x, y;
  vec2(int x, int y) : x(x), y(y) {}
  SDL_Point toSDLPoint() {
    return {x, y};
  }
  SDL_Point* toSDLPointPtr() {
    return new SDL_Point{x, y};
  }
};

class Player {
  public:
    vec2 position = vec2(384, 284);
    float health = 100, energy = 100, thirst = 100;
    SDL_Texture* playerSprite = nullptr;
    std::vector<Item*> inventory;
    int currentItem = 0;
    int move_speed = current_asset_bundle.PLAYER_MOVE_SPEED;
};

#endif // PLAYER_H
