#include <items.hpp>

#include <config.h>

#include <hole.hpp>
#include <base.hpp>

bool func_button_pressed = false, talk_button_pressed = false;

Item::~Item() {
  if (sprite != nullptr) {
    SDL_DestroyTexture(sprite);
  }
}

float shovelDiggingChargeProgress = 0;
Shovel::Shovel() {
  itemName = SHOVEL_ITEM_NAME;
  itemSpritePath = SHOVEL_ITEM_SPRITE_PATH;
  itemDescription = SHOVEL_ITEM_DESCRIPTION;
}

void Shovel::charge() {
  while (func_button_pressed && shovelDiggingChargeProgress <= 100) {
    if (player.energy > 0) shovelDiggingChargeProgress++;
    else shovelDiggingChargeProgress += 0.5;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  if (shovelDiggingChargeProgress >= 100) {
    Hole* hole = new Hole();
    hole->holeRect.x = 400;
    hole->holeRect.y = 300 + 30;
    holesVec.push_back(hole);
    player.energy -= 10;
    player.thirst -= 10;
    if (player.thirst < 0) player.thirst = 0;
    if (player.energy < 0) player.energy = 0;
  }
  shovelDiggingChargeProgress = 0;
}

void Shovel::func() {
  if (shovelDiggingChargeProgress == 0) {
    if (holdThread.joinable()) holdThread.join();
    holdThread = std::thread(&Shovel::charge, this);
  }
}

Shovel::~Shovel() {
  if (holdThread.joinable()) {
    holdThread.join();
  }
}

Bottle::Bottle() {
  itemName = BOTTLE_ITEM_NAME;
  itemSpritePath = BOTTLE_ITEM_SPRITE_PATH;
  itemDescription = std::to_string(capacity) + "%";
}

void Bottle::func() {
  if (capacity > 0) {
    if (player.thirst >= 99) return;
    player.thirst += 10;
    if (player.thirst > 100) player.thirst = 100;
    capacity -= 20;
    itemDescription = std::to_string(capacity) + "%";
  } else {
    player.inventory.erase(player.inventory.begin() + player.currentItem);
  }
}

Food::Food() {
  itemName = FOOD_ITEM_NAME;
  itemSpritePath = FOOD_ITEM_SPRITE_PATH;
  itemDescription = std::to_string(capacity) + "%";
}

void Food::func() {
  if (capacity > 0) {
    if (player.energy >= 100) return;
    player.energy += 10;
    if (player.energy > 100) player.energy = 100;
    capacity -= 20;
    itemDescription = std::to_string(capacity) + "%";
  } else {
    player.inventory.erase(player.inventory.begin() + player.currentItem);
  }
}
