#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <SDL.h>
#include <string>
#include <stdexcept>
#include <config.h>
#include <base.hpp>
#include <assetbundleloader.hpp>

struct MapInteractable {
  public:
    std::string interactableName = assetBundle.INTERACTABLE_NULL_NAME;
    SDL_Rect rect;
    SDL_Texture* texture;
    std::string texturePath = assetBundle.INTERACTABLE_NULL_SPRITE_PATH;
    virtual void func() { throw std::runtime_error("Interactable function not configured"); }
};

struct WaterRefillStation : public MapInteractable {
  public:
    WaterRefillStation() {
      interactableName = assetBundle.WATER_REFILL_STATION_NAME;
      texturePath = assetBundle.WATER_REFILL_STATION_SPRITE_PATH;
      rect = { 0, 0, 64, 128 }; 
    }

    void func() override {
      for (int i = 0; i < player.inventory.size(); i++) {
        if (player.inventory[i]->itemName == "Bottle") {
          player.inventory.erase(player.inventory.begin() + i);
          break;
        }
      }
      player.inventory.push_back(new Bottle());
    }
};

#endif // INTERACTABLE_H
