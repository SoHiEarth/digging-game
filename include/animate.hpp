#ifndef ANIMATE_HPP
#define ANIMATE_HPP

#include <SDL.h>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <stdexcept>
#include <fstream>
#include <sstream>

struct AnimationFrame {
  int xOffset = 0;
  int yOffset = 0;
  int wOffset = 0;
  int hOffset = 0;
  int timeUntilNextFrame = 1;
};

class Animator_Rect {
  std::vector<AnimationFrame> frames{};
  bool isPlaying = false;

  void PlayAnimation() {
    isPlaying = true;
    for (AnimationFrame& frame : frames) {
      rectToAnimate.x += frame.xOffset;
      rectToAnimate.y += frame.yOffset;
      rectToAnimate.w += frame.wOffset;
      rectToAnimate.h += frame.hOffset;
      SDL_Delay(frame.timeUntilNextFrame);
    }
    isPlaying = false;
  }

public:
  std::string AnimationSourceFile;
  SDL_Rect& rectToAnimate;
  std::thread animationThread;

  Animator_Rect(SDL_Rect& rect) : rectToAnimate(rect) {}

  void Play() {
    if (animationThread.joinable()) {
      animationThread.join();
    }
    if (!isPlaying) animationThread = std::thread(&Animator_Rect::PlayAnimation, this);
  }

  void LoadAnimation(const std::string& sourceFile) {
    AnimationSourceFile = sourceFile;
    // Load animation from file
    // Animations are loaded as the following format
    // {specify properties (xOffset, etc.)}:{specify value of offset} {}:{} timeUntilNextFrame
    // Example:
    // {xOffset}:{10} {yOffset}:{0} {wOffset}:{0} {hOffset}:{0} 100

    std::ifstream file(sourceFile);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file: " + sourceFile);
    }

    std::string line;
    while (std::getline(file, line)) {
      AnimationFrame frame;
      std::istringstream iss(line);
      std::string token;
      while (iss >> token) {
        if (token == "{xOffset}:") {
          iss >> frame.xOffset;
        }
        else if (token == "{yOffset}:") {
          iss >> frame.yOffset;
        }
        else if (token == "{wOffset}:") {
          iss >> frame.wOffset;
        }
        else if (token == "{hOffset}:") {
          iss >> frame.hOffset;
        }
        else if (token == "{timeUntilNextFrame}:") {
          iss >> frame.timeUntilNextFrame;
        }
      }
      frames.push_back(frame);
    }

    file.close();

    if (frames.empty()) {
      throw std::runtime_error("No frames found in file: " + sourceFile);
    }

    std::cout << "Loaded animation from file: " << sourceFile << "\n";
  }
};

#endif // ANIMATE_HPP
