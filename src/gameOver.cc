#include <application.h>
#include <base.h>
#include <resload.h>
void Application::GameOver() {
  TTF_Font* gameOverFont = nullptr, *gameOverHintFont = nullptr;
  SDL_Texture *gameOverTexture = nullptr, *gameOverHintTexture = nullptr;
  SDL_Rect gameOverRect = { 0, 0, window_width, window_height }, gameOverHintRect = gameOverRect;
  gameOverFont = ResLoad::LoadFont(current_asset_bundle.FONT_GAMEOVER_PATH, 32);
  gameOverHintFont = ResLoad::LoadFont(current_asset_bundle.FONT_GAMEOVER_HINT_PATH, 16);
  gameOverTexture = ResLoad::RenderText(gameOverFont, "Game Over", {255, 0, 0, 255});
  SDL_QueryTexture(gameOverTexture, NULL, NULL, &gameOverRect.w, &gameOverRect.h);
  gameOverRect = { (window_width - gameOverRect.w) / 2, (window_height - gameOverRect.h) / 2, gameOverRect.w, gameOverRect.h };
  gameOverHintTexture = ResLoad::RenderText(gameOverHintFont, "[Press Any Key to Continue]", {255, 255, 255, 255});
  SDL_QueryTexture(gameOverHintTexture, NULL, NULL, &gameOverHintRect.w, &gameOverHintRect.h);
  gameOverHintRect = { (window_width - gameOverHintRect.w) / 2, window_height - gameOverHintRect.h - 10, gameOverHintRect.w, gameOverHintRect.h };
  
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
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        state = APP_STATE_MAIN_MENU;
      }
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          SDL_GetWindowSize(window, &window_width, &window_height);
          gameOverRect = { (window_width - gameOverRect.w) / 2, (window_height - gameOverRect.h) / 2, gameOverRect.w, gameOverRect.h };
          gameOverHintRect = { (window_width - gameOverHintRect.w) / 2, window_height - gameOverHintRect.h - 10, gameOverHintRect.w, gameOverHintRect.h };
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
  ResLoad::FreeFont(gameOverFont);
  ResLoad::FreeFont(gameOverHintFont);
}
