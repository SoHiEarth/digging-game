#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <atomic>
enum ApplicationState {
  APP_STATE_MAIN_MENU,
  APP_STATE_GAME,
  APP_STATE_DIALOUGE,
  APP_STATE_PAUSE,
  APP_STATE_GAME_OVER,
  APP_STATE_QUIT
};

class Application {
public:
  ApplicationState state = APP_STATE_QUIT;
  void Init();
  void MainMenu();
  void Game();
  void Fixed(std::atomic<bool>& running);
  void Dialouge();
  void GameOver();
  void Run();
  void Quit();
};

#endif // APPLICATION_HPP
