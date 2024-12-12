#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>
#include <items.hpp>
#include <assetbundleloader.hpp>

class Player {
  public:
    int x = 384, y = 284;
    float health = 100, energy = 100, thirst = 100;
    SDL_Texture* playerSprite = nullptr;
    std::vector<Item*> inventory;
    int currentItem = 0;
    int moveSpeed = assetBundle.PLAYER_MOVE_SPEED;
};

#endif // PLAYER_H
