#include <level.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include "base.h"
#include <humanoid.h>
#include <interactable.h>
#include <safe_thread.h>
#include <application.h>
void Level::Load(std::string path) {
  std::cout << "--- Loading level\n";
  if (!std::filesystem::exists(path)) {
    std::cerr << "-!- Requested file doesn't exist.\nFile: " << path << "\n";
    app->state = APP_STATE_QUIT;
    return;
  }
  // Close threads that aren't main before loading
  if (!AllThreadsClosed()) {
    std::cout << "Closing all threads\n";
    CloseAllThreads();
  }
  if (loaded) {
    std::cout << "Unloading level\n";
    Unload();
  }
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "-!- Failed to open level file: " << path << "\n";
    app->state = APP_STATE_QUIT;
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
      std::cerr << "\n-!- Unknown object name: " << object_name << "\n";
    }
  }
  std::cout << "--- Level loaded\n";
  loaded = true;
  std::cout << "--- Starting level\n";
  for (auto& object : objects) {
    object->Start();
  }
  std::cout << "--- Level Started\n";
  key_states.clear();
  global_brightness = 100;
  std::cout << "--- Starting fixed thread\n";
  fixed_thread.Start(std::bind(&Application::Fixed, app, std::placeholders::_1), "Fixed");
  std::cout << "--- Level loading complete\n";
}

void Level::LoadAtNextFrame(std::string path) {
  next_frame_path = path;
  std::cout << "Level with path " << path << " registered for next update\n";
}

bool Level::HasNextFramePath() {
  return next_frame_path != "";
}

void Level::LoadNextFramePath() {
  Load(next_frame_path);
  next_frame_path = "";
  std::cout << "--- Next frame loaded\n";
}

void Level::Unload() {
  thread_state = THREAD_STATE_TERMINATE;
  std::cout << "--- Unloading level\n";
  CloseAllThreads();
  for (Object* object : objects) {
    object->Quit();
    delete object;
  }
  objects.clear();
  loaded = false;
  thread_state = THREAD_STATE_OPEN;
}
