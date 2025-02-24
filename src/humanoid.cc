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
  if (player_up) {
    if (player.energy > 0) rect.y += player.move_speed;
    else rect.y += player.move_speed / 2;
  }
  if (player_down) {
    if (player.energy > 0) rect.y -= player.move_speed;
    else rect.y -= player.move_speed / 2;
  }
  if (player_left) {
    if (player.energy > 0) rect.x += player.move_speed;
    else rect.x += player.move_speed / 2;
  }
  if (player_right) {
    if (player.energy > 0) rect.x -= player.move_speed;
    else rect.x -= player.move_speed / 2;
  }
}

void Humanoid::Update() {
  
  // Render the humanoid
  SDL_RenderCopy(renderer, texture, NULL, &rect);

  // If the humanoid has something new to say, render a exclamation mark with a red color
  if (has_critical_update) {
    auto exclamation_texture = ResLoad::RenderText(inventoryFont, "!");
    SDL_Rect exclamationRect = { rect.x + 20, rect.y - 30 };
    SDL_QueryTexture(exclamation_texture, NULL, NULL, &exclamationRect.w, &exclamationRect.h);
    SDL_Rect exclamation_bg_rect = { exclamationRect.x - 5, exclamationRect.y - 5, exclamationRect.w + 10, exclamationRect.h + 10 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &exclamation_bg_rect);
    SDL_RenderCopy(renderer, exclamation_texture, NULL, &exclamationRect);
    SDL_DestroyTexture(exclamation_texture);
  }

  if (SDL_HasIntersection(&player.rect, &rect) == SDL_TRUE) {
    auto control_texture = ResLoad::RenderText(inventoryFont, "Press [F] to speak");
    SDL_Rect talkControlRect = { rect.x, rect.y - 30};
    SDL_QueryTexture(control_texture, NULL, NULL, &talkControlRect.w, &talkControlRect.h);
    SDL_RenderCopy(renderer, control_texture, NULL, &talkControlRect);
    SDL_DestroyTexture(control_texture);
    if (talk_button_pressed) {
      current_humanoid = this;
      app->state = APP_STATE_DIALOUGE;
    }
  }
}
