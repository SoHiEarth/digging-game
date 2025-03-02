#include <SDL_blendmode.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <prompt.h>
#include <resload.h>
#include <base.h>
#include "application.h"
#include "renderer_temp.h"
// Basically start an event loop
std::vector<SDL_Texture*> prompt_sprite_textures;
Animator_Int* prompt_sprite_texture_animation;
int prompt_sprite_texture_index = 0;
void LoadPromptSprites() {
  if (!prompt_sprite_textures.empty()) return;
  prompt_sprite_textures.push_back(ResLoad::LoadImage("assets/loading_sprite_1.png"));
  prompt_sprite_textures.push_back(ResLoad::LoadImage("assets/loading_sprite_2.png"));
}

void StartAnimation() {
  LoadPromptSprites();
  prompt_sprite_texture_animation = new Animator_Int(prompt_sprite_texture_index);
  prompt_sprite_texture_animation->LoadAnimationByDelta("assets/loading_sprite.anim");
  prompt_sprite_texture_animation->Play();
}

void Holes::PromptPlayerWithMessage(std::string message) {
  bool closed = false;
  StartAnimation();
  TTF_SetFontStyle(inventoryFont, TTF_STYLE_BOLD);
  SDL_Texture* message_texture = ResLoad::RenderText(inventoryFont, message, {255, 255, 255} ,window_width - 200), *hint_texture = ResLoad::RenderText(inventoryFont, "Press any key to continue", {255, 255, 255, 170});
  TTF_SetFontStyle(inventoryFont, TTF_STYLE_NORMAL);
  SDL_Rect message_texture_rect, hint_texture_rect;
  SDL_QueryTexture(message_texture, NULL, NULL, &message_texture_rect.w, &message_texture_rect.h);
  SDL_QueryTexture(hint_texture, NULL, NULL, &hint_texture_rect.w, &hint_texture_rect.h);
  while (!closed) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        app->state = APP_STATE_QUIT;
        closed = true;
      }
      if (event.type == SDL_KEYDOWN) closed = true;
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        closed = true;
      }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.type == SDL_WINDOWEVENT_RESIZED) {
          SDL_GetWindowSize(window, &window_width, &window_height);
        }
      }
    }
    message_texture_rect.x = (window_width - message_texture_rect.w)/2;
    message_texture_rect.y = (window_height - message_texture_rect.h)/2;
    hint_texture_rect.x = (window_width - hint_texture_rect.w)/2;
    hint_texture_rect.y = window_height - hint_texture_rect.h - 10;
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_RenderClear(renderer);
    SDL_Rect prompt_sprite_rect = {window_width - 100, window_height - 100, 50, 50};
    prompt_sprite_texture_index = std::clamp(prompt_sprite_texture_index, 0, static_cast<int>(prompt_sprite_textures.size() - 1));
    prompt_sprite_rect.y = window_height - prompt_sprite_rect.h - 10;
    SDL_RenderCopy(renderer, prompt_sprite_textures[prompt_sprite_texture_index], NULL, &prompt_sprite_rect);
    SDL_RenderCopy(renderer, message_texture, NULL, &message_texture_rect);
    SDL_RenderCopy(renderer, hint_texture, NULL, &hint_texture_rect);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyTexture(message_texture);
  prompt_sprite_texture_animation->Halt();
}

