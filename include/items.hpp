#ifndef ITEMS_H
#define ITEMS_H

#include <SDL.h>
#include <string>
#include <stdexcept>
#include <assetbundleloader.hpp>

extern bool func_button_pressed, talk_button_pressed;

struct Item {
  std::string itemName = assetBundle.ITEM_NULL_NAME;
  std::string itemDescription = assetBundle.ITEM_NULL_DESCRIPTION;
  SDL_Texture* sprite = nullptr;
  std::string itemSpritePath = assetBundle.ITEM_NULL_SPRITE_PATH;
  virtual void func() { throw std::runtime_error("Item function not configured"); }
  virtual ~Item();
};

struct Shovel : public Item {
  Shovel();
  void func() override;
};

struct Bottle : public Item {
    int capacity = 100;
   
    Bottle();
    void func() override;
};

struct Food : public Item {
    int capacity = 100;
    
    Food();
    void func() override;
};

#endif // ITEMS_H
