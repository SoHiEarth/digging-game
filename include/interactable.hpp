#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <SDL.h>
#include <string>
#include <config.h>
#include <base.hpp>
#include <assetbundleloader.hpp>

struct MapInteractable : public Object{
  public:
    std::string interactableName = current_asset_bundle.INTERACTABLE_NULL_NAME;
    SDL_Texture* texture;
    std::string texturePath = current_asset_bundle.INTERACTABLE_NULL_SPRITE_PATH;
    void Update() override;
    virtual void func();
};

struct WaterRefillStation : public MapInteractable {
 public:
  WaterRefillStation();
  void func() override;
};

#endif // INTERACTABLE_H
