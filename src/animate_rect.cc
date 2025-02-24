#include <animate.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <base.h>

void Animator_Rect::PlayAnimation(std::atomic<bool>& running) {
  is_playing = true;
  play_count_since_start++;
  for (auto& frame : frames) {
    rect_to_animate.x += frame.xOffset;
    rect_to_animate.y += frame.yOffset;
    rect_to_animate.w += frame.wOffset;
    rect_to_animate.h += frame.hOffset;
    if (launch_args.find("-AnimDebugLog") != launch_args.end()) {
      if (launch_args["-AnimDebugLog"] == "1") std::cout << "x: " << rect_to_animate.x << " y: " << rect_to_animate.y << " w: " << rect_to_animate.w << " h: " << rect_to_animate.h << " time: " << frame.timeUntilNextFrame << "\n";
    }
    if (!running) return;
    std::this_thread::sleep_for(std::chrono::milliseconds(frame.timeUntilNextFrame));
  }
  is_playing = false;
}

void Animator_Rect::Play() {
  if (is_playing)
    animation_thread.Close();
  animation_thread.Start(std::bind(&Animator_Rect::PlayAnimation, this, std::placeholders::_1), "Rect");
}

void Animator_Rect::LoadAnimationByDelta(const std::string& source_file) {
  animation_source_file = source_file;
  std::ifstream file(source_file);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + source_file);
  }
  std::string line;
  
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string token;
    int x = 0, y = 0, w = 0, h = 0,
      // How many frames should the animation should be divided to?
      step = 1,
      time = 0;
    while (iss >> token) {
      if (token == "{xDestination}:") {
        iss >> x;
      }
      else if (token == "{yDestination}:") {
        iss >> y;
      }
      else if (token == "{wDestination}:") {
        iss >> w;
      }
      else if (token == "{hDestination}:") {
        iss >> h;
      }
      else if (token == "{time}:") {
        iss >> time;
      }
      else if (token == "{step}:") {
        iss >> step;
      }     
    }
    int x_step = (x - rect_to_animate.x) / step,
      y_step = (y - rect_to_animate.y) / step,
      w_step = (w - rect_to_animate.w) / step,
      h_step = (h - rect_to_animate.h) / step,
      frame_time = time / step; 
    for (int i = 0; i < step; i++) {
      AnimationFrame frame;
      frame.xOffset = x_step;
      frame.yOffset = y_step;
      frame.wOffset = w_step;
      frame.hOffset = h_step;
      frame.timeUntilNextFrame = frame_time;
      std::cout << "LOG: " << frame.xOffset << " " << frame.yOffset << " " << frame.wOffset << " " << frame.hOffset << " " << frame.timeUntilNextFrame << "\n";
      frames.push_back(frame);
     }
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