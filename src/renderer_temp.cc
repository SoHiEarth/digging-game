#include <humanoid.h>
#include <renderer_temp.h>
#include <resload.h>
#include "SDL2_gfx/SDL2_gfxPrimitives.h"
SDL_Texture *hpIconTexture = nullptr, *thirstIconTexture = nullptr, *energyIconTexture = nullptr;
TTF_Font *widgetFont = nullptr, *inventoryFont = nullptr;
SDL_Texture* map_texture_Part_Hill = nullptr;
SDL_Rect mapRect = { 0, 0, 3200, 2400 };
bool player_up = false, player_down = false, player_left = false, player_right = false;
SDL_Rect chargeRectBg = { 400 - 50 + 16, 300 - 50, 100, 10 };

void LoadStatusBarIcons() {
  hpIconTexture = ResLoad::LoadImage(current_asset_bundle.PLAYERSTAT_HEALTH_ICON_PATH);
  thirstIconTexture = ResLoad::LoadImage(current_asset_bundle.PLAYERSTAT_THIRST_ICON_PATH);
  energyIconTexture = ResLoad::LoadImage(current_asset_bundle.PLAYERSTAT_ENERGY_ICON_PATH);
  widgetFont = ResLoad::LoadFont(current_asset_bundle.PLAYERSTAT_FONT_PATH, 30);
}

void LoadPlayerSprite() {
  if (player == nullptr) return;
  if (player->texture != nullptr) ResLoad::FreeImage(player->texture);
  player->texture = ResLoad::LoadImage(current_asset_bundle.PLAYER_SPRITE_PATH);
}

void ResetPlayerStats() {
  if (player == nullptr) return;
  player->rect.x = (window_width - 64) / 2;
  player->rect.y = (window_height - 64) / 2;
  player->health = 100;
  player->energy = 100;
  player->thirst = 100;
}

void PreloadMapTexture() {
  if (map_texture_Part_Hill != nullptr) return;
  map_texture_Part_Hill = ResLoad::LoadImage(current_asset_bundle.MAP_PART_HILL_PATH);
}

void RenderWidget(SDL_Rect anchor, SDL_Texture* icon, int val, int maxVal, SDL_Color theme){
  SDL_Rect iconRect, valRect, barRect;
  SDL_Texture* valTexture = nullptr;
  iconRect = { anchor.x + 5, anchor.y + 2, 32, 32 };
  if (std::to_string(val) == "") val = 0;
  valTexture = ResLoad::RenderText(widgetFont, std::to_string(val), theme);
  SDL_QueryTexture(valTexture, NULL, NULL, &valRect.w, &valRect.h);
  valRect.x = iconRect.x + iconRect.w + 5;
  valRect.y = iconRect.y + static_cast<int>((iconRect.h - valRect.h) * 0.5);
  barRect = { iconRect.x + 5, iconRect.y + iconRect.h - 1, static_cast<int>((anchor.w - 15) * (static_cast<float>(val) / maxVal)), 2 };

  roundedRectangleRGBA(renderer,
      static_cast<Sint16>(anchor.x),
      static_cast<Sint16>(anchor.y),
      static_cast<Sint16>(anchor.x + anchor.w),
      static_cast<Sint16>(anchor.y + anchor.h),
      10, theme.r, theme.g, theme.b, 120);
  SDL_RenderCopy(renderer, icon, NULL, &iconRect);
  SDL_RenderCopy(renderer, valTexture, NULL, &valRect);
  SDL_SetRenderDrawColor(renderer, theme.r, theme.g, theme.b, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &barRect);
  SDL_DestroyTexture(valTexture);
}

void RenderPlayerStats() {
  SDL_Rect playerStat_Background = { 20, window_height - 100 - 10, 200, 95 };
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
  roundedBoxRGBA(renderer,
      static_cast<Sint16>(playerStat_Background.x),
      static_cast<Sint16>(playerStat_Background.y),
      static_cast<Sint16>(playerStat_Background.x + playerStat_Background.w),
      static_cast<Sint16>(playerStat_Background.y + playerStat_Background.h),
      10, 0, 0, 0, 120);
  if (player == nullptr) return;

  SDL_Rect playerStat_HP_Anchor = { playerStat_Background.x + 5,
    playerStat_Background.y + 5,
    190, 40 };
  SDL_Rect playerStat_Energy_Anchor = { playerStat_Background.x + 5,
    playerStat_HP_Anchor.y + playerStat_HP_Anchor.h + 5,
    92, 40 };
  SDL_Rect playerStat_Thirst_Anchor = { playerStat_Background.x + 5 + playerStat_Energy_Anchor.w + 5,
    playerStat_Energy_Anchor.y,
    92, 40 };

  RenderWidget(playerStat_HP_Anchor, hpIconTexture, static_cast<int>(player->health), 100, {255, 0, 0, 255});
  RenderWidget(playerStat_Energy_Anchor, energyIconTexture, static_cast<int>(player->energy), 100, {0, 255, 0, 255});
  RenderWidget(playerStat_Thirst_Anchor, thirstIconTexture, static_cast<int>(player->thirst), 100, {150, 150, 255, 255});
}

