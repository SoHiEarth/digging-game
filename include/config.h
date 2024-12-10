#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define FONT_PRESET_INTER "assets/Inter/Inter.ttf";
#define FONT_PRESET_INTER_ITALIC "assets/Inter/Inter-Italic.ttf";
#define FONT_PRESET_SPACE_GROTESK "assets/SpaceGrotesk/SpaceGrotesk.ttf";
#define FONT_PRESET_SPACE_MONO "assets/SpaceMono/SpaceMono-Regular.ttf";
#define FONT_PRESET_SPACE_MONO_BOLD "assets/SpaceMono/SpaceMono-Bold.ttf";
#define FONT_PRESET_JACQUARD "assets/Jacquard_12/Jacquard12-Regular.ttf";
#define FONT_PRESET_JERSEY_10 "assets/Jersey_10/Jersey10-Regular.ttf";

extern const char* APP_TITLE;

extern const char* ITEM_NULL_NAME;
extern const char* ITEM_NULL_DESCRIPTION;
extern const char* ITEM_NULL_SPRITE_PATH;

extern const char* INTERACTABLE_NULL_NAME;
extern const char* INTERACTABLE_NULL_SPRITE_PATH;

extern const int PLAYER_MOVE_SPEED;

extern const char* HOLE_UNIFIED_TEXTURE_PATH;

extern const char* SHOVEL_ITEM_NAME;
extern const char* SHOVEL_ITEM_DESCRIPTION;
extern const char* SHOVEL_ITEM_SPRITE_PATH;

extern const char* BOTTLE_ITEM_NAME;
extern const char* BOTTLE_ITEM_SPRITE_PATH;

extern const char* FOOD_ITEM_NAME;
extern const char* FOOD_ITEM_SPRITE_PATH;

extern const char* WATER_REFILL_STATION_NAME;
extern const char* WATER_REFILL_STATION_SPRITE_PATH;

extern const char* PLAYERSTAT_HEALTH_ICON_PATH;
extern const char* PLAYERSTAT_ENERGY_ICON_PATH;
extern const char* PLAYERSTAT_THIRST_ICON_PATH;
extern const char* PLAYERSTAT_FONT_PATH;

extern const char* PLAYER_SPRITE_PATH;

extern const char* MAP_PART_HILL_PATH;

extern const char* FONT_MENU_HINT_PATH;
extern const char* MENU_BACKGROUND_PATH;

extern const char* FONT_GAME_INVENTORY_PATH;

extern const char* FONT_GAMEOVER_PATH;
extern const char* FONT_GAMEOVER_HINT_PATH;

extern int DIALOUGE_BG_BORDER_THICKNESS;
extern const char* FONT_GAME_DIALOUGE_PATH;
extern const char* FONT_GAME_DIALOUGE_NAME_PATH;

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H
