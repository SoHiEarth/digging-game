#include <holes_camp.h>
#include <resload.h>
#include "renderer_temp.h"
Holes::Camp_2_In::Camp_2_In() {
  rect = { -300, -300, 256, 256 };
}

void Holes::Camp_2_In::Start() {
  texture = ResLoad::LoadImage("assets/2_camp_in.png");
}

void Holes::Camp_2_In::Update() {
  if (SDL_HasIntersection(&player->rect, &rect)) {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
  }
}

void Holes::Camp_2_In::Quit() {
  ResLoad::FreeImage(texture);
}

Holes::Camp_2::Camp_2() {
  rect = { -300, -300, 256, 256 };
}

void Holes::Camp_2::Start() {
  texture = ResLoad::LoadImage("assets/2_camp_out.png");
}

void Holes::Camp_2::Update() {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  if (SDL_HasIntersection(&player->rect, &rect)) {
    SDL_SetTextureAlphaMod(texture, 100);
    if (global_brightness <= 30) {
      SDL_Texture* hint_texture = ResLoad::RenderText(inventoryFont, "Press [F] to sleep and recuperate energy", {255, 255, 255});
      SDL_Rect hint_rect;
      SDL_QueryTexture(hint_texture, NULL, NULL, &hint_rect.w, &hint_rect.h);
      hint_rect.x = rect.x - (hint_rect.w / 2);
      hint_rect.y = rect.y - hint_rect.h;
      extras.insert({hint_texture, hint_rect});
      if (key_states[SDLK_f]) {
        level.LoadAtNextFrame("assets/3.lvl");
      }
    }
  } else {
    SDL_SetTextureAlphaMod(texture, 255);
  }
}

void Holes::Camp_2::Quit() {
  ResLoad::FreeImage(texture);
}
