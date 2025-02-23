#include <animate.h>
#include <base.h>
#include <algorithm>
#include <fstream>
#include <sstream>
void Animator_Brightness::PlayAnimation() {
  is_playing = true;
  for (AnimationFrame& frame : frames) {
    global_brightness += frame.aOffset;
    global_brightness = std::clamp(global_brightness, 0, 100);
    SDL_Delay(frame.timeUntilNextFrame);
  }
  is_playing = false;
}

void Animator_Brightness::Play() {
  if (is_playing)
    animation_thread.Close();
  animation_thread.Start([this] { PlayAnimation(); });
}

void Animator_Brightness::LoadAnimation(const std::string& sourceFile) {
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
