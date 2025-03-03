#ifndef HOLES_CAMP_H
#define HOLES_CAMP_H
#include <object.h>
#include <resload.h>
struct Camp_1 : public Object {
  Animator_Brightness* animator_brightness;
  Camp_1() {
    rect = { 100, 100, 200, 200 };
    texture = ResLoad::LoadImage("assets/2_camp_out.png");
    animator_brightness = new Animator_Brightness();
  }
  void Update() override {
    if (SDL_HasIntersection(&rect, &player->rect)) {
      if (!animator_brightness->is_playing) {
      }
    }
  }
};

#endif // HOLES_CAMP_H
