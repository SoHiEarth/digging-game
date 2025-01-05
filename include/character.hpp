#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include <animate.hpp>

struct HumanoidDialougeOnTriggerAnim {
  std::string dialouge;
  std::string animPath;

};

struct Humanoid {
  std::string characterName;
  std::vector<std::string> messages;

  SDL_Rect characterRect;
  std::string characterTexturePath;
  SDL_Texture* characterTexture = nullptr;
};

struct Sir : public Humanoid {
  Sir() {
    this->characterName = "Mr. Sir";
    this->messages.push_back("Stop Right There!");
    this->messages.push_back("You are not in the girl's scouts anymore.");
    this->characterRect = { 240, 200, 64, 64 };
    this->characterTexturePath = "assets/character.png";
  }

  ~Sir() {
    SDL_DestroyTexture(characterTexture);
  }
};

struct JailGuard : public Humanoid {
  JailGuard() {
    this->characterName = "Guard";
    this->messages.push_back("Stop Right There!");
    this->characterRect = { 100, 200, 64, 64 };
    this->characterTexturePath = "assets/character.png";
  }

  ~JailGuard() {
    SDL_DestroyTexture(characterTexture);
  }
};

struct TestHumanoid : public Humanoid {
  TestHumanoid() {
    this->characterName = "Test Humanoid";
    this->messages.push_back("Hello, I am a test humanoid.");
    this->messages.push_back("Test1");
    this->messages.push_back("Test2");
    this->messages.push_back("Test3\nWith Newline");

    this->characterRect = { 0, 0, 64, 64 };
    this->characterTexturePath = "assets/character.png";
  }

  ~TestHumanoid() {
    SDL_DestroyTexture(characterTexture);
  }
};

#endif // CHARACTER_HPP
