#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <object.h>
// A level is a basically a list of objects
// that are institated in the game world when
// the level is loaded, including
// humanoids, interactables, and the player.
class Level {
 public:
  std::vector<Object*> objects;
  void Load(std::string path);
};

#endif // LEVEL_H
