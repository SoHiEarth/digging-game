#include <SDL_render.h>
#include <hole.h>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <assetbundleloader.h>
#include <algorithm>
#include <resload.h>

std::vector<Hole*> holes_vector;
SDL_Texture* hole_texture = nullptr;
SDL_Rect hole_texture_rect = { 0, 0, 64, 64 };

void Hole::Update() {
  if (progress >= 100) {
    enabled = false;
  }
}

void LoadHoleTexture() {
  if (hole_texture != nullptr) {
    SDL_DestroyTexture(hole_texture);
  }
  hole_texture = ResLoad::LoadImage(current_asset_bundle.HOLE_UNIFIED_TEXTURE_PATH);
  SDL_QueryTexture(hole_texture, NULL, NULL, &hole_texture_rect.w, &hole_texture_rect.h);
}
vec2 hole_progress_bar = {100, 10};

void RenderHole(Hole* hole, SDL_Rect hole_rect) {
  if (hole_texture == nullptr) {
    LoadHoleTexture();
  }

  SDL_Rect hole_border_rect = {
    hole_rect.x - 5,
    hole_rect.y - 5,
    hole_rect.w + 10,
    hole_rect.h + 10
  };

  if (hole->progress < 100) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &hole_border_rect);
    SDL_Rect hole_progress_bar_bg = {
      hole_rect.x + (hole_rect.w)/2 - (hole_progress_bar.x/2),
      hole_rect.y - hole_progress_bar.y - 10,
      hole_progress_bar.x,
      hole_progress_bar.y
    },
    hole_progress_bar_content = {
      hole_progress_bar_bg.x,
      hole_progress_bar_bg.y,
      hole_progress_bar_bg.w * std::clamp(hole->progress, 1, 100)/100,
      hole_progress_bar_bg.h
    };
    hole->extras.insert({ResLoad::MakeTextureFromColor(hole_progress_bar_bg.w, hole_progress_bar_bg.h, {0, 0, 0}), hole_progress_bar_bg});
    hole->extras.insert({ResLoad::MakeTextureFromColor(hole_progress_bar_content.w, hole_progress_bar_content.h, {0, 255, 0}), hole_progress_bar_content});
  }
}
