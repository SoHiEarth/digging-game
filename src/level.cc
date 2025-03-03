#include <level.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <base.h>
#include <humanoid.h>
#include <interactable.h>
#include <safe_thread.h>
#include <application.h>
#include <holes_2_manager.h>
#include <map_part.h>
#include <unordered_map>
std::unordered_map<std::string, Object*(*)()> object_factory {
  {"Sir", []() -> Object* { return new Sir(); }},
  {"JailGuard", []() -> Object* { return new JailGuard(); }},
  {"WaterRefillStation", []() -> Object* { return new WaterRefillStation(); }},
  {"Transport_Bus_1", []() -> Object* { return new TransportBus_Lv1(); }},
  {"Holes_2_Manager", []() -> Object* { return new Holes_2_Manager(); }},
  {"Pen", []() -> Object* { return new Pendanski(); }},
  {"Player", []() -> Object* { return new Player(); }},
  {"MapPart_Hill", []() -> Object* { return new Holes::Map::Hill(); }},
};

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
  Player* temp_player = nullptr;
  std::string line;
  int lineNumber = 0;
  while (std::getline(file, line)) {
    lineNumber++;
    std::cout << "Reading line: " << lineNumber << "\r";
    std::istringstream iss(line);
    std::string object_name;
    iss >> object_name;
    if (object_name == "Player") {
      if (temp_player != nullptr) {
        std::cerr << "-! L:" << lineNumber << "- Player already exists in level\n";
        continue;
      }
      temp_player = new Player();
    } else {
      if (object_factory.find(object_name) == object_factory.end()) {
        std::cerr << "-!- Object not found: " << object_name << "\n";
        continue;
      }
      Object* new_object = object_factory[object_name]();
      objects.push_back(new_object);
    }
  }
  if (temp_player != nullptr) {
    player = temp_player;
    objects.push_back(temp_player);
  } else {
    std::cerr << "-!- Player not found in level\n";
  }
  std::cout << "--- Level loaded\n";
  player_should_lock_camera = true;
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

void Level::CopyTempObjects() {
  if (objects.back() == player) {
    temp_objects.push_back(player);
    objects.pop_back();
  }
  for (Object* object : temp_objects) {
    objects.push_back(object);
  }
  temp_objects.clear();
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
