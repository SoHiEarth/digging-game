#ifndef HOLES_3_MANAGER_H
#define HOLES_3_MANAGER_H
#include <object.h>
#include <base.h>
#include <resload.h>
#include "config.h"
struct Holes_3_Manager : public Object {
  void Start() {
    TTF_Font* font = ResLoad::LoadFont(_FONT_MENU_HINT_PATH, 16);
    texture = ResLoad::RenderText(font, "Press [F] to sleep and recuperate energy", {255, 255, 255});
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    rect.x = (window_width - rect.w) / 2;
    rect.y = (window_height - rect.h) / 2;
    ResLoad::FreeFont(font);
  }
  void Update() {
    SDL_Delay(1000);
    level.LoadAtNextFrame("assets/4.lvl");
  }
  void Quit() {
    SDL_DestroyTexture(texture);
  }
};

#endif // HOLES_3_MANAGER_H
