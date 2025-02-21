#ifndef ANIMATE_HPP
#define ANIMATE_HPP

#include <SDL.h>
#include <string>
#include <vector>
#include <thread>

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
  bool isPlaying = false;
  virtual void Play() = 0;
  virtual void LoadAnimation(const std::string& sourceFile) = 0;
};

class Animator_Color : public Animator {
  void PlayAnimation();
  public:
  std::string AnimationSourceFile;
  SDL_Color& colorToAnimate;
  std::thread animationThread;
  Animator_Color(SDL_Color& color) : colorToAnimate(color) {}
  void Play();
  void LoadAnimation(const std::string& sourceFile);
};

class Animator_Rect : public Animator {
  void PlayAnimation();
public:
  std::string AnimationSourceFile;
  SDL_Rect& rectToAnimate;
  std::thread animationThread;
  Animator_Rect(SDL_Rect& rect) : rectToAnimate(rect) {}

  void Play();
  void LoadAnimation(const std::string& sourceFile);
};

class Animator_Brightness : public Animator {
  void PlayAnimation();
public:
  std::string AnimationSourceFile;
  std::thread animationThread;
  void Play();
  void LoadAnimation(const std::string& sourceFile);
};

#endif // ANIMATE_HPP
