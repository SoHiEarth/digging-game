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
std::map<std::string, ResLoad::TextureData> ResLoad::Internal::image_cache;
std::map<std::string, TTF_Font*> ResLoad::Internal::font_cache;
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

void ResLoad::DestroyData(ResLoad::TextureData data) {
  SDL_DestroyTexture(data.texture);
  if (Internal::image_cache.find(data.path) != Internal::image_cache.end()) {
    Internal::image_cache.erase(data.path);
  }
}

RESLOAD_API ResLoad::LoadImage(const std::string &path, bool fatal) {
  if (ResLoad::Internal::CheckState() != 0) {
    ResLoad::Internal::log("Check for proper init failed.");
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Check for proper init failed.");
    }
  }
  if (!std::filesystem::exists(path)) {
    ResLoad::Internal::log("Image doesn't exist. Following errors may be related to this error. Path: " + path);
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Image doesn't exist. Path: " + path);
    }
  }
  ResLoad::TextureData texture_data = {};
  texture_data.path = path;
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

TTF_Font* ResLoad::LoadFont(const std::string &path, int size, bool fatal) {
  if (ResLoad::Internal::CheckState() != 0) {
    ResLoad::Internal::log("Check for proper init failed.");
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Check for proper init failed.");
    }  
  }
  if (ResLoad::Internal::font_cache.find(path) != ResLoad::Internal::font_cache.end()) {
    return ResLoad::Internal::font_cache[path];
  }
  if (!std::filesystem::exists(path)) {
    ResLoad::Internal::log("Font doesn't exist. Path: " + path);
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Font doesn't exist. Path: " + path);
    }
  }
  TTF_Font* font = TTF_OpenFont(path.c_str(), size);
  if (font == nullptr) {
    ResLoad::Internal::log("Failed to load font. SDL: " + std::string(TTF_GetError()));
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Failed to load font.\nPath: " + path + "\nSDL: " + std::string(TTF_GetError()));
    }
  }
  ResLoad::Internal::font_cache[path] = font;
  return font;
}

RESLOAD_API ResLoad::RenderText(TTF_Font *font, const std::string &text, SDL_Color color, int wrap, bool fatal) {
  if (ResLoad::Internal::CheckState() != 0) {
    ResLoad::Internal::log("Check for proper init failed.");
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Check for proper init failed.");
    }
    return {nullptr, 0, 0};
  }
  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, wrap);
  if (surface == nullptr) {
    ResLoad::Internal::log("Failed to render text. SDL: " + std::string(TTF_GetError()));
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Failed to render text.\nSDL: " + std::string(TTF_GetError()));
    }
    return {nullptr, 0, 0};
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(ResLoad::Internal::renderer, surface);
  if (texture == nullptr) {
    ResLoad::Internal::log("Failed to create texture from surface. SDL: " + std::string(SDL_GetError()));
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Failed to create texture from surface.\nSDL: " + std::string(SDL_GetError()));
    }
    return {nullptr, 0, 0};
  }
  SDL_FreeSurface(surface);
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);
  return {texture, w, h};
}
