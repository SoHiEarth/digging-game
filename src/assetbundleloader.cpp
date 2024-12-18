#include <assetbundleloader.hpp>
#include <string>
#include <fstream>
#include <sstream>

AssetBundle assetBundle;

void AssetBundle::LoadAssetBundle(const char* path) {
  // Load asset bundle from path
  // {VariableName} = {AssetValue};
  
  std::ifstream file(path);
  std::string line;
  int lineNumber = 0;
  while (std::getline(file, line)) {
    lineNumber++;
    std::istringstream iss(line);
    std::string variableName;
    std::string equalSign;
    std::string assetValue;
    iss >> variableName >> equalSign >> assetValue;
    if (equalSign == "=") {
      if (variableName == "APP_TITLE") {
        APP_TITLE = assetValue;
      } else if (variableName == "ITEM_NULL_NAME") {
        ITEM_NULL_NAME = assetValue;
      } else if (variableName == "ITEM_NULL_DESCRIPTION") {
        ITEM_NULL_DESCRIPTION = assetValue;
      } else if (variableName == "ITEM_NULL_SPRITE_PATH") {
        ITEM_NULL_SPRITE_PATH = assetValue;
      } else if (variableName == "INTERACTABLE_NULL_NAME") {
        INTERACTABLE_NULL_NAME = assetValue;
      } else if (variableName == "INTERACTABLE_NULL_SPRITE_PATH") {
        INTERACTABLE_NULL_SPRITE_PATH = assetValue;
      } else if (variableName == "PLAYER_MOVE_SPEED") {
        PLAYER_MOVE_SPEED = std::stoi(assetValue);
      } else if (variableName == "HOLE_UNIFIED_TEXTURE_PATH") {
        HOLE_UNIFIED_TEXTURE_PATH = assetValue;
      } else if (variableName == "SHOVEL_ITEM_NAME") {
        SHOVEL_ITEM_NAME = assetValue;
      } else if (variableName == "SHOVEL_ITEM_DESCRIPTION") {
        SHOVEL_ITEM_DESCRIPTION = assetValue;
      } else if (variableName == "SHOVEL_ITEM_SPRITE_PATH") {
        SHOVEL_ITEM_SPRITE_PATH = assetValue;
      } else if (variableName == "BOTTLE_ITEM_NAME") {
        BOTTLE_ITEM_NAME = assetValue;
      } else if (variableName == "BOTTLE_ITEM_SPRITE_PATH") {
        BOTTLE_ITEM_SPRITE_PATH = assetValue;
      } else if (variableName == "FOOD_ITEM_NAME") {
        FOOD_ITEM_NAME = assetValue;
      } else if (variableName == "FOOD_ITEM_SPRITE_PATH") {
        FOOD_ITEM_SPRITE_PATH = assetValue;
      } else if (variableName == "WATER_REFILL_STATION_NAME") {
        WATER_REFILL_STATION_NAME = assetValue;
      } else if (variableName == "WATER_REFILL_STATION_SPRITE_PATH") {
        WATER_REFILL_STATION_SPRITE_PATH = assetValue;
      } else if (variableName == "PLAYERSTAT_HEALTH_ICON_PATH") {
        PLAYERSTAT_HEALTH_ICON_PATH = assetValue;
      } else if (variableName == "PLAYERSTAT_ENERGY_ICON_PATH") {
        PLAYERSTAT_ENERGY_ICON_PATH = assetValue;
      } else if (variableName == "PLAYERSTAT_THIRST_ICON_PATH") {
        PLAYERSTAT_THIRST_ICON_PATH = assetValue;
      } else if (variableName == "PLAYERSTAT_FONT_PATH") {
        PLAYERSTAT_FONT_PATH = assetValue;
      } else if (variableName == "PLAYER_SPRITE_PATH") {
        PLAYER_SPRITE_PATH = assetValue;
      } else if (variableName == "MAP_PART_HILL_PATH") {
        MAP_PART_HILL_PATH = assetValue;
      } else if (variableName == "FONT_MENU_HINT_PATH") {
        FONT_MENU_HINT_PATH = assetValue;
      } else if (variableName == "MENU_BACKGROUND_PATH") {
        MENU_BACKGROUND_PATH = assetValue;
      } else if (variableName == "FONT_GAME_INVENTORY_PATH") {
        FONT_GAME_INVENTORY_PATH = assetValue;
      } else if (variableName == "FONT_GAMEOVER_PATH") {
        FONT_GAMEOVER_PATH = assetValue;
      } else if (variableName == "FONT_GAMEOVER_HINT_PATH") {
        FONT_GAMEOVER_HINT_PATH = assetValue;
      } else if (variableName == "DIALOUGE_BG_BORDER_THICKNESS") {
        DIALOUGE_BG_BORDER_THICKNESS = std::stoi(assetValue);
      } else if (variableName == "FONT_GAME_DIALOUGE_PATH") {
        FONT_GAME_DIALOUGE_PATH = assetValue;
      } else if (variableName == "FONT_GAME_DIALOUGE_NAME_PATH") {
        FONT_GAME_DIALOUGE_NAME_PATH = assetValue;
      }
    } else {
    throw std::runtime_error("Invalid asset bundle format, expected " + variableName + " = AsseetValue at line " + std::to_string(lineNumber));
    }
  }
}
