#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <object.h>

struct MapPart : public Object {
 public:
  std::string path;
  MapPart(std::string path) : path(path) {}
  void Start() override;
  void Update() override;
  void Fixed() override;
  void Quit() override;
};

namespace Holes {
  namespace Map {
    struct Hill : public MapPart {
      Hill() : MapPart("assets/hill.png") {}
    };
  }
}

#endif // MAP_H
