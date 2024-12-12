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

extern const char* _APP_TITLE;

extern const char* _ITEM_NULL_NAME;
extern const char* _ITEM_NULL_DESCRIPTION;
extern const char* _ITEM_NULL_SPRITE_PATH;

extern const char* _INTERACTABLE_NULL_NAME;
extern const char* _INTERACTABLE_NULL_SPRITE_PATH;

extern const int _PLAYER_MOVE_SPEED;

extern const char* _HOLE_UNIFIED_TEXTURE_PATH;

extern const char* _SHOVEL_ITEM_NAME;
extern const char* _SHOVEL_ITEM_DESCRIPTION;
extern const char* _SHOVEL_ITEM_SPRITE_PATH;

extern const char* _BOTTLE_ITEM_NAME;
extern const char* _BOTTLE_ITEM_SPRITE_PATH;

extern const char* _FOOD_ITEM_NAME;
extern const char* _FOOD_ITEM_SPRITE_PATH;

extern const char* _WATER_REFILL_STATION_NAME;
extern const char* _WATER_REFILL_STATION_SPRITE_PATH;

extern const char* _PLAYERSTAT_HEALTH_ICON_PATH;
extern const char* _PLAYERSTAT_ENERGY_ICON_PATH;
extern const char* _PLAYERSTAT_THIRST_ICON_PATH;
extern const char* _PLAYERSTAT_FONT_PATH;

extern const char* _PLAYER_SPRITE_PATH;

extern const char* _MAP_PART_HILL_PATH;

extern const char* _FONT_MENU_HINT_PATH;
extern const char* _MENU_BACKGROUND_PATH;

extern const char* _FONT_GAME_INVENTORY_PATH;

extern const char* _FONT_GAMEOVER_PATH;
extern const char* _FONT_GAMEOVER_HINT_PATH;

extern int _DIALOUGE_BG_BORDER_THICKNESS;
extern const char* _FONT_GAME_DIALOUGE_PATH;
extern const char* _FONT_GAME_DIALOUGE_NAME_PATH;

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H
