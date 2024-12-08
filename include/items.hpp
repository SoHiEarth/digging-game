#ifndef ITEMS_H
#define ITEMS_H

#include <SDL.h>
#include <string>
#include <stdexcept>
#include <config.h>
#include <thread>

extern bool func_button_pressed;
extern float shovelDiggingChargeProgress;

struct Item {
  std::string itemName = ITEM_NULL_NAME;
  std::string itemDescription = ITEM_NULL_DESCRIPTION;
  SDL_Texture* sprite = nullptr;
  std::string itemSpritePath = ITEM_NULL_SPRITE_PATH;
  virtual void func() { throw std::runtime_error("Item function not configured"); }
  virtual ~Item();
};

struct Shovel : public Item {
    std::thread holdThread;
    
    Shovel();
    void charge();
    void func() override;
    ~Shovel();
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
