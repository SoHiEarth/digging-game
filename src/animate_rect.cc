#include <animate.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <base.h>

void Animator_Rect::PlayAnimation(std::atomic<bool>& running) {
  is_playing = true;
  for (AnimationFrame& frame : frames) {
    rect_to_animate.x += frame.xOffset;
    rect_to_animate.y += frame.yOffset;
    rect_to_animate.w += frame.wOffset;
    rect_to_animate.h += frame.hOffset;
    if (launch_args.find("-AnimDebugLog") != launch_args.end()) {
      if (launch_args["-AnimDebugLog"] == "1") std::cout << "x: " << rect_to_animate.x << " y: " << rect_to_animate.y << " w: " << rect_to_animate.w << " h: " << rect_to_animate.h << "\n";
    }
    if (!running) return;
    SDL_Delay(frame.timeUntilNextFrame);
  }
  is_playing = false;
}

void Animator_Rect::Play() {
  if (is_playing)
    animation_thread.Close();
  animation_thread.Start(std::bind(&Animator_Rect::PlayAnimation, this, std::placeholders::_1), "Rect");

}

void Animator_Rect::LoadAnimation(const std::string& source_file) {
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
    throw std::runtime_error("No frames found in file: " + source_file);
  }
  std::cout << "Loaded animation from file: " << source_file << "\n";
  int total_time = 0;
  for (const auto& frame : frames) {
    total_time += frame.timeUntilNextFrame;
  }
  std::cout << "Total time: " << total_time << "ms\n";
}
