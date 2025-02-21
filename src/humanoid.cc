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
  texture = ResLoad::LoadImage(texture_path).texture;
}

void Humanoid::Update() {
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

  // Render the humanoid
  SDL_RenderCopy(renderer, texture, NULL, &rect);

  // Talking if player is in 
  // 10 pixel radius of the humanoid
  if (SDL_HasIntersection(&player.rect, &rect) == SDL_TRUE) {
    auto control_data = ResLoad::RenderText(inventoryFont, "Press [F] to speak");
    SDL_Rect talkControlRect = { rect.x, rect.y - 30, control_data.w, control_data.h};
    SDL_RenderCopy(renderer, control_data.texture, NULL, &talkControlRect);
    SDL_DestroyTexture(control_data.texture);
    if (talk_button_pressed) {
      current_humanoid = this;
      app->state = APP_STATE_DIALOUGE;
    }
  }
}
