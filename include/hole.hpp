#ifndef HOLE_H
#define HOLE_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <base.hpp>
#include <config.h>

extern SDL_Texture* hole_unifiedTexture;

struct Hole {
  SDL_Rect holeRect;
  int hole_dig_progress = 0;
};
extern std::vector<Hole*> holesVec;
void PreloadHoleTexture();
void RenderHole(Hole& hole);
#endif // HOLE_H
