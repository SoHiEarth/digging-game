#include <interactable.hpp>

WaterRefillStation::WaterRefillStation() {
  interactableName = current_asset_bundle.WATER_REFILL_STATION_NAME;
  texturePath = current_asset_bundle.WATER_REFILL_STATION_SPRITE_PATH;
  rect = { 0, 0, 64, 128 }; 
}

void WaterRefillStation::func() {
  for (int i = 0; i < player.inventory.size(); i++) {
    if (player.inventory[i]->itemName == "Bottle") {
      player.inventory.erase(player.inventory.begin() + i);
      break;
    }
  }
  player.inventory.push_back(new Bottle());
}
