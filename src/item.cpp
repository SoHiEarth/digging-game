#include <items.hpp>
#include <config.h>
#include <hole.hpp>
#include <base.hpp>
#include <algorithm>
#include <iostream>
bool func_button_pressed = false, talk_button_pressed = false;

Item::~Item() {
  if (sprite != nullptr) {
    SDL_DestroyTexture(sprite);
  }
}

Shovel::Shovel() {
  itemName = assetBundle.SHOVEL_ITEM_NAME;
  itemSpritePath = assetBundle.SHOVEL_ITEM_SPRITE_PATH;
  itemDescription = assetBundle.SHOVEL_ITEM_DESCRIPTION;
}

void Shovel::func() {
  Hole* current_hole = nullptr;
  SDL_Rect player_rect = {
    player.x,
    player.y,
    64,
    64
  };
  std::cout << "Cpt 1";
  if (!holesVec.empty()) {
    for (Hole* hole : holesVec) {
      SDL_Point top_left = {hole->holeRect.x,hole->holeRect.y},
        bottom_right = {hole->holeRect.x + 32};
      if (SDL_PointInRect(&top_left, &player_rect) == SDL_TRUE ||
        SDL_PointInRect(&bottom_right, &player_rect) == SDL_TRUE) {
        current_hole = hole;
        break;
      }
    }
  }
  std::cout << "2";
  if (current_hole != nullptr) {
    if (std::clamp(current_hole->hole_dig_progress, 0, 100) == current_hole->hole_dig_progress) {
      current_hole->hole_dig_progress++;
    }
    return;
  }
  std::cout << "3";
  Hole* hole = new Hole();
  hole->holeRect = {player_rect.x, player_rect.y, 32, 32};
  holesVec.push_back(hole);
  std::cout << "4";
}

Bottle::Bottle() {
  itemName = assetBundle.BOTTLE_ITEM_NAME;
  itemSpritePath = assetBundle.BOTTLE_ITEM_SPRITE_PATH;
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
  itemName = assetBundle.FOOD_ITEM_NAME;
  itemSpritePath = assetBundle.FOOD_ITEM_SPRITE_PATH;
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
