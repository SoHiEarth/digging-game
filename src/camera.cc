#include <camera.h>
#include <SDL.h>
#include <base.h>
#include <hole.h>
void Camera::Render() {
  // Render the camera
  for (const Object* object : level.objects) {
    // Render the object
    SDL_Rect screenspace_rect = {
      object->rect.x - position.x,
      object->rect.y - position.y,
      object->rect.w,
      object->rect.h
    };
    SDL_RenderCopy(renderer, object->texture, NULL, &screenspace_rect);
  }
  // Render holes as it is not a object
  for (Hole* hole : holes_vector) {
    // Render the hole
    RenderHole(*hole);
  }
}
