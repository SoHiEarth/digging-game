#include <level.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <humanoid.h>
#include <interactable.h>
#include <safe_thread.h>
#include <application.h>
void Level::Load(std::string path) {
  std::cout << "--- Loading level\r";
  // Close threads that aren't main before loading
  if (!all_threads_closed()) {
    close_all_threads();
  }
  if (loaded) {
    Unload();
  }
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "-!- Failed to open level file: " << path << std::endl;
    return;
  }
  std::string line;
  int lineNumber = 0;
  while (std::getline(file, line)) {
    lineNumber++;
    std::cout << "Reading line: " << lineNumber << "\r";
    std::istringstream iss(line);
    std::string object_name;
    iss >> object_name;
    if (object_name == "Sir") {
      objects.push_back(new Sir());
    } else if (object_name == "JailGuard") {
      objects.push_back(new JailGuard());
    } else if (object_name == "WaterRefillStation") {
      objects.push_back(new WaterRefillStation());
    } else if (object_name == "Transport_Bus_1") {
      objects.push_back(new TransportBus_Lv1());
    } else {
      std::cerr << "-!- Unknown object name: " << object_name << std::endl;
    }
  }
  std::cout << "\r--- Level loaded\n";
  loaded = true;
  // Start fixed update thread
  fixed_thread.Start([this] { app->game_fixed(); });
}

void Level::LoadAtNextFrame(std::string path) {
  next_frame_path = path;
}

bool Level::HasNextFramePath() {
  return next_frame_path != "";
}

void Level::LoadNextFramePath() {
  Load(next_frame_path);
  next_frame_path = "";
}

void Level::Unload() {
  close_all_threads();
  for (Object* object : objects) {
    object->Quit();
    delete object;
  }
  objects.clear();
  loaded = false;
}
