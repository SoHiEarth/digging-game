#include <SDL_render.h>
#include <hole.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <assetbundleloader.hpp>
#include <algorithm>
#include <iostream>

std::vector<Hole*> holesVec;
SDL_Texture* hole_unifiedTexture = nullptr;
SDL_Rect hole_unifiedRect = { 0, 0, 64, 64 };
void PreloadHoleTexture() {
  hole_unifiedTexture = IMG_LoadTexture(renderer, current_asset_bundle.HOLE_UNIFIED_TEXTURE_PATH.c_str());
  if (hole_unifiedTexture == nullptr) {
    throw std::runtime_error("Failed to load hole texture");
  }
  SDL_QueryTexture(hole_unifiedTexture, NULL, NULL, &hole_unifiedRect.w, &hole_unifiedRect.h);
}

SDL_Rect hole_progress_bar = {
  0,
  0,
  100,
  10
};
void RenderHole(Hole& hole) {
  if (hole_unifiedTexture == nullptr) {
    try {
      PreloadHoleTexture();
    } catch (const std::runtime_error& e) {
      std::cerr << "Failed to preload hole texture: " << e.what() << std::endl;
    }
  }

  SDL_Rect hole_src_rect = {
    0, 0,
    hole_unifiedRect.w,
    hole_unifiedRect.h * std::clamp(hole.hole_dig_progress, 1, 100)/100
  },
  hole_border_rect = {
    hole.holeRect.x - 2,
    hole.holeRect.y - 2,
    hole.holeRect.w + 4,
    hole.holeRect.h + 4
  };


  if (SDL_RenderCopy(
    renderer,
    hole_unifiedTexture,
    &hole_src_rect,
    &hole.holeRect
  ) != 0) {
    std::cerr << "Failed to render hole texture: " << SDL_GetError() << std::endl;
  }
  
  if (hole.hole_dig_progress < 100) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &hole_border_rect);
    SDL_Rect hole_progress_bar_bg = {
      hole.holeRect.x + (hole.holeRect.w)/2 - (hole_progress_bar.w/2),
      hole.holeRect.y - hole_progress_bar.h - 10,
      hole_progress_bar.w,
      hole_progress_bar.h
    },
    hole_progress_bar_content = {
      hole_progress_bar_bg.x,
      hole_progress_bar_bg.y,
      hole_progress_bar_bg.w * std::clamp(hole.hole_dig_progress, 1, 100)/100,
      hole_progress_bar_bg.h
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hole_progress_bar_bg);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &hole_progress_bar_content);
  }
}
