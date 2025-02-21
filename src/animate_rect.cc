#include <animate.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <base.h>

void Animator_Rect::PlayAnimation() {
  isPlaying = true;
  for (AnimationFrame& frame : frames) {
    rectToAnimate.x += frame.xOffset;
    rectToAnimate.y += frame.yOffset;
    rectToAnimate.w += frame.wOffset;
    rectToAnimate.h += frame.hOffset;
    if (launch_args.find("-AnimDebugLog") != launch_args.end()) {
      if (launch_args["-AnimDebugLog"] == "1") std::cout << "x: " << rectToAnimate.x << " y: " << rectToAnimate.y << " w: " << rectToAnimate.w << " h: " << rectToAnimate.h << "\n";
    }
    SDL_Delay(frame.timeUntilNextFrame);
  }
  isPlaying = false;
}

void Animator_Rect::Play() {
  if (animationThread.joinable())
    animationThread.join();
  if (!isPlaying) animationThread = std::thread(&Animator_Rect::PlayAnimation, this);
}

void Animator_Rect::LoadAnimation(const std::string& sourceFile) {
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
