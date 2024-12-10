#include <SDL_render.h>

#include <renderer_temp.hpp>
#include <character.hpp>

SDL_Texture *hpIconTexture = nullptr, *thirstIconTexture = nullptr, *energyIconTexture = nullptr;
TTF_Font *widgetFont = nullptr, *inventoryFont = nullptr;
SDL_Texture* mapTexture_Part_Hill = nullptr;
SDL_Rect mapRect = { 0, 0, 3200, 2400 };
bool player_Up = false, player_Down = false, player_Left = false, player_Right = false;
SDL_Rect playerRect = { 384, 284, 64, 64 }, itemRect = { 415, 300, 35, 35 };
SDL_Rect chargeRectBg = { 400 - 50 + 16, 300 - 50, 100, 10 };
 
WaterRefillStation* waterRefillStation = nullptr;

SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color color) {
  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text, color, 0);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

void PreloadStatusBarIcons() {
  hpIconTexture = IMG_LoadTexture(renderer, PLAYERSTAT_HEALTH_ICON_PATH);
  thirstIconTexture = IMG_LoadTexture(renderer, PLAYERSTAT_THIRST_ICON_PATH);
  energyIconTexture = IMG_LoadTexture(renderer, PLAYERSTAT_ENERGY_ICON_PATH);
  if (hpIconTexture == NULL || thirstIconTexture == NULL || energyIconTexture == NULL) throw std::runtime_error("Error loading status bar icons");
  widgetFont = TTF_OpenFont(PLAYERSTAT_FONT_PATH, 24);
  if (widgetFont == NULL) throw std::runtime_error("Error loading widget font");
}

void PreloadPlayerSprite() {
  if (player.playerSprite != nullptr) SDL_DestroyTexture(player.playerSprite);
  player.playerSprite = IMG_LoadTexture(renderer, PLAYER_SPRITE_PATH);
  if (player.playerSprite == NULL) throw std::runtime_error("Error loading player sprite");
}

void ResetPlayerStats() {
  player.x = 384;
  player.y = 284;
  player.health = 100;
  player.energy = 100;
  player.thirst = 100;
  if (!player.inventory.empty()) {
    for (Item* item : player.inventory) {
      delete item;
    }
  }
  player.inventory.clear();
  player.inventory.push_back(new Shovel());
  player.inventory.push_back(new Food());
  player.inventory.push_back(new Bottle());
}

void PreloadMapTexture() {
  if (mapTexture_Part_Hill != nullptr) return;
  mapTexture_Part_Hill = IMG_LoadTexture(renderer, MAP_PART_HILL_PATH);
  if (mapTexture_Part_Hill == NULL) throw std::runtime_error("Error loading map texture");
}

void RenderChargeBar() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &chargeRectBg);
  SDL_Rect chargeRect = { chargeRectBg.x, chargeRectBg.y, static_cast<int>(shovelDiggingChargeProgress), chargeRectBg.h };
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(renderer, &chargeRect);
}

void RenderWidget(SDL_Rect anchor, SDL_Texture* icon, int val, int maxVal, SDL_Color theme){
  SDL_Rect iconRect, valRect, barRect;
  SDL_Texture* valTexture = nullptr;
  iconRect = { anchor.x + 5, anchor.y + 2, 32, 32 };
  valTexture = renderText(std::to_string(val).c_str(), widgetFont, theme);
  SDL_QueryTexture(valTexture, NULL, NULL, &valRect.w, &valRect.h);
  valRect.x = iconRect.x + iconRect.w + 5;
  valRect.y = iconRect.y + (iconRect.h - valRect.h) * 0.5;
  barRect = { iconRect.x + 5, iconRect.y + iconRect.h - 1, static_cast<int>((anchor.w - 15) * (static_cast<float>(val) / maxVal)), 2 };

  SDL_SetRenderDrawColor(renderer, theme.r, theme.g, theme.b, 50);
  SDL_RenderFillRect(renderer, &anchor);
  SDL_RenderCopy(renderer, icon, NULL, &iconRect);
  SDL_RenderCopy(renderer, valTexture, NULL, &valRect);
  SDL_SetRenderDrawColor(renderer, theme.r, theme.g, theme.b, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &barRect);
  SDL_DestroyTexture(valTexture);
}

