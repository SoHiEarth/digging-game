#ifndef APPLICATION_HPP
#define APPLICATION_HPP

enum ApplicationState {
  APP_STATE_MAIN_MENU,
  APP_STATE_GAME,
  APP_STATE_PAUSE,
  APP_STATE_GAME_OVER,
  APP_STATE_QUIT
};

class Application {
  ApplicationState state = APP_STATE_QUIT;
public:
  void Init();
  void mainMenu();
  void game_fixed();
  void game();
  void gameOver();
  void Run();
  void Quit();
};

#endif // APPLICATION_HPP
