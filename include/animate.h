#ifndef ANIMATE_HPP
#define ANIMATE_HPP

#include <SDL.h>
#include <string>
#include <vector>
#include <safe_thread.h>

struct AnimationFrame {
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
  virtual void PlayAnimation() = 0;
  std::string animation_source_file;
  ST animation_thread;
  bool is_playing = false;
  virtual void Play() = 0;
  virtual void LoadAnimation(const std::string& sourceFile) = 0;
};

class Animator_Color : public Animator {
  void PlayAnimation();
  public:
  SDL_Color& color_to_animate;
  Animator_Color(SDL_Color& color) : color_to_animate(color) {}
  void Play();
  void LoadAnimation(const std::string& sourceFile);
};

class Animator_Rect : public Animator {
  void PlayAnimation();
public:
  SDL_Rect& rect_to_animate;
  Animator_Rect(SDL_Rect& rect) : rect_to_animate(rect) {}

  void Play();
  void LoadAnimation(const std::string& sourceFile);
};

class Animator_Brightness : public Animator {
  void PlayAnimation();
public:
  void Play();
  void LoadAnimation(const std::string& sourceFile);
};

#endif // ANIMATE_HPP