Item* inv_prevItem = nullptr, *inv_currItem = nullptr, *inv_nextItem = nullptr, *prevItem = nullptr;
void RenderItem(Item* item, SDL_Rect anchor, int alpha) {
  SDL_Rect itemIconRect = { anchor.x + 5, anchor.y, 32, 32 };
  if (item->sprite == nullptr) {
    item->sprite = ResLoad::LoadImage(item->itemSpritePath);
  }
  if (item->itemName == "") {
    item->itemName = "???";
  }
  if (item->itemDescription == "") {
    item->itemDescription = "???";
  }
  TTF_SetFontStyle(inventoryFont, TTF_STYLE_BOLD);
  SDL_Texture* itemNameTexture = ResLoad::RenderText(inventoryFont, item->itemName, {255, 255, 255, static_cast<Uint8>(alpha)});
  TTF_SetFontStyle(inventoryFont, TTF_STYLE_NORMAL);
  SDL_Texture* itemDescTexture = ResLoad::RenderText(inventoryFont, item->itemDescription, {150, 150, 150, static_cast<Uint8>(alpha)});

  SDL_Rect itemNameRect, itemDescRect;
  SDL_QueryTexture(itemNameTexture, NULL, NULL, &itemNameRect.w, &itemNameRect.h);
  SDL_QueryTexture(itemDescTexture, NULL, NULL, &itemDescRect.w, &itemDescRect.h);

  itemNameRect = { itemIconRect.x + itemIconRect.w + 5, itemIconRect.y, itemNameRect.w, itemNameRect.h };
  itemDescRect = { itemIconRect.x + itemIconRect.w + itemNameRect.w + 10, itemIconRect.y, itemDescRect.w, itemDescRect.h };

  SDL_RenderCopy(renderer, item->sprite, NULL, &itemIconRect);
  SDL_RenderCopy(renderer, itemNameTexture, NULL, &itemNameRect);
  SDL_RenderCopy(renderer, itemDescTexture, NULL, &itemDescRect);

  SDL_DestroyTexture(itemNameTexture);
  SDL_DestroyTexture(itemDescTexture);
}

void RenderInventory() {
  SDL_Rect anchorRect = { window_width - 200 - 20, window_height - 100 - 10, 200, 95 };
  if (player->inventory.empty()) return;
  if (player->current_item - 1 >= 0) inv_prevItem = player->inventory[player->current_item - 1];
  else inv_prevItem = nullptr;
  inv_currItem = player->inventory[player->current_item];
  if (player->current_item + 1 < player->inventory.size()) inv_nextItem = player->inventory[player->current_item + 1];
  else inv_nextItem = nullptr;

  SDL_Rect prevItemRect = { anchorRect.x + 5, anchorRect.y, anchorRect.w, 30 };
  SDL_Rect currItemRect = { prevItemRect.x, anchorRect.y + 30, anchorRect.w, 30 };
  SDL_Rect nextItemRect = { prevItemRect.x, anchorRect.y + 60, anchorRect.w, 30 };

  if (inv_prevItem != nullptr) RenderItem(inv_prevItem, prevItemRect, 150);
  if (inv_nextItem != nullptr) RenderItem(inv_nextItem, nextItemRect, 150);
  roundedBoxRGBA(renderer,
      static_cast<Sint16>(anchorRect.x),
      static_cast<Sint16>(anchorRect.y),
      static_cast<Sint16>(anchorRect.x + anchorRect.w),
      static_cast<Sint16>(anchorRect.y + anchorRect.h),
      10, 0, 0, 0, 120);
  RenderItem(inv_currItem, currItemRect, 255);
}

void RenderHumanoid(Humanoid* humanoid) {
 if (humanoid->texture == nullptr) {
    humanoid->texture = ResLoad::LoadImage(humanoid->texture_path);
  }
  SDL_Texture* characterName = ResLoad::RenderText(inventoryFont, humanoid->name, {255, 255, 255, 255});
  SDL_Rect characterNameRect;
  SDL_QueryTexture(characterName, NULL, NULL, &characterNameRect.w, &characterNameRect.h);
  characterNameRect = { humanoid->rect.x - (humanoid->rect.w / 2), humanoid->rect.y + humanoid->rect.w, characterNameRect.w, characterNameRect.h };
  SDL_RenderCopy(renderer, characterName, NULL, &characterNameRect);
  SDL_RenderCopy(renderer, humanoid->texture, NULL, &humanoid->rect);
  SDL_DestroyTexture(characterName);
}
