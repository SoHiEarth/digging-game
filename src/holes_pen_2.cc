#include <humanoid.h>
#include <holescan.h>
#include <base.h>
#include <objective.h>
Pendanski_2::Pendanski_2() {
  name = "Mr. Pendanski";
  rect = { 100, 100, 64, 64 };
  texture_path = "assets/pen.png";
  messages = {
    {"Alright, the time is 7:00 AM, you are going to dig another hole today.", [](std::string){}},
    {"Go dig another hole in the designated location, just like you did yesterday.", [](std::string){}},
    {"If you don't, you will be punished. Now go dig that hole.", [](std::string){
      Holes::SetCurrentObjective({"Dig a Hole (Day 2) - In Progress", "Go dig a hole in the designated location."});
      level.AddObjectInLoop(new HoleDesignatedArea_2());
    }}
  };
}

void Pendanski_2::Update() {
  Humanoid::Update();
  if (interacted) {
    interacted = false;
    has_critical_update = true;
    Holes::UnsetCurrentObjective();
    level.AddObjectInLoop(new StoryTeller_2());
  }
}
