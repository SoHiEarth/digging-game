#include <SDL_rect.h>
#include <humanoid.h>
#include <base.h>
#include <application.h>
#include <renderer_temp.h>
#include <SDL_image.h>
#include <resload.h>
Humanoid::~Humanoid() {
  SDL_DestroyTexture(texture);
}

void Humanoid::Start() {
  texture = ResLoad::LoadImage(texture_path);
}

void Humanoid::Fixed() {
}

void Humanoid::Update() {
  SDL_Texture* humanoid_name_texture = ResLoad::RenderText(inventoryFont, name);
  SDL_Rect name_rect, name_bg_rect;
  SDL_QueryTexture(humanoid_name_texture, NULL, NULL, &name_rect.w, &name_rect.h);
  name_rect.x = rect.x + (rect.w - name_rect.w) / 2;
  name_rect.y = rect.y + rect.h + 5;
  name_bg_rect = { name_rect.x - 5, name_rect.y - 5, name_rect.w + 10, name_rect.h + 10 };
  extras.insert({ResLoad::MakeTextureFromColor(name_rect.w, name_rect.h, {0, 0, 0, 255}), name_bg_rect});
  extras.insert({humanoid_name_texture, name_rect});

  // If the humanoid has something new to say, render a exclamation mark with a red color
  if (has_critical_update) {
    auto exclamation_texture = ResLoad::RenderText(inventoryFont, "!");
    SDL_Rect exclamationRect = { rect.x + 20, rect.y - 30 };
    SDL_QueryTexture(exclamation_texture, NULL, NULL, &exclamationRect.w, &exclamationRect.h);
    SDL_Rect exclamation_bg_rect = { exclamationRect.x - 5, exclamationRect.y - 5, exclamationRect.w + 10, exclamationRect.h + 10 };
    extras.insert({ResLoad::MakeTextureFromColor(exclamation_bg_rect.w, exclamation_bg_rect.h, {255, 0, 0, 255}), exclamation_bg_rect});
    extras.insert({exclamation_texture, exclamationRect});
  }

  if (SDL_HasIntersection(&player->rect, &rect) == SDL_TRUE) {
    auto control_texture = ResLoad::RenderText(inventoryFont, "Press [F] to speak");
    SDL_Rect talkControlRect = { rect.x, rect.y - 30};
    SDL_QueryTexture(control_texture, NULL, NULL, &talkControlRect.w, &talkControlRect.h);
    extras.insert({control_texture, talkControlRect});
    if (talk_button_pressed) {
      current_humanoid = this;
      app->state = APP_STATE_DIALOUGE;
    }
  }
}
