#ifndef CAMERA_H
#define CAMERA_H

#include <player.h>

struct Camera {
  vec2 position{0, 0};
  void Render();
};

#endif // CAMERA_H
