#ifndef CAMERA_H
#define CAMERA_H
#include <vec2.h>
struct Camera {
  vec2 position{0, 0};
  void Render();
};
SDL_Rect GetRectInCameraSpace(SDL_Rect rect);
#endif // CAMERA_H
