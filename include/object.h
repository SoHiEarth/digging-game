#ifndef OBJECT_H
#define OBJECT_H
#include <SDL.h>
#include <map>
struct Object {
  bool enabled         = true;
  SDL_Rect rect        = { 0, 0, 64, 64 };
  SDL_Texture* texture = nullptr;
  std::map<SDL_Texture*, SDL_Rect> extras;
  virtual void Start()      {}
  virtual void Update()     {}
  virtual void Fixed()      {}
  virtual void LateUpdate() {}
  virtual void Quit()       {}
};
#endif // OBJECT_H
