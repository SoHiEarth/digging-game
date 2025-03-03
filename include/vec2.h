#ifndef VEC2_H
#define VEC2_H
#include <SDL.h>
struct vec2 {
  int x, y;
  vec2(int x, int y) : x(x), y(y) {}
  SDL_Point toSDLPoint() {
    return {x, y};
  }
  SDL_Point* toSDLPointPtr() {
    return new SDL_Point{x, y};
  }
};
#endif // VEC2_H
