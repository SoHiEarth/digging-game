#ifndef HOLES_CAMP_H
#define HOLES_CAMP_H
#include <SDL_blendmode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <base.h>
#include <object.h>
#include <resload.h>
#include <objective.h>
#include <renderer_temp.h>
namespace Holes {
  struct Camp_2_In : public Object {
    Camp_2_In();
    void Start() override;
    void Update() override;
    void Quit() override;
  };
  struct Camp_2 : public Object {
    Camp_2 ();
    void Start() override;
    void Update() override;
    void Quit() override;
  };
}

#endif // HOLES_CAMP_H
