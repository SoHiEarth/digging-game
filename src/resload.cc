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
std::map<std::string, SDL_Texture*> ResLoad::Internal::image_cache;
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
  SDL_Texture* texture;
  texture = IMG_LoadTexture(ResLoad::Internal::renderer,
      path.c_str());
  if (texture == nullptr) {
    ResLoad::Internal::log("Failed to load image. SDL: " + std::string(IMG_GetError()));
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Failed to load image.\nPath: " + path + "\nSDL: " + std::string(IMG_GetError()));
    }
  }
  return texture;
}

void ResLoad::FreeImage(SDL_Texture* texture) {
  for (auto it = ResLoad::Internal::image_cache.begin(); it != ResLoad::Internal::image_cache.end(); it++) {
    if (it->second == texture) {
      ResLoad::Internal::image_cache.erase(it);
      break;
    }
  }
  SDL_DestroyTexture(texture);
}

TTF_Font* ResLoad::LoadFont(const std::string &path, int size, bool fatal) {
  ResLoad::Internal::log("Loading Font");
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
  ResLoad::Internal::log("Font loaded");
  return font;
}

void ResLoad::FreeFont(TTF_Font* font) {
  for (auto it = ResLoad::Internal::font_cache.begin(); it != ResLoad::Internal::font_cache.end(); it++) {
    if (it->second == font) {
      ResLoad::Internal::font_cache.erase(it);
      break;
    }
  }
  TTF_CloseFont(font);
}

RESLOAD_API ResLoad::RenderText(TTF_Font *font, const std::string &text, SDL_Color color, int wrap, bool fatal) {
  if (ResLoad::Internal::CheckState() != 0) {
    ResLoad::Internal::log("Check for proper init failed.");
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Check for proper init failed.");
    }
    return nullptr;
  }
  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, wrap);
  if (surface == nullptr) {
    ResLoad::Internal::log("Failed to render text. SDL: " + std::string(TTF_GetError()));
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Failed to render text.\nSDL: " + std::string(TTF_GetError()));
    }
    return nullptr;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(ResLoad::Internal::renderer, surface);
  if (texture == nullptr) {
    ResLoad::Internal::log("Failed to create texture from surface. SDL: " + std::string(SDL_GetError()));
    if (fatal) {
      throw std::runtime_error("[RESLOAD] Failed to create texture from surface.\nSDL: " + std::string(SDL_GetError()));
    }
    return nullptr;
  }
  SDL_FreeSurface(surface);
  return texture;
}
