#include <humanoid.h>
#include <renderer_temp.h>
#include <resload.h>
#include <objective.h>
#define CAN_SPEAK interacted && !animator_rect->is_playing

enum PENDANSKI_ITERATION {
  PENDANSKI_FOLLOW,
  PENDANSKI_WELCOME,
  PENDANSKI_AWAIT,
  PENDANSKI_ITEM,
  PENDANSKI_DONE
};

PENDANSKI_ITERATION pendanski_iteration = PENDANSKI_FOLLOW;

Pendanski::Pendanski() {
  this->name = "Mr. Pendanski";
  this->messages.push_back("Follow me.");
  this->rect = { 100, 100, 64, 64 };
  this->texture_path = "assets/pen.png";
  animator_rect = new Animator_Rect(rect);
  animator_rect->LoadAnimationByDelta("assets/2_pen_follow.anim");
}

void Pendanski::Update() {
  Humanoid::Update();
  if (CAN_SPEAK && animator_rect->play_count_since_start == 0 && pendanski_iteration == PENDANSKI_FOLLOW) {
    animator_rect->Play();
    Holes::Objective objective = { "Pendanski", "Follow Pendanski" };
    Holes::SetCurrentObjective(objective);
  }
  if (CAN_SPEAK && animator_rect->play_count_since_start == 1 && pendanski_iteration == PENDANSKI_FOLLOW) {
    pendanski_iteration = PENDANSKI_WELCOME;
    interacted = false;
    Holes::UnsetCurrentObjective();
  }
  switch (pendanski_iteration) {
  case PENDANSKI_WELCOME:
    this->messages.clear();
    this->messages.push_back("You are now a part of the family.");
    this->messages.push_back("Welcome to Camp Green Lake.");
    this->messages.push_back("Here's your shovel and water bottle.");
    this->has_critical_update = true;
    this->interacted = false;
    pendanski_iteration = PENDANSKI_AWAIT;
    break;
  case PENDANSKI_AWAIT:
    if (!interacted) break;
    // If the player has interacted with Pendanski, then Pendanski will give the player an item
    player.inventory.push_back(new Shovel());
    player.inventory.push_back(new Bottle());
    this->messages.clear();
    pendanski_iteration = PENDANSKI_DONE;
    break;
  default:
    break;
  }
  
}
