#include <animate.h>
#include <base.h>
#include <algorithm>
#include <fstream>
#include <sstream>
void Animator_Brightness::PlayAnimation(std::atomic<bool>& running) {
  is_playing = true;
  for (AnimationFrame& frame : frames) {
    global_brightness += frame.aOffset;
    global_brightness = std::clamp(global_brightness, 0, 100);
    if (!running) return;
    SDL_Delay(frame.timeUntilNextFrame);
  }
  is_playing = false;
}

void Animator_Brightness::Play() {
  if (is_playing)
    animation_thread.Close();
  animation_thread.Start(std::bind(&Animator_Brightness::PlayAnimation, this, std::placeholders::_1), "Brightness");
}

void Animator_Brightness::LoadAnimation(const std::string& source_file) {
  animation_source_file = source_file;
  std::ifstream file(source_file);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + source_file);
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
    throw std::runtime_error("No frames loaded from file: " + source_file);
  }
  std::cout << "Loaded animation from file: " << source_file << "\n";
  int total_time = 0;
  for (const auto& frame : frames) {
    total_time += frame.timeUntilNextFrame;
  }
  std::cout << "Total time: " << total_time << "ms\n";
}
