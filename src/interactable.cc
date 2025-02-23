#include <interactable.h>
#include <base.h>
#include <renderer_temp.h>
#include <resload.h>
void MapInteractable::Update() {
  if (SDL_HasIntersection(&player.rect, &rect) == SDL_TRUE && key_states[SDLK_f]) {
    TTF_SetFontStyle(inventoryFont, TTF_STYLE_BOLD);
    auto control_texture = ResLoad::RenderText(inventoryFont, "Press [F] to interact");
    SDL_Rect talkControlRect = { rect.x, rect.y - 30 };
    SDL_QueryTexture(control_texture, NULL, NULL, &talkControlRect.w, &talkControlRect.h);
    SDL_RenderCopy(renderer, control_texture, NULL, &talkControlRect);
    SDL_DestroyTexture(control_texture);
    if (talk_button_pressed) {
      func();
    }
  }
}

void MapInteractable::func() {
  throw std::runtime_error("Interactable function not configured");
}
