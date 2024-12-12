#ifndef RENDERER_TEMP_HPP
#define RENDERER_TEMP_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <config.h>
#include <items.hpp>
#include <player.hpp>
#include <base.hpp>
#include <interactable.hpp>
#include <hole.hpp>
#include <character.hpp>
#include <assetbundleloader.hpp>

extern SDL_Texture *hpIconTexture, *thirstIconTexture, *energyIconTexture;
extern TTF_Font *widgetFont, *inventoryFont;
extern SDL_Texture* mapTexture_Part_Hill;
extern SDL_Rect mapRect;
extern bool player_Up, player_Down, player_Left, player_Right;
extern SDL_Rect playerRect, itemRect;
extern SDL_Rect chargeRectBg;
 
extern WaterRefillStation* waterRefillStation;

SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color color);

void PreloadStatusBarIcons();

void PreloadPlayerSprite();

void ResetPlayerStats();

void PreloadMapTexture();

void RenderChargeBar();

void RenderWidget(SDL_Rect anchor, SDL_Texture* icon, int val, int maxVal, SDL_Color theme);

void RenderPlayerStats(); 

void RenderItem(Item* item, SDL_Rect anchor, int alpha);

void RenderInventory();

void RenderHumanoid(Humanoid* humanoid);

#endif