void Holes::PromptPlayerWithIconBox(std::string message, SDL_Texture* icon) {
  bool closed = false;
  StartAnimation();
  TTF_SetFontStyle(inventoryFont, TTF_STYLE_BOLD);
  SDL_Texture* message_texture = ResLoad::RenderText(inventoryFont, message, {255, 255, 255} ,window_width - 200), *hint_texture = ResLoad::RenderText(inventoryFont, "Press any key to continue", {255, 255, 255, 170});
  TTF_SetFontStyle(inventoryFont, TTF_STYLE_NORMAL);
  SDL_Rect message_texture_rect, hint_texture_rect, icon_box_rect;
  SDL_QueryTexture(message_texture, NULL, NULL, &message_texture_rect.w, &message_texture_rect.h);
  SDL_QueryTexture(hint_texture, NULL, NULL, &hint_texture_rect.w, &hint_texture_rect.h);
  SDL_QueryTexture(icon, NULL, NULL, &icon_box_rect.w, &icon_box_rect.h);
  while (!closed) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        app->state = APP_STATE_QUIT;
        closed = true;
      }
      if (event.type == SDL_KEYDOWN) closed = true;
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        closed = true;
      }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.type == SDL_WINDOWEVENT_RESIZED) {
          SDL_GetWindowSize(window, &window_width, &window_height);
        }
      }
    }
    message_texture_rect.x = (window_width - message_texture_rect.w)/2;
    message_texture_rect.y = (window_height - message_texture_rect.h)/2 - 50;
    icon_box_rect.x = (window_width - icon_box_rect.w)/2;
    icon_box_rect.y = message_texture_rect.y + message_texture_rect.h + 20;
    hint_texture_rect.x = (window_width - hint_texture_rect.w)/2;
    hint_texture_rect.y = window_height - hint_texture_rect.h - 10;
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_RenderClear(renderer);
    SDL_Rect prompt_sprite_rect = {window_width - 100, window_height - 100, 50, 50};
    prompt_sprite_texture_index = std::clamp(prompt_sprite_texture_index, 0, static_cast<int>(prompt_sprite_textures.size() - 1));
    prompt_sprite_rect.y = window_height - prompt_sprite_rect.h - 10;
    SDL_RenderCopy(renderer, prompt_sprite_textures[prompt_sprite_texture_index], NULL, &prompt_sprite_rect);
    SDL_RenderCopy(renderer, message_texture, NULL, &message_texture_rect);
    SDL_RenderCopy(renderer, icon, NULL, &icon_box_rect);
    SDL_RenderCopy(renderer, hint_texture, NULL, &hint_texture_rect);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyTexture(message_texture);
  prompt_sprite_texture_animation->Halt();
}

void Holes::PromptPlayerWithTextEntry(std::string message, std::string &entry) {
  bool closed = false;
  StartAnimation();
  SDL_StartTextInput();
  while (!closed) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        app->state = APP_STATE_QUIT;
        closed = true;
      }
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN) {
          closed = true;
        } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
          if (!entry.empty()) entry.pop_back();
        } else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
          SDL_SetClipboardText(entry.c_str());
        } else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
          entry = SDL_GetClipboardText();
        }
      }
      if (event.type == SDL_TEXTINPUT) {
        entry += event.text.text;
      }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.type == SDL_WINDOWEVENT_RESIZED) {
          SDL_GetWindowSize(window, &window_width, &window_height);
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);
    SDL_Texture* message_texture = ResLoad::RenderText(inventoryFont, message, {255, 255, 255} ,window_width - 200),
      *entry_texture = ResLoad::RenderText(inventoryFont, entry, {0, 0, 0} ,window_width - 200);
    SDL_Rect message_texture_rect, entry_texture_rect;
    SDL_QueryTexture(message_texture, NULL, NULL, &message_texture_rect.w, &message_texture_rect.h);
    SDL_QueryTexture(entry_texture, NULL, NULL, &entry_texture_rect.w, &entry_texture_rect.h);
    message_texture_rect.x = (window_width - message_texture_rect.w)/2;
    message_texture_rect.y = (window_height - message_texture_rect.h)/2 - 50;
    entry_texture_rect.x = (window_width - entry_texture_rect.w)/2;
    entry_texture_rect.y = message_texture_rect.y + message_texture_rect.h + 20;

    SDL_Rect entry_bg_rect = {entry_texture_rect.x - 5, entry_texture_rect.y - 5, entry_texture_rect.w + 10, entry_texture_rect.h + 10};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &entry_bg_rect);

    SDL_Rect prompt_sprite_rect = {window_width - 100, window_height - 100, 50, 50};
    prompt_sprite_texture_index = std::clamp(prompt_sprite_texture_index, 0, static_cast<int>(prompt_sprite_textures.size() - 1));
    prompt_sprite_rect.y = window_height - prompt_sprite_rect.h - 10;
    SDL_RenderCopy(renderer, prompt_sprite_textures[prompt_sprite_texture_index], NULL, &prompt_sprite_rect);

    SDL_RenderCopy(renderer, message_texture, NULL, &message_texture_rect);
    SDL_RenderCopy(renderer, entry_texture, NULL, &entry_texture_rect);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(message_texture);
    SDL_DestroyTexture(entry_texture);
  }
  SDL_StopTextInput();
  prompt_sprite_texture_animation->Halt();
}
