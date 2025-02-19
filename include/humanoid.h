#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include <animate.hpp>
#include <object.h>
struct HumanoidDialougeOnTriggerAnim {
  std::string dialouge;
  std::string animPath;
};

struct Humanoid : public Object{
  std::string humanoid_name;
  std::vector<std::string> messages;
  std::string humanoid_texture_path;
  SDL_Texture* humanoid_texture = nullptr;
  ~Humanoid();
  void Start() override;
  void Update() override;
};

struct Sir : public Humanoid {
  Sir();
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

struct TestHumanoid : public Humanoid {
  TestHumanoid();
};

#endif // CHARACTER_HPP
