#ifndef HOLESCAN_H
#define HOLESCAN_H
#include <object.h>
#include <hole.h>
#include <objective.h>
struct HoleDesignatedArea : public Object {
  bool objective_complete = false;
  HoleDesignatedArea() {
    rect = { -800, -400, 256, 256 };
  }
  void Update() override {
    if (objective_complete) return;
    for (auto* hole : holes_vector) {
      if (SDL_HasIntersection(&hole->rect, &player->rect) == SDL_TRUE) {
        if (hole->progress >= 100) {
          objective_complete = true;
          Holes::SetCurrentObjective({"Dig a Hole - Complete!", "Go to camp and spend the rest of your day."});
          StoryTeller* storyteller = new StoryTeller();
          storyteller->Start();
          level.AddObjectInLoop(storyteller);
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect_camera = GetRectInCameraSpace(this->rect);
    SDL_RenderDrawRect(renderer, &rect_camera);
  }
};

struct HoleDesignatedArea_2 : public Object {
  bool objective_complete = false;
  HoleDesignatedArea_2() {
    rect = { -800, -400, 256, 256 };
  }
  void Update() override {
    if (objective_complete) return;
    for (auto* hole : holes_vector) {
      if (SDL_HasIntersection(&hole->rect, &player->rect) == SDL_TRUE) {
        if (hole->progress >= 100) {
          objective_complete = true;
          Holes::SetCurrentObjective({"Dig a Hole (Day 2) - Complete!", "Go to camp and spend the rest of your day."});
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect_camera = GetRectInCameraSpace(this->rect);
    SDL_RenderDrawRect(renderer, &rect_camera);
  }
};

#endif // HOLESCAN_H
