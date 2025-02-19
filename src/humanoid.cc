#include <humanoid.h>
#include <base.hpp>
#include <application.hpp>
#include <renderer_temp.hpp>
#include <SDL_image.h>
Humanoid::~Humanoid() {
  SDL_DestroyTexture(humanoid_texture);
}

void Humanoid::Start() {
  if (humanoid_texture == nullptr) {
    humanoid_texture = IMG_LoadTexture(renderer, humanoid_texture_path.c_str());
    if (humanoid_texture == nullptr) {
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
  SDL_RenderCopy(renderer, humanoid_texture, NULL, &rect);

  // Talking if player is in 
  // 10 pixel radius of the humanoid
  if (SDL_PointInRect(player.position.toSDLPointPtr(), &rect)) {
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
