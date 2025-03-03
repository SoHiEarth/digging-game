#include <SDL_image.h>
#include <application.h>
#include <animate.h>
#include <base.h>
#include <renderer_temp.h>
#include <SDL.h>
#include <algorithm>
#include <humanoid.h>
#include <error.h>
#include <level.h>
#include <resload.h>
#include <object.h>
std::map<SDL_Keycode, bool> key_states, prev_key_states;
void Application::Fixed(std::atomic<bool>& running) {
  while (running && state == APP_STATE_GAME) {
    player->energy = std::clamp(player->energy, 0.0f, 100.0f);
    if (key_states[SDLK_w] || key_states[SDLK_a] || key_states[SDLK_s] || key_states[SDLK_d]) {
      player->energy -= 0.01f;
    }
    if (player->thirst <= 0) player->health -= 0.05f;
    else player->thirst -= 0.01f;
    if (player->energy <= 0) player->health -= 0.075f;
    if (player->thirst > 90 && player->energy > 90 && player->health < 100) {
      player->health += 0.05f;
    }
    for (const auto object : level.objects) {
      if (object->enabled)
        object->Fixed();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
  }
}

void Application::Game() {
  SDL_SetWindowTitle(window, "Holes - Game");
  LoadStatusBarIcons();
  func_button_pressed = false;
  talk_button_pressed = false;
  key_states.clear();
  inventoryFont = ResLoad::LoadFont(current_asset_bundle.FONT_GAME_INVENTORY_PATH.c_str(), 16);
  if (!level.loaded) {
    level.Load("assets/1.lvl");
    ResetPlayerStats();
  }
  if (!level.fixed_thread.open) {
    level.fixed_thread.Start(std::bind(&Application::Fixed, this, std::placeholders::_1), "Fixed");
  }
  while (state == APP_STATE_GAME) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        state = APP_STATE_MAIN_MENU;
      }
      if (event.type == SDL_KEYDOWN) {
        key_states[event.key.keysym.sym] = true;
      }
      if (event.type == SDL_KEYUP) {
        key_states[event.key.keysym.sym] = false;
      }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          SDL_GetWindowSize(window, &window_width, &window_height);
        }
      }
    }
    if (key_states[SDLK_ESCAPE]) state = APP_STATE_QUIT;
    if (key_states[SDLK_e] && !prev_key_states[SDLK_e]) {
      func_button_pressed = true;
      if (!player->inventory.empty()) {
        player->inventory[std::clamp(player->current_item, 0, (int)player->inventory.size())]->func();
      }
    }
    else func_button_pressed = false;
    talk_button_pressed = key_states[SDLK_f];
    for (int i = 1; i < 10; i++) {
      if (key_states[SDL_GetKeyFromName(std::to_string(i).c_str())]) {
        player->current_item = std::clamp(i - 1, 0, static_cast<int>(player->inventory.size()));
      }
    }
    if (player->current_item > player->inventory.size() - 1) player->current_item = 0;
    level.camera.Render();
    for (const auto object : level.objects) {
      if (object->enabled)
        object->LateUpdate();
    }
    if (player->health <= 0) {
      state = APP_STATE_GAME_OVER;
    }
    prev_key_states = key_states;
    level.CopyTempObjects();
    SDL_Delay(1000/60);
    if (level.HasNextFramePath()) {
      level.LoadNextFramePath();
    }
  }
}
