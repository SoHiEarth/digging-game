#include <base.h>
#include <humanoid.h>
#include <resload.h>
#include <string>
#include "objective.h"
#include <prompt.h>
#ifndef EMPTY_FUNCTION
#define EMPTY_FUNCTION [](std::string) {}
#endif
SDL_Texture *pendant = nullptr;
bool story_added = false, pendant_added = false;
StoryTeller::StoryTeller() {
  name = "Mysterious Figure";
  rect = {-275, -275, 64, 64};
  texture_path = "assets/storyteller.png";
  messages = {
    {"Sorry to surprise ya, " + player->GetPlayerName() + ". I'm the storyteller, and I'm here to guide you through the remainder of this story.", [](std::string) {
      current_humanoid->name = "Storyteller";
    }},
    {"This is due to this game being a work in progress, continuously being updated and improved.", EMPTY_FUNCTION},
    {"I'm here to tell you the story of Stanley Yelnats, a young lad who you just controlled.", EMPTY_FUNCTION},
    {"Stanley was sent to a juvenile detention center, Camp Green Lake, for a crime he didn't commit.", EMPTY_FUNCTION},
    {"So it continued, one day after another, digging holes in the scorching heat.", EMPTY_FUNCTION},
    {"One day, Stanley noticed that he'd dug up a very funny looking object.", [](std::string) {
      Holes::PromptPlayerWithIconBox("The mysterious item in question:", pendant);
    }},
    {"It was a pendant, with the initials KB engraved on it.", EMPTY_FUNCTION},
    {"Stanley knew that this pendant was special, and he decided to keep it.", EMPTY_FUNCTION},
    {"The pendant was a key to a mystery that would change Stanley's life forever.", EMPTY_FUNCTION},
    {"And that's where our story ends for now. I'll be back to guide you through the rest of the story.", EMPTY_FUNCTION},
    {"Goodbye for now, " + player->GetPlayerName() + ".", [](std::string) {
      current_humanoid->name = "Mysterious Figure";
      if (!pendant_added) {
        player->inventory.push_back(new Pendant());
        Holes::PromptPlayerWithIconBox("You received a pendant!", pendant);
        pendant_added = true;
      }
      Holes::SetCurrentObjective({"Storyteller's Words - Complete!", "You received a pendant."});
    }},
    {"You can meet me again using the \"Intro\" story item", [](std::string) {
      if (!story_added) {
        player->inventory.push_back(new Story_Intro());
        Holes::PromptPlayerWithIconBox("You received a story item!", player->inventory.back()->sprite);
        story_added = true;
      }
      Holes::SetCurrentObjective({"Storyteller's Words - Complete!", "You received a story item."});
      Holes::SetCurrentObjective({"Time is now 8:00 PM", "You should head to your tent, eat and sleep."});
      global_brightness = 30;
    }}
  };
}

void StoryTeller::Start() {
  pendant = ResLoad::LoadImage("assets/pendant.png");
  Humanoid::Start();
}

void StoryTeller::Update() {
  if (interacted) {
    enabled = false;
  }
  Humanoid::Update();
}

void StoryTeller::Quit() {
  ResLoad::FreeImage(pendant);
  Humanoid::Quit();
}
