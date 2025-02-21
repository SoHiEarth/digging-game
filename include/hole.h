#ifndef HOLE_H
#define HOLE_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <base.h>
#include <config.h>

extern SDL_Texture* hole_texture;

struct Hole {
  SDL_Rect rect = { 0, 0, 32, 32 };
  int progress = 0, speed = 10;
};
extern std::vector<Hole*> holes_vector;
void PreloadHoleTexture();
void RenderHole(Hole& hole);
#endif // HOLE_H
