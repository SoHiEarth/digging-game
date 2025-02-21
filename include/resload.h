#ifndef RESLOAD_H
#define RESLOAD_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>
#include <functional>

#define RESLOAD_API ResLoad::TextureData

namespace ResLoad {
  struct TextureData {
    SDL_Texture* texture;
    int w;
    int h;
    TextureData(SDL_Texture* texture = nullptr, int w = 0, int h = 0) : texture(texture), w(w), h(h) {}
    std::string path;
  };
  void SetRenderer(SDL_Renderer* renderer);
  void SetLogger(std::function<void(std::string)> logger);
  void DestroyData(ResLoad::TextureData data);
  // Loads an image using SDL_image.
  /* Param: Path: Path to the image to load. */
  /* Param: Fatal: Should the function throw an exception if 
     an error occured. */
  RESLOAD_API LoadImage(const std::string& path = "", bool fatal = false);
  TTF_Font* LoadFont(const std::string& path = "", int size = 18, bool fatal = false);
  RESLOAD_API RenderText(TTF_Font *font = nullptr, const std::string &text = "", SDL_Color color = {255, 255, 255, 255}, int wrap = 0, bool fatal = false);
  namespace Internal {
    extern std::map<std::string, ResLoad::TextureData> image_cache;
    extern std::map<std::string, TTF_Font*> font_cache;
    extern std::function<void(std::string)> log;
    extern SDL_Renderer* renderer;
    void default_logger(std::string message);
    // Returns 0 if everything is init.
    int CheckState();
  }
}

#endif // RESLOAD_H
