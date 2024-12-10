#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include <SDL.h>

struct Humanoid {
  std::string characterName; 
  std::vector<std::string> messages;

  SDL_Rect characterRect;
  std::string characterTexturePath;
  SDL_Texture* characterTexture = nullptr;
};

struct TestHumanoid : public Humanoid {
  TestHumanoid() {
    this->characterName = "Test Humanoid";
    this->messages.push_back("Hello, I am a test humanoid.");

    this->characterRect = { 0, 0, 64, 64 };
    this->characterTexturePath = "assets/character.png";
  }

  ~TestHumanoid() {
    SDL_DestroyTexture(characterTexture);
  }
};

#endif // CHARACTER_HPP
