#include <interactable.hpp>
#include <base.hpp>
#include <renderer_temp.hpp>
void MapInteractable::Update() {
  if (SDL_PointInRect(player.position.toSDLPointPtr(), &rect) & key_states[SDLK_f]) {
    TTF_SetFontStyle(inventoryFont, TTF_STYLE_BOLD);
    SDL_Texture* talkControlTexture = renderText("Press [F] to interact", inventoryFont, {255, 255, 255, 200});
    SDL_Rect talkControlRect = { rect.x, rect.y - 30, 0, 0};
    SDL_QueryTexture(talkControlTexture, NULL, NULL, &talkControlRect.w, &talkControlRect.h);
    SDL_RenderCopy(renderer, talkControlTexture, NULL, &talkControlRect);
    SDL_DestroyTexture(talkControlTexture);
    if (talk_button_pressed) {
      func();
    }
  }
}

void MapInteractable::func() {
  throw std::runtime_error("Interactable function not configured");
}
