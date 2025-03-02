#include <items.h>
#include <config.h>
#include <hole.h>
#include <base.h>
#include <algorithm>
#include <resload.h>
#include <level.h>
#include <renderer_temp.h>
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
      SDL_Point player_center = {player->rect.x + player->rect.w / 2, player->rect.y + player->rect.h / 2};
      if (SDL_PointInRect(&player_center, &hole->rect) == SDL_TRUE) {
        current_hole = hole;
        break;
      }
    }
  }
  if (current_hole != nullptr) {
    if (std::clamp(current_hole->progress, 0, 100) == current_hole->progress) {
      current_hole->progress += current_hole->speed;
      SDL_Texture* progress_texture = ResLoad::RenderText(inventoryFont, std::to_string(current_hole->progress) + "%", {255, 255, 255});
      SDL_Rect progress_rect;
      SDL_QueryTexture(progress_texture, nullptr, nullptr, &progress_rect.w, &progress_rect.h);
      progress_rect.x = current_hole->rect.x + current_hole->rect.w + 10;
      progress_rect.y = current_hole->rect.y + 10;
      current_hole->extras.insert({progress_texture, progress_rect});
    }
    return;
  } else {
    level.AddObjectInLoop(new Hole());
  }
}

Bottle::Bottle() {
  itemName = current_asset_bundle.BOTTLE_ITEM_NAME;
  itemSpritePath = current_asset_bundle.BOTTLE_ITEM_SPRITE_PATH;
  itemDescription = std::to_string(capacity) + "%";
}

void Bottle::func() {
  if (capacity > 0) {
    if (player->thirst >= 99) return;
    player->thirst += 10;
    if (player->thirst > 100) player->thirst = 100;
    capacity -= 20;
    itemDescription = std::to_string(capacity) + "%";
  } else {
    player->inventory.erase(player->inventory.begin() + player->current_item);
  }
}

Food::Food() {
  itemName = current_asset_bundle.FOOD_ITEM_NAME;
  itemSpritePath = current_asset_bundle.FOOD_ITEM_SPRITE_PATH;
  itemDescription = std::to_string(capacity) + "%";
}

void Food::func() {
  if (capacity > 0) {
    if (player->energy >= 100) return;
    player->energy += 10;
    if (player->energy > 100) player->energy = 100;
    capacity -= 20;
    itemDescription = std::to_string(capacity) + "%";
  } else {
    player->inventory.erase(player->inventory.begin() + player->current_item);
  }
}
