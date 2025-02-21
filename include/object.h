#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
struct Object {
  SDL_Rect rect;
  SDL_Texture* texture;
  virtual void Start() {}
  virtual void Update() {}
  virtual void Fixed() {}
  virtual void Quit() {}
};

#endif // OBJECT_H
