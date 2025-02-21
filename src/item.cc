#include <items.h>
#include <config.h>
#include <hole.h>
#include <base.h>
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
  if (!holes_vector.empty()) {
    for (Hole* hole : holes_vector) {
      if (hole == nullptr) continue;
      if (SDL_HasIntersection(&player.rect, &hole->rect) == SDL_TRUE) {
        current_hole = hole;
        break;
      }
    }
  }
  if (current_hole != nullptr) {
    if (std::clamp(current_hole->progress, 0, 100) == current_hole->progress) {
      current_hole->progress += current_hole->speed;
    }
    return;
  } else {
    Hole* hole = new Hole();
    hole->rect = player.rect;
    holes_vector.push_back(hole);
  }
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
    player.inventory.erase(player.inventory.begin() + player.current_item);
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
    player.inventory.erase(player.inventory.begin() + player.current_item);
  }
}
