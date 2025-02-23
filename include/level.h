#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <object.h>
#include <safe_thread.h>
// A level is a basically a list of objects
// that are institated in the game world when
// the level is loaded, including
// humanoids, interactables, and the player.
class Level {
 private:
   std::string next_frame_path;
 public:
  bool loaded = false;
  ST fixed_thread;
  std::vector<Object*> objects;
  void LoadAtNextFrame(std::string path); // For calling Load while updating.
                        // This is to avoid loading in the middle of a frame,
                        // which can cause segfaults.
  bool HasNextFramePath();
  void LoadNextFramePath();
  void Load(std::string path);
  void Unload();
};

#endif // LEVEL_H
