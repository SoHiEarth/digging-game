#include <animate.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <base.h>

void Animator_Color::PlayAnimation(std::atomic<bool>& running) {
  is_playing = true;
  for (AnimationFrame& frame : frames) {
    color_to_animate.r += frame.rOffset;
    color_to_animate.g += frame.gOffset;
    color_to_animate.b += frame.bOffset;
    color_to_animate.a += frame.aOffset;
    if (!running) return;
    SDL_Delay(frame.timeUntilNextFrame);
  }
  is_playing = false;
}

void Animator_Color::Play() {
  if (is_playing)
    animation_thread.Close();
  animation_thread.Start(std::bind(&Animator_Color::PlayAnimation, this, std::placeholders::_1), "Color");
}

void Animator_Color::LoadAnimation(const std::string& source_file) {
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
        std::cout << "Invalid token: " << token << " on file " << source_file << "\n";
        throw std::runtime_error("Invalid token: " + token + " on file " + source_file + ", file may be corrupt");
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
    throw std::runtime_error("No frames found in file: " + source_file);
  }
  std::cout << "Loaded animation from file: " << source_file << "\n";
}
