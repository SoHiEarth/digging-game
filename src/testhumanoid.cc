#include <humanoid.h>

TestHumanoid::TestHumanoid() {
  this->humanoid_name = "Test Humanoid";
  this->messages.push_back("Test1");
  this->messages.push_back("Test2");
  this->messages.push_back("Test3\nWith Newline");

  this->rect = { 0, 0, 64, 64 };
  this->humanoid_texture_path = "assets/character.png";
}
