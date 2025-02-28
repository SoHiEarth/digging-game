#ifndef HOLES_2_MANAGER_H
#define HOLES_2_MANAGER_H

#include <object.h>
#include <animate.h>
#include "objective.h"
struct Holes_2_Manager : public Object {
  Animator_Brightness anim_bright;
  void Start() override {
    anim_bright.LoadAnimation("assets/2_b.anim");
    anim_bright.Play();
    Holes::SetCurrentObjective({"Greetings!", "Speak with Mr. Sir."});
  }
};

#endif // !HOLES_2_MANAGER_H
