#include <animate.hpp>
#include <base.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
void Animator_Brightness::PlayAnimation() {
  isPlaying = true;
  for (AnimationFrame& frame : frames) {
    globalBrightness += frame.aOffset;
    globalBrightness = std::clamp(globalBrightness, 0, 100);
    SDL_Delay(frame.timeUntilNextFrame);
  }
  isPlaying = false;
}

void Animator_Brightness::Play() {
  if (animationThread.joinable())
    animationThread.join();
  if (!isPlaying) animationThread = std::thread(&Animator_Brightness::PlayAnimation, this);
}

void Animator_Brightness::LoadAnimation(const std::string& sourceFile) {
  AnimationSourceFile = sourceFile;
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
      if (token == "{aOffset}:") {
        iss >> frame.aOffset;
      }
      else if (token == "{timeUntilNextFrame}:") {
        iss >> frame.timeUntilNextFrame;
      }
    }
    frames.push_back(frame);
  }
  file.close();
  if (frames.empty()) {
    throw std::runtime_error("No frames loaded from file: " + sourceFile);
  }
}
