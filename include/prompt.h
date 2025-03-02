#ifndef PROMPT_H
#define PROMPT_H
#include <string>
#include <SDL.h>
namespace Holes {
  void PromptPlayerWithMessage(std::string message);
  void PromptPlayerWithIconBox(std::string message, SDL_Texture* icon);
  void PromptPlayerWithTextEntry(std::string message, std::string& entry);
}

#endif // PROMPT_H
