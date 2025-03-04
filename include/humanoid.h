#ifndef HUMANOID_H
#define HUMANOID_H
#include <string>
#include <vector>
#include <functional>
#include <SDL.h>
#include <animate.h>
#include <object.h>
struct HumanoidDialougeOnTriggerAnim {
  std::string dialouge;
  std::string animPath;
};

struct Humanoid : public Object{
  std::string name;
  std::vector<std::pair<std::string, std::function<void(std::string)>>> messages;
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

struct Pendanski_2 : public Pendanski {
  void Update() override;
  Pendanski_2();
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

struct StoryTeller : public Humanoid {
  void Start() override;
  void Update() override;
  void Quit() override;
  StoryTeller();
};

struct StoryTeller_2 : public Humanoid {
  void Start() override;
  void Update() override;
  void Quit() override;
  StoryTeller_2();
};
#endif // HUMANOID_H
