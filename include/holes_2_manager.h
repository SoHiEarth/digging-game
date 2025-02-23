#ifndef HOLES_2_MANAGER_H
#define HOLES_2_MANAGER_H

#include <object.h>
#include <animate.h>
struct Holes_2_Manager : public Object {
  Animator_Brightness anim_bright;
  void Start() override {
    anim_bright.LoadAnimation("assets/2_b.anim");
    anim_bright.Play();
  }
};

#endif // !HOLES_2_MANAGER_H
