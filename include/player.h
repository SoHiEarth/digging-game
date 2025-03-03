#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>
#include <items.h>
#include <iostream>
#include <assetbundleloader.h>
#include <object.h>


class Player : public Object {
  std::string name = "Player";
  public:
  std::string GetPlayerName() {
    return name;
  }
  void SetPlayerName(std::string name) {
    this->name = name;
    std::cout << "Player name set to " << name << "\n";
  }
    float health = 100, energy = 100, thirst = 100;
    std::vector<Item*> inventory;
    int current_item = 0;
    int move_speed = current_asset_bundle.PLAYER_MOVE_SPEED;
    void Start() override;
    void Fixed() override;
    void Update() override;
    void Quit() override;
};

#endif // PLAYER_H
