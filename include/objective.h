#ifndef OBJECTIVE_H
#define OBJECTIVE_H
#include <string>
namespace Holes {
  struct Objective {
    std::string title,
      message;
  };
  void SetCurrentObjective(Objective objective);
  Objective GetCurrentObjective();
  void UnsetCurrentObjective();
  void RenderObjective();
};
#endif // OBJECTIVE_H
