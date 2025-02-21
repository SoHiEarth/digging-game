#include <SDL_render.h>
#include <resload.h>

#include <stdexcept>
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <filesystem>
#include <SDL.h>
#include <SDL_image.h>

void ResLoad::Internal::default_logger(std::string message) {
  std::cout << "[RESLOAD]: " << message << std::endl;
}
std::function<void(std::string)> ResLoad::Internal::log = std::bind(ResLoad::Internal::default_logger, std::placeholders::_1);
std::map<std::string, SDL_Texture*> ResLoad::Internal::texture_cache;
SDL_Renderer* ResLoad::Internal::renderer;

int ResLoad::Internal::CheckState() {
  if (renderer == nullptr) {
    return 1;
  }
  return 0;
}

void ResLoad::SetRenderer(SDL_Renderer* renderer) {
  ResLoad::Internal::renderer = renderer;
}

void ResLoad::SetLogger(std::function<void(std::string)> logger) {
  ResLoad::Internal::log = std::bind(logger, std::placeholders::_1);
}

RESLOAD_API ResLoad::LoadImage(const std::string &path, bool fatal) {
  if (ResLoad::Internal::CheckState() != 0) {
    ResLoad::Internal::log("[RESLOAD] Check for proper init failed.");
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Check for proper init failed.");
    }
  }
  if (!std::filesystem::exists(path)) {
    ResLoad::Internal::log("[RESLOAD] Image doesn't exist. Following errors may be related to this error. Path: " + path);
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Image doesn't exist. Path: " + path);
    }
  }
  ResLoad::TextureData texture_data = {};
  texture_data.texture = IMG_LoadTexture(ResLoad::Internal::renderer,
      path.c_str());
  if (texture_data.texture == nullptr) {
    ResLoad::Internal::log("Failed to load image. SDL: " + std::string(IMG_GetError()));
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Failed to load image.\nPath: " + path + "\nSDL: " + std::string(IMG_GetError()));
    }
  }
  if (SDL_QueryTexture(texture_data.texture,
    NULL,
    NULL,
    &texture_data.w,
    &texture_data.h) != 0) {
    ResLoad::Internal::log("Failed to query image.\nPath: " + path + "\nSDL: " + std::string(IMG_GetError()));
  }
  return texture_data;
}
