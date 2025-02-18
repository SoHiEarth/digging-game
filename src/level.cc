#include <level.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <humanoid.h>
#include <interactable.hpp>
void Level::Load(std::string path) {
  std::cout << "--- Loading level\n";
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "-!- Failed to open level file: " << path << std::endl;
    return;
  }
  std::string line;
  int lineNumber = 0;
  std::cout << "Reading line: ";
  while (std::getline(file, line)) {
    lineNumber++;
    std::cout << lineNumber << std::endl;
    std::istringstream iss(line);
    std::string objectName;
    iss >> objectName;
    if (objectName == "Sir") {
      objects.push_back(new Sir());
    } else if (objectName == "JailGuard") {
      objects.push_back(new JailGuard());
    } else if (objectName == "TestHumanoid") {
      objects.push_back(new TestHumanoid());
    } else if (objectName == "WaterRefillStation") {
      objects.push_back(new WaterRefillStation());
    }
  }
  std::cout << "\n--- Level loaded\n";
}
