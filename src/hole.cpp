#include <SDL_render.h>
#include <hole.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <assetbundleloader.hpp>
#include <algorithm>

std::vector<Hole*> holesVec;
SDL_Texture* hole_unifiedTexture = nullptr;
void PreloadHoleTexture() {
  hole_unifiedTexture = IMG_LoadTexture(renderer, current_asset_bundle.HOLE_UNIFIED_TEXTURE_PATH.c_str());
}

SDL_Rect hole_progress_bar = {
  0,
  0,
  100,
  20
};
void RenderHole(Hole& hole) {
  SDL_Rect hole_src_rect = {
    0,
    0,
    static_cast<int>(hole.holeRect.w * std::clamp(hole.hole_dig_progress, 1, 100)/100),
    static_cast<int>(hole.holeRect.h * std::clamp(hole.hole_dig_progress, 1, 100)/100)
  },
  hole_progress_bar_bg = {
    hole.holeRect.x + hole_progress_bar.x,
    hole.holeRect.y + hole_progress_bar.y,
    hole_progress_bar.w,
    hole_progress_bar.h
  },
  hole_progress_bar_content = {
    hole_progress_bar_bg.x,
    hole_progress_bar_bg.y,
    hole_progress_bar_bg.w * std::clamp(hole.hole_dig_progress, 1, 100)/100,
    hole_progress_bar_bg.h
  };

  SDL_RenderCopy(
    renderer,
    hole_unifiedTexture,
    &hole_src_rect,
    &hole.holeRect
  );

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &hole_progress_bar_bg);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(renderer, &hole_progress_bar_content);
}
