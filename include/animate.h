#ifndef ANIMATE_HPP
#define ANIMATE_HPP

#include <SDL.h>
#include <string>
#include <vector>
#include <safe_thread.h>
#include <atomic>
struct AnimationFrame {
  float iOffset = 0;
  int xOffset = 0;
  int yOffset = 0;
  int wOffset = 0;
  int hOffset = 0;
  int rOffset = 0;
  int gOffset = 0;
  int bOffset = 0;
  int aOffset = 0;
  int timeUntilNextFrame = 1;
};

class Animator {
 protected:
  std::vector<AnimationFrame> frames{};
  virtual void PlayAnimation(std::atomic<bool>& running) = 0;
  std::string animation_source_file;
  ST animation_thread;
 public:
  bool is_playing = false;
  bool repeat = false;
  std::string custom_attrib = "";
  std::atomic<int> play_count_since_start = 0;
  virtual void Play() = 0;
  void Halt() {
    is_playing = false;
    animation_thread.Close();
  }
  virtual void LoadAnimation(const std::string& source_file) = 0;
};

class Animator_Color : public Animator {
  void PlayAnimation(std::atomic<bool>& running);
 public:
  SDL_Color& color_to_animate;
  Animator_Color(SDL_Color& color) : color_to_animate(color) {}
  void Play();
  void LoadAnimation(const std::string& source_file);
};

class Animator_Int : public Animator {
  void PlayAnimation(std::atomic<bool>& running);
 public:
  int& int_to_animate;
  Animator_Int(int& integer) : int_to_animate(integer) {}
  void Play();
  void LoadAnimation(const std::string& source_file);
  void LoadAnimationByDelta(const std::string& source_file);
};

class Animator_Rect : public Animator {
  void PlayAnimation(std::atomic<bool>& running);
 public:
  SDL_Rect& rect_to_animate;
  Animator_Rect(SDL_Rect& rect) : rect_to_animate(rect) {}

  void Play();
  void LoadAnimation(const std::string& sourceFile);
  void LoadAnimationByDelta(const std::string& sourceFile);
};

class Animator_Brightness : public Animator {
  void PlayAnimation(std::atomic<bool>& running);
public:
  void Play();
  void LoadAnimation(const std::string& source_file);
};
#endif // ANIMATE_HPP
