#include <humanoid.h>
#include <renderer_temp.h>
#include <resload.h>
#include <objective.h>
#include <holescan.h>
#include <prompt.h>
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
  this->messages.push_back({"Hey there, new kid.", [](std::string) {}});
  this->messages.push_back({"I'm Mr. Pendanski, the camp counselor.", [](std::string) {}});
  this->messages.push_back({"So, what's your name?",
      [](std::string name) {
      std::string player_ptr = player->GetPlayerName();
      Holes::PromptPlayerWithTextEntry("Set your name.", player_ptr);
      player->SetPlayerName(player_ptr);
      }});
  this->messages.push_back({"You're going to be here for a while, make yourself at \"home\"", [](std::string) {}});
  this->messages.push_back({"Follow me.", [](std::string) {
      pendanski_iteration = PENDANSKI_FOLLOW;
      }});
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
    has_critical_update = true;
    Holes::UnsetCurrentObjective();
  }
  switch (pendanski_iteration) {
  case PENDANSKI_WELCOME:
    this->messages.clear();
    this->messages.push_back({"You, " + player->GetPlayerName() + ", are now a part of the family.", [](std::string) {}});
    this->messages.push_back({"Welcome to Camp Green Lake.", [](std::string) {}});
    this->messages.push_back({"Here's your shovel and water bottle.", [](std::string) {
        player->inventory.push_back(new Shovel());
    if (player->inventory.back()->sprite == nullptr) {
      player->inventory.back()->sprite = ResLoad::LoadImage(player->inventory.back()->itemSpritePath);
    }
    Holes::PromptPlayerWithIconBox("You got a Shovel!", player->inventory.back()->sprite);
    player->inventory.push_back(new Bottle());
    if (player->inventory.back()->sprite == nullptr) {
      player->inventory.back()->sprite = ResLoad::LoadImage(player->inventory.back()->itemSpritePath);
    }
    Holes::PromptPlayerWithIconBox("You got a Bottle!", player->inventory.back()->sprite);
        }});
    this->messages.push_back({"You'll need them for the work here.", [](std::string) {}});
    this->messages.push_back({"Now, go and dig a hole. I will meet you back at camp when you're finished.", [](std::string) {
        Holes::SetCurrentObjective({"Dig a hole", "Dig one hole at the blue area by pressing [E].\n(P.S.) It's to the west."});
        level.AddObjectInLoop(new HoleDesignatedArea());
        pendanski_iteration = PENDANSKI_AWAIT;
    }});
    this->has_critical_update = true;
    this->interacted = false;
    pendanski_iteration = PENDANSKI_AWAIT;
    break;
  case PENDANSKI_AWAIT:
    if (!interacted) break;
    // If the player has interacted with Pendanski, then Pendanski will give the player an item
    this->messages.clear();
    pendanski_iteration = PENDANSKI_DONE;
    break;
  default:
    break;
  }

}
