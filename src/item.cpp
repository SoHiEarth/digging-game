#include <items.hpp>
#include <config.h>
#include <hole.hpp>
#include <base.hpp>
#include <algorithm>
bool func_button_pressed = false, talk_button_pressed = false;
Item::~Item() {
  if (sprite != nullptr) {
    SDL_DestroyTexture(sprite);
  }
}

Shovel::Shovel() {
  itemName = current_asset_bundle.SHOVEL_ITEM_NAME;
  itemSpritePath = current_asset_bundle.SHOVEL_ITEM_SPRITE_PATH;
  itemDescription = current_asset_bundle.SHOVEL_ITEM_DESCRIPTION;
}

void Shovel::func() {
  Hole* current_hole = nullptr;
  SDL_Rect player_rect = {
    player.position.x,
    player.position.y,
    64,
    64
  };
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
  if (current_hole != nullptr) {
    if (std::clamp(current_hole->hole_dig_progress, 0, 100) == current_hole->hole_dig_progress) {
      current_hole->hole_dig_progress++;
    }
    return;
  }
  Hole* hole = new Hole();
  hole->holeRect = {player_rect.x, player_rect.y, 32, 32};
  holesVec.push_back(hole);
}

Bottle::Bottle() {
  itemName = current_asset_bundle.BOTTLE_ITEM_NAME;
  itemSpritePath = current_asset_bundle.BOTTLE_ITEM_SPRITE_PATH;
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
  itemName = current_asset_bundle.FOOD_ITEM_NAME;
  itemSpritePath = current_asset_bundle.FOOD_ITEM_SPRITE_PATH;
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
