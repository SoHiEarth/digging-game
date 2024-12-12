#include <hole.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <assetbundleloader.hpp>

std::vector<Hole*> holesVec;
SDL_Texture* hole_unifiedTexture = nullptr;
void PreloadHoleTexture() {
  hole_unifiedTexture = IMG_LoadTexture(renderer, assetBundle.HOLE_UNIFIED_TEXTURE_PATH.c_str());
}
