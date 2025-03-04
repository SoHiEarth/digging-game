#ifndef ITEMS_H
#define ITEMS_H

#include <SDL.h>
#include <string>
#include <stdexcept>
#include "prompt.h"
#include <assetbundleloader.h>
extern bool func_button_pressed, talk_button_pressed;

struct Item {
  std::string itemName = current_asset_bundle.ITEM_NULL_NAME;
  std::string itemDescription = current_asset_bundle.ITEM_NULL_DESCRIPTION;
  SDL_Texture* sprite = nullptr;
  std::string itemSpritePath = current_asset_bundle.ITEM_NULL_SPRITE_PATH;
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

////////////////////////////////////////
/// MISC ITEMS
struct Pendant : public Item {
  Pendant() { itemName = "Pendant"; itemDescription = "???"; itemSpritePath = "assets/pendant.png"; }
  void func() override { Holes::PromptPlayerWithIconBox("A mysterious pendant... what could it be?", sprite);}
};

struct StoryItem : public Item {
  StoryItem();
  void func() override;
};

struct StoryItem_2 : public Item {
  StoryItem_2();
  void func() override;
};

struct Story_Intro : public StoryItem {
  Story_Intro() {
    itemName = "Story - Intro";
    itemDescription = "???";
    itemSpritePath = "assets/story_intro.png";
  }
};

struct Story_KB : public StoryItem_2 {
  Story_KB() {
    itemName = "Story - Kate Barlow";
    itemDescription = "???";
    itemSpritePath = "assets/story_kb.png";
  }
};

struct Story_EY : public StoryItem_2 {
  Story_EY() {
    itemName = "Story - Elya Yelnats";
    itemDescription = "???";
    itemSpritePath = "assets/story_ey.png";
  }
};

#endif // ITEMS_H
