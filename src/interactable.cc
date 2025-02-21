#include <interactable.h>
#include <base.h>
#include <renderer_temp.h>
#include <resload.h>
void MapInteractable::Update() {
  if (SDL_HasIntersection(&player.rect, &rect) == SDL_TRUE && key_states[SDLK_f]) {
    TTF_SetFontStyle(inventoryFont, TTF_STYLE_BOLD);
    auto control_data = ResLoad::RenderText(inventoryFont, "Press [F] to interact");
    SDL_Rect talkControlRect = { rect.x, rect.y - 30, control_data.w, control_data.h };
    SDL_RenderCopy(renderer, control_data.texture, NULL, &talkControlRect);
    SDL_DestroyTexture(control_data.texture);
    if (talk_button_pressed) {
      func();
    }
  }
}

void MapInteractable::func() {
  throw std::runtime_error("Interactable function not configured");
}
