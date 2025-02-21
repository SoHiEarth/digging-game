#include <interactable.h>
#include <base.h>
#include <SDL_image.h>
#include <renderer_temp.h>
#include <iostream>
TransportBus_Lv1::TransportBus_Lv1() {
  rect = {0, 0, 120, 240};
  SDL_GetWindowSize(window, &rect.x, &rect.y);
  rect.x = (rect.x - rect.w) / 2;
  rect.y = (rect.y - rect.h) / 2;
  animator_transform = new Animator_Rect(rect);
  animator_brightness = new Animator_Brightness();
}

void TransportBus_Lv1::Start() {
  texture = IMG_LoadTexture(renderer, "assets/transport_bus_1.png");
  if (texture == nullptr) {
    std::cerr << "Failed to load texture: " << SDL_GetError() << std::endl;
  }
  animator_transform->LoadAnimation("assets/transport_bus_1.anim");
  animator_brightness->LoadAnimation("assets/transport_1_brightness.anim");
  animator_transform->Play();
  animator_brightness->Play();
}

void TransportBus_Lv1::Update() {
  player_up = true;
  player_down = false;
  player_left = false;
  player_right = false;
  player.rect.x = rect.x + rect.w / 2;
  player.rect.y = rect.y + rect.h / 2;
  //SDL_RenderCopy(renderer, texture, NULL, &rect);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &rect);
}

void TransportBus_Lv1::Quit() {
  SDL_DestroyTexture(texture);
  delete animator_transform;
}
