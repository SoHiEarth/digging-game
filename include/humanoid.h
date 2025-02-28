#ifndef HUMANOID_H
#define HUMANOID_H
#include <string>
#include <vector>
#include <SDL.h>
#include <animate.h>
#include <object.h>
struct HumanoidDialougeOnTriggerAnim {
  std::string dialouge;
  std::string animPath;
};

struct Humanoid : public Object{
  std::string name;
  std::vector<std::string> messages;
  bool interacted = false, has_critical_update = false;
  std::string texture_path;
  ~Humanoid();
  void Start() override;
  void Fixed() override;
  void Update() override;
};

struct Sir : public Humanoid {
  Sir();
  void Update() override;
};

struct Pendanski : public Humanoid {
  Animator_Rect* animator_rect;
  void Update() override;
  Pendanski();
};

struct JailGuard : public Humanoid {
  JailGuard();
};

struct Warden : public Humanoid {
  Warden();
};

struct Zero : public Humanoid {
  Zero();
};
#endif // HUMANOID_H
