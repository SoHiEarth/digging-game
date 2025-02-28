#include <humanoid.h>
#include "objective.h"

Sir::Sir() {
  this->name = "Mr. Sir";
  this->messages.push_back("Stop Right There!");
  this->messages.push_back("You are not in the girl's scouts anymore.");
  this->messages.push_back("Go to Mr. Pendanski, the man in the white shirt.");
  this->rect = { 240, 200, 64, 64 };
  this->texture_path = "assets/sir.png";
}

void Sir::Update() {
  if (!interacted) {
    has_critical_update = true;
  }
  if (interacted && Holes::GetCurrentObjective().title == "Greetings!") {
    Holes::SetCurrentObjective({"Go to Mr. Pendanski", "Meet Mr. Pendanski, the man with the white shirt."});
  }
  Humanoid::Update();
}
