#ifndef RESLOAD_H
#define RESLOAD_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>
#include <functional>
#define RESLOAD_API SDL_Texture*
namespace ResLoad {
  void SetRenderer(SDL_Renderer* renderer);
  void SetLogger(std::function<void(std::string)> logger);
  SDL_Texture* MakeTextureFromColor(int width, int height, SDL_Color color);
  // Loads an image using SDL_image.
  /* Param: Path: Path to the image to load. */
  /* Param: Fatal: Should the function throw an exception if 
     an error occured. */
  RESLOAD_API LoadImage(const std::string& path = "", bool fatal = false);
  void FreeImage(SDL_Texture* texture);
  TTF_Font* LoadFont(const std::string& path = "", int size = 18, bool fatal = false);
  void FreeFont(TTF_Font* font);
  RESLOAD_API RenderText(TTF_Font *font = nullptr, const std::string &text = "", SDL_Color color = {255, 255, 255, 255}, int wrap = 0, bool fatal = false);
  namespace Internal {
    extern std::map<std::string, TTF_Font*> font_cache;
    extern std::map<std::string, SDL_Texture*> image_cache;
    extern std::function<void(std::string)> log;
    extern SDL_Renderer* renderer;
    void default_logger(std::string message);
    // Returns 0 if everything is init.
    int CheckState();
  }
}
#endif // RESLOAD_H
