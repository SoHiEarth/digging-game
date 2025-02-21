#include <SDL_rect.h>
#include <humanoid.h>
#include <base.h>
#include <application.h>
#include <renderer_temp.h>
#include <SDL_image.h>
Humanoid::~Humanoid() {
  SDL_DestroyTexture(texture);
}

void Humanoid::Start() {
  if (texture == nullptr) {
    texture = IMG_LoadTexture(renderer, texture_path.c_str());
    if (texture == nullptr) {
      throw std::runtime_error("Failed to load character texture");
    }
  }
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
    SDL_Texture* talkControlTexture = renderText("Press [F] to speak", inventoryFont, {255, 255, 255, 200});
    SDL_Rect talkControlRect = { rect.x, rect.y - 30, 0, 0};
    SDL_QueryTexture(talkControlTexture, NULL, NULL, &talkControlRect.w, &talkControlRect.h);
    if (SDL_RenderCopy(renderer, talkControlTexture, NULL, &talkControlRect) != 0) {
      throw std::runtime_error("Failed to render talk control texture: " + std::string(SDL_GetError()));
    }
    SDL_DestroyTexture(talkControlTexture);
    if (talk_button_pressed) {
      currentHumanoid = this;
      app->state = APP_STATE_DIALOUGE;
    }
  }
}
