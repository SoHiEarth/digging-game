#include <player.h>
#include <resload.h>
#include <level.h>
#include <base.h>
void Player::Start() {
  texture = ResLoad::LoadImage(current_asset_bundle.PLAYER_SPRITE_PATH.c_str());
  rect = { 0, 0, 64, 64 };
}

void Player::Fixed() {
  if (key_states[SDLK_w]) {
    if (energy > 0) rect.y -= move_speed;
    else rect.y -= move_speed / 2;
  }
  if (key_states[SDLK_s]) {
    if (energy > 0) rect.y += move_speed;
    else rect.y += move_speed / 2;
  }
  if (key_states[SDLK_a]) {
    if (energy > 0) rect.x -= move_speed;
    else rect.x -= move_speed / 2;
  }
  if (key_states[SDLK_d]) {
    if (energy > 0) rect.x += move_speed;
    else rect.x += move_speed / 2;
  }
}

void Player::Update() {
  // center the player on the screen using camera
  if (player_should_lock_camera) {
    level.camera.position = { rect.x - window_width / 2, rect.y - window_height / 2 };
  }
}

void Player::Quit() {
  SDL_DestroyTexture(texture);
}
