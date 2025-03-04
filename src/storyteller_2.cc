#include <humanoid.h>
#include <base.h>
#include <resload.h>
#include <objective.h>
#include <items.h>
#ifndef EMPTY_FUNCTION
#define EMPTY_FUNCTION [](std::string) {}
#endif
StoryTeller_2::StoryTeller_2() {
  name = "Storyteller";
  rect = { -275, -275, 64, 64 };
  texture_path = "assets/storyteller.png";
  messages = {
    {"Hey, I'm back again.", EMPTY_FUNCTION},
    {"This time, I'm here to tell you about the pendant you found.", EMPTY_FUNCTION},
    {"The pendant you found belonged to Kate Barlow, a school teacher who lived in the town of Green Lake.", EMPTY_FUNCTION},
    {"She was a kind school teacher who fell in love with a black man, causing the entire town to dislike her.", EMPTY_FUNCTION},
    {"She was exiled, and did not take it well. She became a bandit, robbing the townspeople.", EMPTY_FUNCTION},
    {"She would rob many banks and citizens, including Elya Yelnats.", EMPTY_FUNCTION},
    {"Elya Yelnats was a young man from Latvia who was in love with a dumb woman named Myra.", EMPTY_FUNCTION},
    {"Elya did not have the looks or the charm to win Myra's heart, and he went to America to seek his fortune.", EMPTY_FUNCTION},
    {"He was robbed by Kate Barlow, and she took his suitcase, but miraculously spared him.", EMPTY_FUNCTION},
    {"And here's the kicker: Elya Yelnats is the great-great-grandfather of Stanley Yelnats who now has a curse for not completing his chores.", EMPTY_FUNCTION},
    {"And that's it for now. You can meet me again using the story item.", [](std::string) {
      player->inventory.push_back(new Story_KB());
      Holes::PromptPlayerWithIconBox("You received a story item!", player->inventory.back()->sprite);
      player->inventory.push_back(new Story_EY());
      Holes::PromptPlayerWithIconBox("You received a story item!", player->inventory.back()->sprite);
      Holes::SetCurrentObjective({"Storyteller's Words - Complete!", "You received a story item."});
    }}
  };
  interacted = false;
}

void StoryTeller_2::Start() {
  texture = ResLoad::LoadImage("assets/storyteller.png");
}

void StoryTeller_2::Update() {
  if (interacted) {
    enabled = false;
    std::cout << "ADJFOADJFADLSJFKLDSJFLDAKSFJLAKDSFJDLSKFJAL";
  }
  Humanoid::Update();
}

void StoryTeller_2::Quit() {
  ResLoad::FreeImage(texture);
}
