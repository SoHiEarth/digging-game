#include <humanoid.h>

Sir::Sir() {
  this->name = "Mr. Sir";
  this->messages.push_back("Stop Right There!");
  this->messages.push_back("You are not in the girl's scouts anymore.");
  this->rect = { 240, 200, 64, 64 };
  this->texture_path = "assets/sir.png";
}
