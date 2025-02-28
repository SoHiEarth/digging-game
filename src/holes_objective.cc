#include <SDL_render.h>
#include <objective.h>
#include <base.h>
#include <SDL.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
#include <resload.h>
#include <renderer_temp.h>
#include <animate.h>
Holes::Objective current_objective;
int objective_window_x_offset = 0;
bool objective_unset = false;
Animator_Int* slide_animation = nullptr;
void Holes::SetCurrentObjective(Objective objective) {
  current_objective = objective;
  objective_unset = false;
  if (slide_animation != nullptr) {
    slide_animation->Halt();
    delete slide_animation;
  }
  slide_animation = new Animator_Int(objective_window_x_offset);
  slide_animation->LoadAnimationByDelta("assets/objective.anim");
  slide_animation->Play();
}

Holes::Objective Holes::GetCurrentObjective() {
  return current_objective;
}

void Holes::UnsetCurrentObjective() {
  objective_unset = true;
}

void Holes::RenderObjective() {
  if (current_objective.title.empty() || objective_unset) return;
  SDL_Texture* title_texture = ResLoad::RenderText(inventoryFont, current_objective.title, {255, 255, 255, 255}, window_width * 0.35),
    *message_texture = ResLoad::RenderText(inventoryFont, current_objective.message, {255, 255, 255, 255}, window_width * 0.25);
  SDL_Rect title_texture_rect = { objective_window_x_offset + 10, 40, 0, 0 },
           message_texture_rect = { objective_window_x_offset + 10, 150, 0, 0 };
  SDL_QueryTexture(title_texture,
      NULL, NULL,
      &title_texture_rect.w, &title_texture_rect.h);
  SDL_QueryTexture(message_texture,
      NULL, NULL,
      &message_texture_rect.w, &message_texture_rect.h);
  message_texture_rect.y = title_texture_rect.y + title_texture_rect.h + 20;
  SDL_Rect bg_rect = { objective_window_x_offset, 30, std::max(title_texture_rect.w, message_texture_rect.w) + 20, message_texture_rect.y + message_texture_rect.h - 30 + 10 };
  roundedBoxRGBA(renderer,
      static_cast<Sint16>(bg_rect.x),
      static_cast<Sint16>(bg_rect.y),
      static_cast<Sint16>(bg_rect.x + bg_rect.w),
      static_cast<Sint16>(bg_rect.y + bg_rect.h),
      10, 0, 0, 0, 120);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawLine(renderer, objective_window_x_offset + 10, message_texture_rect.y - 10, 50 + title_texture_rect.w, message_texture_rect.y - 10);
  SDL_RenderCopy(renderer, title_texture, NULL, &title_texture_rect);
  SDL_RenderCopy(renderer, message_texture, NULL, &message_texture_rect);
  SDL_DestroyTexture(title_texture);
  SDL_DestroyTexture(message_texture);
}
