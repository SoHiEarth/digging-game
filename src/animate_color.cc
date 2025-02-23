#include <animate.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <base.h>

void Animator_Color::PlayAnimation() {
  is_playing = true;
  for (AnimationFrame& frame : frames) {
    color_to_animate.r += frame.rOffset;
    color_to_animate.g += frame.gOffset;
    color_to_animate.b += frame.bOffset;
    color_to_animate.a += frame.aOffset;
    SDL_Delay(frame.timeUntilNextFrame);
  }
  is_playing = false;
}

void Animator_Color::Play() {
  if (is_playing)
    animation_thread.Close();
  animation_thread.Start([this] { PlayAnimation(); });
}

void Animator_Color::LoadAnimation(const std::string& sourceFile) {
    animation_source_file = sourceFile;    std::ifstream file(sourceFile);
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