void RenderPlayerStats() {
  SDL_Rect playerStat_Background = { 20, 480, 200, 95 };
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
  SDL_RenderFillRect(renderer, &playerStat_Background);

  SDL_Rect playerStat_HP_Anchor = { playerStat_Background.x + 5,
    playerStat_Background.y + 5,
    190, 40 };
  SDL_Rect playerStat_Energy_Anchor = { playerStat_HP_Anchor.x,
    playerStat_HP_Anchor.y + playerStat_HP_Anchor.h + 5,
    95, 40 };
  SDL_Rect playerStat_Thirst_Anchor = { playerStat_Background.x + playerStat_Background.w / 2,
    playerStat_Energy_Anchor.y,
    95, 40 };
  
  RenderWidget(playerStat_HP_Anchor, hpIconTexture, static_cast<int>(player.health), 100, {255, 0, 0, 255});
  RenderWidget(playerStat_Energy_Anchor, energyIconTexture, static_cast<int>(player.energy), 100, {0, 255, 0, 255});
  RenderWidget(playerStat_Thirst_Anchor, thirstIconTexture, static_cast<int>(player.thirst), 100, {150, 150, 255, 255});
  
}

Item* inv_prevItem = nullptr, *inv_currItem = nullptr, *inv_nextItem = nullptr, *prevItem = nullptr;
void RenderItem(Item* item, SDL_Rect anchor, int alpha) {
  SDL_Rect itemIconRect = { anchor.x + 5, anchor.y, 32, 32 };
  if (item->sprite == nullptr) {
    item->sprite = IMG_LoadTexture(renderer, item->itemSpritePath.c_str());
  }
  SDL_Texture* itemNameTexture = renderText(item->itemName.c_str(), inventoryFont, {255, 255, 255, static_cast<Uint8>(alpha)});
  SDL_Texture* itemDescTexture = renderText(item->itemDescription.c_str(), inventoryFont, {255, 255, 255, static_cast<Uint8>(alpha)});

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
  SDL_Rect anchorRect = { 580, 480, 200, 95 };
  if (player.currentItem - 1 >= 0) inv_prevItem = player.inventory[player.currentItem - 1];
  else inv_prevItem = nullptr;
  inv_currItem = player.inventory[player.currentItem];
  if (player.currentItem + 1 < player.inventory.size()) inv_nextItem = player.inventory[player.currentItem + 1];
  else inv_nextItem = nullptr;

  SDL_Rect prevItemRect = { anchorRect.x + 5, anchorRect.y, anchorRect.w, 30 };
  SDL_Rect currItemRect = { prevItemRect.x, anchorRect.y + 30, anchorRect.w, 30 };
  SDL_Rect nextItemRect = { prevItemRect.x, anchorRect.y + 60, anchorRect.w, 30 };

  if (inv_prevItem != nullptr) RenderItem(inv_prevItem, prevItemRect, 150);
  if (inv_nextItem != nullptr) RenderItem(inv_nextItem, nextItemRect, 150);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
  SDL_RenderFillRect(renderer, &anchorRect);
  RenderItem(inv_currItem, currItemRect, 255);
}

void RenderHumanoid(Humanoid* humanoid) {
 if (humanoid->characterTexture == nullptr) {
    humanoid->characterTexture = IMG_LoadTexture(renderer, humanoid->characterTexturePath.c_str());
    if (humanoid->characterTexture == NULL) {
      throw std::runtime_error("Failed to render humanoid texture");
    }
  }
  SDL_Texture* characterName = renderText(humanoid->characterName.c_str(), inventoryFont, {255, 255, 255, 255});
  SDL_Rect characterNameRect;
  SDL_QueryTexture(characterName, NULL, NULL, &characterNameRect.w, &characterNameRect.h);
  characterNameRect = { humanoid->characterRect.x - (humanoid->characterRect.w / 2), humanoid->characterRect.y + humanoid->characterRect.w, characterNameRect.w, characterNameRect.h };
  SDL_RenderCopy(renderer, characterName, NULL, &characterNameRect);
  SDL_RenderCopy(renderer, humanoid->characterTexture, NULL, &humanoid->characterRect); 
  SDL_DestroyTexture(characterName);
}
