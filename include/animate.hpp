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
#include <map>
#include <base.hpp>

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

class Animator_Color {
  std::vector<AnimationFrame> frames{};
  bool isPlaying = false;

  void PlayAnimation() {
    isPlaying = true;
    for (AnimationFrame& frame : frames) {
      colorToAnimate.r += frame.rOffset;
      colorToAnimate.g += frame.gOffset;
      colorToAnimate.b += frame.bOffset;
      colorToAnimate.a += frame.aOffset;
      SDL_Delay(frame.timeUntilNextFrame);
    }
    isPlaying = false;
  }

  public:
  std::string AnimationSourceFile;
  SDL_Color& colorToAnimate;
  std::thread animationThread;

  Animator_Color(SDL_Color& color) : colorToAnimate(color) {}

  void Play() {
    if (animationThread.joinable()) {
      animationThread.join();
    }
    if (!isPlaying) animationThread = std::thread(&Animator_Color::PlayAnimation, this);
  }

  void LoadAnimation(const std::string& sourceFile) {
    AnimationSourceFile = sourceFile;
    // Load animation from file
    // Animations are loaded as the following format
    // {specify properties (rOffset, etc.)}:{specify value of offset} {}:{} timeUntilNextFrame
    // Example:
    // {rOffset}:{10} {gOffset}:{0} {bOffset}:{0} {aOffset}:{0} 100

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
        // Read into int first, then cast to Uint8 to avoid issues
        int r, g, b, a;
        if (token == "{rOffset}:") {
          iss >> r;
        }
        else if (token == "{gOffset}:") {
          iss >> g;
        }
        else if (token == "{bOffset}:") {
          iss >> b;
        }
        else if (token == "{aOffset}:") {
          iss >> a;
        }
        else if (token == "{timeUntilNextFrame}:") {
          iss >> frame.timeUntilNextFrame;
        }

        else {
          std::cout << "Invalid token: " << token << " on file " << sourceFile << "\n";
          throw std::runtime_error("Invalid token: " + token + " on file " + sourceFile + ", file may be corrupt");
        }

        frame.rOffset = static_cast<Uint8>(r);
        frame.gOffset = static_cast<Uint8>(g);
        frame.bOffset = static_cast<Uint8>(b);
        frame.aOffset = static_cast<Uint8>(a);
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
      if (launchParams.find("-AnimDebugLog") != launchParams.end()) {
        if (launchParams["-AnimDebugLog"] == "1") std::cout << "x: " << rectToAnimate.x << " y: " << rectToAnimate.y << " w: " << rectToAnimate.w << " h: " << rectToAnimate.h << "\n";
      }
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
