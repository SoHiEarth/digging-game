#ifndef RENDERER_TEMP_HPP
#define RENDERER_TEMP_HPP
#include <SDL.h>
#include <SDL_ttf.h>
#include <items.h>
extern SDL_Texture *hpIconTexture, *thirstIconTexture, *energyIconTexture;
extern TTF_Font *widgetFont, *inventoryFont;
void LoadStatusBarIcons();
void ResetPlayerStats();
void RenderWidget(SDL_Rect anchor, SDL_Texture* icon, int val, int maxVal, SDL_Color theme);
void RenderPlayerStats();
void RenderItem(Item* item, SDL_Rect anchor, int alpha);
void RenderInventory();
#endif
