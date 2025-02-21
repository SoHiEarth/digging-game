#include <humanoid.h>

JailGuard::JailGuard() {
  this->name = "Guard";
  this->messages.push_back("Stop Right There!");
  this->rect = { 100, 200, 64, 64 };
  this->texture_path = "assets/character.png";
}
