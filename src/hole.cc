#include <hole.h>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <assetbundleloader.h>
#include <algorithm>
#include <iostream>
#include <resload.h>

std::vector<Hole*> holes_vector;
SDL_Texture* hole_texture = nullptr;
SDL_Rect hole_texture_rect = { 0, 0, 64, 64 };
void LoadHoleTexture() {
  if (hole_texture != nullptr) {
    SDL_DestroyTexture(hole_texture);
  }
  auto hole_data = ResLoad::LoadImage(current_asset_bundle.HOLE_UNIFIED_TEXTURE_PATH);
  hole_texture = hole_data.texture;
  hole_texture_rect.w = hole_data.w;
  hole_texture_rect.h = hole_data.h;
}

vec2 hole_progress_bar = {100, 10};

void RenderHole(Hole& hole) {
  if (hole_texture == nullptr) {
    try {
      LoadHoleTexture();
    } catch (const std::runtime_error& e) {
      std::cerr << "Failed to preload hole texture: " << e.what() << std::endl;
    }
  }

  SDL_Rect hole_src_rect = {
    0, 0,
    hole_texture_rect.w,
    hole_texture_rect.h * std::clamp(hole.progress, 1, 100)/100
  },
  hole_border_rect = {
    hole.rect.x - 2,
    hole.rect.y - 2,
    hole.rect.w + 4,
    hole.rect.h + 4
  };


  if (SDL_RenderCopy(
    renderer,
    hole_texture,
    &hole_src_rect,
    &hole.rect
  ) != 0) {
    std::cerr << "Failed to render hole texture: " << SDL_GetError() << std::endl;
  }
  
  if (hole.progress < 100) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &hole_border_rect);
    SDL_Rect hole_progress_bar_bg = {
      hole.rect.x + (hole.rect.w)/2 - (hole_progress_bar.x/2),
      hole.rect.y - hole_progress_bar.y - 10,
      hole_progress_bar.x,
      hole_progress_bar.y
    },
    hole_progress_bar_content = {
      hole_progress_bar_bg.x,
      hole_progress_bar_bg.y,
      hole_progress_bar_bg.w * std::clamp(hole.progress, 1, 100)/100,
      hole_progress_bar_bg.h
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hole_progress_bar_bg);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &hole_progress_bar_content);
  }
}
