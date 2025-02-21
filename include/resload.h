#ifndef RESLOAD_H
#define RESLOAD_H

#include <SDL.h>
#include <map>
#include <string>
#include <functional>

#define RESLOAD_API static ResLoad::TextureData

namespace ResLoad {
  namespace Internal {
    extern std::map<std::string, SDL_Texture*> texture_cache;
    extern std::function<void(std::string)> log;
    extern SDL_Renderer* renderer;
    void default_logger(std::string message);
    // Returns 0 if everything is init.
    int CheckState();
  }

  struct TextureData {
    SDL_Texture* texture;
    int w;
    int h;
  };

  void SetRenderer(SDL_Renderer* renderer);
  void SetLogger(std::function<void(std::string)> logger);
  // Loads an image using SDL_image.
  /* Param: Path: Path to the image to load. */
  /* Param: Fatal: Should the function throw an exception if 
     an error occured. */
  RESLOAD_API LoadImage(const std::string& path, bool fatal = false);
}

#endif // RESLOAD_H
