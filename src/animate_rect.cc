#include <animate.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <base.h>

void Animator_Rect::PlayAnimation() {
  is_playing = true;
  for (AnimationFrame& frame : frames) {
    rect_to_animate.x += frame.xOffset;
    rect_to_animate.y += frame.yOffset;
    rect_to_animate.w += frame.wOffset;
    rect_to_animate.h += frame.hOffset;
    if (launch_args.find("-AnimDebugLog") != launch_args.end()) {
      if (launch_args["-AnimDebugLog"] == "1") std::cout << "x: " << rect_to_animate.x << " y: " << rect_to_animate.y << " w: " << rect_to_animate.w << " h: " << rect_to_animate.h << "\n";
    }
    SDL_Delay(frame.timeUntilNextFrame);
  }
  is_playing = false;
}

void Animator_Rect::Play() {
  if (is_playing)
    animation_thread.Close();
  animation_thread.Start([this] { PlayAnimation(); });
}

void Animator_Rect::LoadAnimation(const std::string& sourceFile) {
  animation_source_file = sourceFile;
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
