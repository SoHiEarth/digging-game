#include <camera.h>
#include <SDL.h>
#include <base.h>
#include <hole.h>
#include <resload.h>
void Camera::Render() {
  for (auto hole : holes_vector) {
    if (hole->enabled) {
      SDL_Rect screenspace_rect = {
        hole->rect.x - position.x,
        hole->rect.y - position.y,
        hole->rect.w,
        hole->rect.h
      };
      RenderHole(hole, screenspace_rect);
    }
  }

  for (Object* object : level.objects) {
    if (!object->enabled) continue;
    SDL_Rect screenspace_rect = {
      object->rect.x - position.x,
      object->rect.y - position.y,
      object->rect.w,
      object->rect.h
    };
    SDL_RenderCopy(renderer, object->texture, NULL, &screenspace_rect);
    for (const auto& [texture, rect] : object->extras) {
      SDL_Rect screenspace_rect = {
        rect.x - position.x,
        rect.y - position.y,
        rect.w,
        rect.h
      };
      SDL_RenderCopy(renderer, texture, NULL, &screenspace_rect);
      SDL_DestroyTexture(texture);
    }
    // clear the extras map
    object->extras.clear();
  }
  if (!player->inventory.empty()) {
    if (player->inventory[player->current_item]->sprite == nullptr) {
      player->inventory[player->current_item]->sprite = ResLoad::LoadImage(player->inventory[player->current_item]->itemSpritePath);
    }
    SDL_Rect item_rect = {
      player->rect.x + 5 - position.x,
      player->rect.y + 10 - position.y,
      player->rect.w - 10,
      player->rect.h - 15
     };
    SDL_RenderCopy(renderer, player->inventory[player->current_item]->sprite, NULL, &item_rect);
  }
}

SDL_Rect GetRectInCameraSpace(SDL_Rect rect) {
  return { rect.x - level.camera.position.x, rect.y - level.camera.position.y, rect.w, rect.h };
}
