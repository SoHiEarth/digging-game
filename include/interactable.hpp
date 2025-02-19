#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <SDL.h>
#include <string>
#include <config.h>
#include <base.hpp>
#include <assetbundleloader.hpp>
#include <animate.hpp>
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

struct TransportBus_Lv1 : public Object {
 public:
  SDL_Texture* texture;
  Animator_Rect* animator_transform;
  Animator_Brightness* animator_brightness;
  TransportBus_Lv1();
  void Start() override;
  void Update() override;
  void Quit() override;
};

#endif // INTERACTABLE_H
