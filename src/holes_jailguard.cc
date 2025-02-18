#include <humanoid.h>

JailGuard::JailGuard() {
  this->humanoid_name = "Guard";
  this->messages.push_back("Stop Right There!");
  this->rect = { 100, 200, 64, 64 };
  this->humanoid_texture_path = "assets/character.png";
}
