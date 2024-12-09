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
  SDL_Texture* characterTexture;
};

struct TestHumanoid : public Humanoid {
  TestHumanoid() {
    characterName = "Test Humanoid";
    messages = {
      "Hello, I am a test humanoid.",
      "I am here to help you.",
      "I am a test humanoid."
    };

    characterRect = { 0, 0, 64, 64 };
    characterTexturePath = "assets/character.png";
  }
};

#endif // CHARACTER_HPP
