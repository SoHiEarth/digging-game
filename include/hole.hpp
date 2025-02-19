#ifndef HOLE_H
#define HOLE_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <base.hpp>
#include <config.h>

extern SDL_Texture* hole_unifiedTexture;

struct Hole {
  SDL_Rect holeRect = { 0, 0, 32, 32 };
  int hole_dig_progress = 0, hole_dig_speed = 10;
};
extern std::vector<Hole*> holesVec;
void PreloadHoleTexture();
void RenderHole(Hole& hole);
#endif // HOLE_H
