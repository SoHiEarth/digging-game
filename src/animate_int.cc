#include <animate.h>
#include <safe_thread.h>
#include <fstream>
#include <base.h>
int start = 0;
double animate_target_internal = 0;
void Animator_Int::PlayAnimation(std::atomic<bool>& running) {
  is_playing = true;
  int_to_animate = start;
  animate_target_internal = int_to_animate;
  play_count_since_start++;
frame:
  for (auto& frame : frames) {
    animate_target_internal += frame.iOffset;
    int_to_animate = static_cast<int>(animate_target_internal);
    if (launch_args.find("-AnimDebugLog") != launch_args.end()) {
      if (launch_args["-AnimDebugLog"] == "1") std::cout << "Int: " << int_to_animate << "\n";
      if (!running) return;
      std::this_thread::sleep_for(std::chrono::milliseconds(frame.timeUntilNextFrame));
    }
  }
  if (!is_playing) return;
  if (repeat) goto frame;
  is_playing = false;
}

void Animator_Int::Play() {
  if (is_playing) return;
  is_playing = true;
  animation_thread.Start([this](std::atomic<bool>& running) { PlayAnimation(running); }, ((custom_attrib != "") ? custom_attrib : "ANIM_THREADING_DEFAULT"));
}

void Animator_Int::LoadAnimationByDelta(const std::string& source_file) {
  animation_source_file = source_file;
  frames.clear();
  std::ifstream file(source_file);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open animation file: " + source_file);
  }
  std::string line;
  bool delta_is_defined = false;
  while (std::getline(file, line)) {
    std::string token;
    int i = 0, step = 1, time = 0;
    std::istringstream iss(line);
    while (iss >> token) {
      if (token == "//ANIMATION_DELTA" ) {
        delta_is_defined = true;
      } else if (token == "{ANIM_THREADING_CUSTOM_ATTRIB}:") {
        iss >> custom_attrib;
      } if (token == "{dest}:") {
        iss >> i;
      } else if (token == "{time}:") {
        iss >> time;
      } else if (token == "{step}:") {
        iss >> step;
      } else if (token == "{repeat}:") {
        iss >> repeat;
      }
    }
    float i_step = (float)(i + start) / step,
        frame_time = (float)time / step;
    std::cout << "I_Step: " << i_step << " Frame_Time: " << frame_time << std::endl;
    for (int j = 0; j < step; j++) {
      AnimationFrame frame;
      frame.iOffset = i_step;
      frame.timeUntilNextFrame = frame_time;
      frames.push_back(frame);
    }
  }
  if (!delta_is_defined) {
    std::cout << "Dude you forgot to define the delta... wtf?\n";
  }
  file.close();
  std::cout << "Loaded animation from file: " << source_file << "\n";
  int total_time = 0;
  for (const auto& frame : frames) {
    total_time += frame.timeUntilNextFrame;
  }
  std::cout << "Total time: " << total_time << "ms\n";
}

void Animator_Int::LoadAnimation(const std::string& source_file) {
  animation_source_file = source_file;
  frames.clear();
  std::ifstream file(source_file);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open animation file: " + source_file);
  }
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string token;
    int i = 0, time = 0;
    while (iss >> token) {
      if (token == "{offset}:") {
        iss >> i;
      } else if (token == "{time}:") {
        iss >> time;
      }
    }
    AnimationFrame frame;
    frame.iOffset = i;
    frame.timeUntilNextFrame = time;
    frames.push_back(frame);
  }
}
