#ifndef HOLE_H
#define HOLE_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <base.h>
#include <config.h>
extern SDL_Texture* hole_texture;
struct Hole;
extern std::vector<Hole*> holes_vector;
struct Hole : public Object {
  int progress = 0, speed = 5;
  Hole() {
    if (player != nullptr)
      rect = player->rect;
    texture = hole_texture;
    holes_vector.push_back(this);
  }
};
void LoadHoleTexture();
void RenderHole(Hole& hole);
#endif // HOLE_H
