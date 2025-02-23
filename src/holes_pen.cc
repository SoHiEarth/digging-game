#include <humanoid.h>

Pendanski::Pendanski() {
  this->name = "Mr. Pendanski";
  this->messages.push_back("Follow me.");
  this->rect = { 100, 100, 64, 64 };
  this->texture_path = "assets/pen.png";
}
