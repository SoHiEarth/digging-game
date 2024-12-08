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
};
extern std::vector<Hole*> holesVec;
extern void PreloadHoleTexture();

#endif // HOLE_H
