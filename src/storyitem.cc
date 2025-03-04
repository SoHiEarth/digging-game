#include <items.h>
#include <humanoid.h>
#include <base.h>
#include <application.h>
StoryItem::StoryItem() {
  itemName = "Story Item";
  itemDescription = "???";
  itemSpritePath = "assets/shovel.png";
}

void StoryItem::func() {
  current_humanoid = new StoryTeller();
  current_humanoid->Start();
  app->state = ApplicationState::APP_STATE_DIALOGUE;
}

StoryItem_2::StoryItem_2() {
  itemName = "Story Item";
  itemDescription = "???";
  itemSpritePath = "assets/shovel.png";
}

void StoryItem_2::func() {
  current_humanoid = new StoryTeller_2();
  current_humanoid->Start();
  app->state = APP_STATE_DIALOGUE;
}
