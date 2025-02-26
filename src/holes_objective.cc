#include <SDL_render.h>
#include <objective.h>
#include <base.h>
#include <SDL.h>
#include <resload.h>
#include <renderer_temp.h>
Holes::Objective current_objective;
bool objective_unset = false;
void Holes::SetCurrentObjective(Objective objective) {
  current_objective = objective;
  objective_unset = false;
}

void Holes::UnsetCurrentObjective() {
  objective_unset = true;
}

void Holes::RenderObjective() {
  if (current_objective.title.empty() || objective_unset) return;
  SDL_Texture* title_texture = ResLoad::RenderText(inventoryFont, current_objective.title, {255, 255, 255, 255}, window_width * 0.35),
    *message_texture = ResLoad::RenderText(inventoryFont, current_objective.message, {255, 255, 255, 255}, window_width * 0.25);
  SDL_Rect title_texture_rect = { 50, 100, 0, 0 },
           message_texture_rect = { 50, 150, 0, 0 };
  SDL_QueryTexture(title_texture,
      NULL, NULL,
      &title_texture_rect.w, &title_texture_rect.h);
  SDL_QueryTexture(message_texture,
      NULL, NULL,
      &message_texture_rect.w, &message_texture_rect.h);
  message_texture_rect.y = title_texture_rect.y + title_texture_rect.h + 20;
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawLine(renderer, 50, message_texture_rect.y - 10, 50 + title_texture_rect.w, message_texture_rect.y - 10);
  SDL_RenderCopy(renderer, title_texture, NULL, &title_texture_rect);
  SDL_RenderCopy(renderer, message_texture, NULL, &message_texture_rect);
  SDL_DestroyTexture(title_texture);
  SDL_DestroyTexture(message_texture);
}
