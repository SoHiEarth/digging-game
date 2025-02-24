#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
struct Object {
  SDL_Rect rect = { 0, 0, 64, 64 };
  SDL_Texture* texture = nullptr;
  virtual void Start() {}
  virtual void Update() {}
  virtual void Fixed() {}
  virtual void Quit() {}
};

#endif // OBJECT_H
