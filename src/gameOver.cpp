#include <application.hpp>
#include <renderer_temp.hpp>

TTF_Font* gameOverFont = nullptr, *gameOverHintFont = nullptr;
SDL_Texture* gameOverTexture;
SDL_Texture* gameOverHintTexture;
SDL_Rect gameOverRect = { 0, 0, 800, 600 }, gameOverHintRect = { 0, 0, 800, 600 };
void Application::gameOver() {

  if (gameOverFont == nullptr) {
    gameOverFont = TTF_OpenFont(FONT_GAMEOVER_PATH, 32);
    if (gameOverFont == NULL) throw std::runtime_error("Error loading game over font");
  }
  if (gameOverHintFont == nullptr) {
    gameOverHintFont = TTF_OpenFont(FONT_GAMEOVER_HINT_PATH, 16);
    if (gameOverHintFont == NULL) throw std::runtime_error("Error loading game over hint font");
  }
  
  gameOverTexture = renderText("Game Over", gameOverFont, {255, 0, 0, 255});
  SDL_QueryTexture(gameOverTexture, NULL, NULL, &gameOverRect.w, &gameOverRect.h);
  gameOverRect = { (800 - gameOverRect.w) / 2, (600 - gameOverRect.h) / 2, gameOverRect.w, gameOverRect.h };
  gameOverHintTexture = renderText("[Press Any Key to Continue]", gameOverHintFont, {255, 255, 255, 255});
  SDL_QueryTexture(gameOverHintTexture, NULL, NULL, &gameOverHintRect.w, &gameOverHintRect.h);
  gameOverHintRect = { (800 - gameOverHintRect.w) / 2, 600 - gameOverHintRect.h - 10, gameOverHintRect.w, gameOverHintRect.h };
  
  while (state == APP_STATE_GAME_OVER) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        state = APP_STATE_QUIT;
      }
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            state = APP_STATE_QUIT;
            break;
          default:
            state = APP_STATE_MAIN_MENU;
            break;
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
    SDL_RenderCopy(renderer, gameOverHintTexture, NULL, &gameOverHintRect);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyTexture(gameOverTexture);
  SDL_DestroyTexture(gameOverHintTexture);
  TTF_CloseFont(gameOverFont);
  TTF_CloseFont(gameOverHintFont);
}
