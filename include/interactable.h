#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <SDL.h>
#include <string>
#include <config.h>
#include <base.h>
#include <assetbundleloader.h>
#include <animate.h>
struct MapInteractable : public Object{
  public:
    std::string name = current_asset_bundle.INTERACTABLE_NULL_NAME;
    SDL_Texture* texture;
    std::string texture_path = current_asset_bundle.INTERACTABLE_NULL_SPRITE_PATH;
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
