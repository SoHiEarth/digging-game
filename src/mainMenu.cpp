#include <renderer_temp.hpp>
#include <application.hpp>

int currentSelectedMenuItem = 0;
void Application::mainMenu() {
  SDL_SetWindowTitle(window, "Holes - Main Menu");
  TTF_Font* hintFont = TTF_OpenFont(FONT_MENU_HINT_PATH, 20);
  if (hintFont == NULL) throw std::runtime_error("Error loading hint font");
  
  SDL_Surface* hintSurface = TTF_RenderText_Blended_Wrapped(hintFont, "Press Any Key to start\nPress [Esc] to quit", {255, 255, 255, 255}, 0);
  SDL_Texture* hintTexture = SDL_CreateTextureFromSurface(renderer, hintSurface);
  if (hintTexture == NULL) throw std::runtime_error("Error loading hint texture");
  SDL_FreeSurface(hintSurface);
  SDL_Rect hintRect;
  SDL_QueryTexture(hintTexture, NULL, NULL, &hintRect.w, &hintRect.h);
  hintRect = { 80, (600 - hintRect.h) / 2, hintRect.w, hintRect.h };
  SDL_Rect titleRect = { 0, 0, 800, 600};
  SDL_Texture* titleTexture = IMG_LoadTexture(renderer, MENU_BACKGROUND_PATH);
  if (titleTexture == NULL) throw std::runtime_error("Error loading title texture");
  while (state == APP_STATE_MAIN_MENU) {
      bool mouseClicked = false;
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
              state = APP_STATE_QUIT;
          }
          if (event.type == SDL_KEYDOWN) {
              switch (event.key.keysym.sym) {
                case SDLK_UP:
                  currentSelectedMenuItem--;
                  if (currentSelectedMenuItem < 0) currentSelectedMenuItem = 0;
                  break;
                case SDLK_DOWN:
                  currentSelectedMenuItem++;
                  if (currentSelectedMenuItem > 1) currentSelectedMenuItem = 1;
                  break;
                case SDLK_RETURN:
                  switch (currentSelectedMenuItem) {
                    case 0:
                      state = APP_STATE_GAME;
                      break;
                    case 1:
                      state = APP_STATE_QUIT;
                      break;
                  }
              }
          }
          if (event.type == SDL_MOUSEBUTTONDOWN) {
            mouseClicked = true;
          }
      }
      int mouseX, mouseY;
      SDL_GetMouseState(&mouseX, &mouseY);
      if (mouseClicked) {
        if (mouseX >= 400 - 100 && mouseX <= 400 + 100 && mouseY >= 300 && mouseY <= 300 + 50) {
          state = APP_STATE_GAME;
        }
        if (mouseX >= 400 - 100 && mouseX <= 400 + 100 && mouseY >= 350 && mouseY <= 350 + 50) {
          state = APP_STATE_QUIT;
        }
      }
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      
      SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
      
      SDL_Texture* menuItem1 = renderText("Start Game", hintFont, {0, 0, 0, 255});
      SDL_Texture* menuItem2 = renderText("Quit", hintFont, {0, 0, 0, 255});
      SDL_Rect menuItem1Rect, menuItem2Rect;
      SDL_QueryTexture(menuItem1, NULL, NULL, &menuItem1Rect.w, &menuItem1Rect.h);
      SDL_QueryTexture(menuItem2, NULL, NULL, &menuItem2Rect.w, &menuItem2Rect.h);
      menuItem1Rect = { 400 - menuItem1Rect.w / 2, 300, menuItem1Rect.w, menuItem1Rect.h };
      menuItem2Rect = { 400 - menuItem2Rect.w / 2, 350, menuItem2Rect.w, menuItem2Rect.h };
      
      SDL_Rect menuItem1BgRect = { menuItem1Rect.x - 10, menuItem1Rect.y - 10, menuItem1Rect.w + 20, menuItem1Rect.h + 20 };
      SDL_Rect menuItem2BgRect = { menuItem2Rect.x - 10, menuItem2Rect.y - 10, menuItem2Rect.w + 20, menuItem2Rect.h + 20 };
      SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
      switch (currentSelectedMenuItem) {
        case 0:
          SDL_RenderDrawRect(renderer, &menuItem1BgRect);
          break;
        case 1:
          SDL_RenderDrawRect(renderer, &menuItem2BgRect);
          break;
      }
      
      SDL_SetRenderDrawColor(renderer, 120, 120, 120, 128); 
      SDL_RenderFillRect(renderer, &menuItem1BgRect);
      SDL_RenderFillRect(renderer, &menuItem2BgRect);
      SDL_RenderCopy(renderer, menuItem1, NULL, &menuItem1Rect);
      SDL_RenderCopy(renderer, menuItem2, NULL, &menuItem2Rect);
      SDL_DestroyTexture(menuItem1);
      SDL_DestroyTexture(menuItem2);
      SDL_RenderPresent(renderer);
  }
  SDL_DestroyTexture(hintTexture);
  TTF_CloseFont(hintFont);
}