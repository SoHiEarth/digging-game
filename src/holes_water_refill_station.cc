#include <interactable.h>
#include <items.h>
WaterRefillStation::WaterRefillStation() {
  name = current_asset_bundle.WATER_REFILL_STATION_NAME;
  texture_path = current_asset_bundle.WATER_REFILL_STATION_SPRITE_PATH;
  rect = { 0, 0, 64, 128 }; 
}

void WaterRefillStation::func() {
  for (int i = 0; i < player.inventory.size(); i++) {
    if (player.inventory[i] == nullptr) {
      continue;
    }
    if (player.inventory[i]->itemName == "Bottle") {
      Bottle* bottle = static_cast<Bottle*>(player.inventory[i]);
      if (bottle == nullptr) continue;
      else {
        bottle->capacity = 100;
      }
      break;
    }
  }
  player.inventory.push_back(new Bottle());
}
