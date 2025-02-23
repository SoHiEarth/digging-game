#include <renderer_temp.h>
#include <application.h>
#include <resload.h>
int currentSelectedMenuItem = 0,
    window_width = 800,
    window_height = 600;
void Application::MainMenu() {
  SDL_SetWindowTitle(window, "Holes - Main Menu");
  TTF_Font* hintFont = ResLoad::LoadFont(current_asset_bundle.FONT_MENU_HINT_PATH, 20);
  SDL_Rect background_rect = { 0, 0, 800, 600};
  SDL_Texture* background_texture = ResLoad::LoadImage(current_asset_bundle.MENU_BACKGROUND_PATH);
  SDL_Texture* title_texture = ResLoad::LoadImage(current_asset_bundle.MENU_TITLE_PATH),
    *menu_start = ResLoad::RenderText(hintFont, "Start Game", {0, 0, 0, 255}),
    *menu_quit = ResLoad::RenderText(hintFont, "Quit", {0, 0, 0, 255});
  int menu_start_w, menu_start_h, menu_quit_w, menu_quit_h;
  SDL_QueryTexture(menu_start, NULL, NULL, &menu_start_w, &menu_start_h);
  SDL_QueryTexture(menu_quit, NULL, NULL, &menu_quit_w, &menu_quit_h);
  SDL_Rect menu_start_rect = { 400 - menu_start_w / 2, 300, menu_start_w, menu_start_h },
    menu_quit_rect = { 400 - menu_quit_w / 2, 350, menu_quit_w, menu_quit_h },
    menu_start_bg_rect = { menu_start_rect.x - 10, menu_start_rect.y - 10, menu_start_rect.w + 20, menu_start_rect.h + 20 },
    menu_quit_bg_rect = { menu_quit_rect.x - 10, menu_quit_rect.y - 10, menu_quit_rect.w + 20, menu_quit_rect.h + 20 },
    menu_start_selected_rect = { menu_start_bg_rect.x, menu_start_bg_rect.y, menu_start_bg_rect.w + 50, menu_start_bg_rect.h + 10 },
    menu_quit_selected_rect = { menu_quit_bg_rect.x, menu_quit_bg_rect.y, menu_quit_bg_rect.w + 50, menu_quit_bg_rect.h + 10 };
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
          case SDLK_ESCAPE:
            state = APP_STATE_QUIT;
            break;
          case SDLK_SPACE:
          case SDLK_RETURN:
            switch (currentSelectedMenuItem) {
              case 0:
                state = APP_STATE_GAME;
                break;
              case 1:
                state = APP_STATE_QUIT;
                break;
            }
            break;
        }
      }
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        mouseClicked = true;
      }
    }
    vec2 mousePos = {0, 0};
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    SDL_GetWindowSize(window, &window_width, &window_height);
    SDL_Rect title_rect = {window_width/2 - 384, 80, 384, 200};
    background_rect = { 0, 0, window_width, window_height };
    menu_start_rect = {title_rect.x + 30, window_height/2, menu_start_w, menu_start_h};
    menu_quit_rect = {title_rect.x + 30, menu_start_rect.y + menu_start_h + menu_quit_h, menu_quit_w, menu_quit_h};
    menu_start_bg_rect = { menu_start_rect.x - 10, menu_start_rect.y - 10, menu_start_rect.w + 20, menu_start_rect.h + 20 };
    menu_quit_bg_rect = { menu_quit_rect.x - 10, menu_quit_rect.y - 10, menu_quit_rect.w + 20, menu_quit_rect.h + 20 };
    menu_start_selected_rect = { menu_start_bg_rect.x, menu_start_bg_rect.y, menu_start_bg_rect.w + 50, menu_start_bg_rect.h };
    menu_quit_selected_rect = { menu_quit_bg_rect.x, menu_quit_bg_rect.y, menu_quit_bg_rect.w + 50, menu_quit_bg_rect.h };
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);  
    SDL_RenderCopy(renderer, background_texture, NULL, &background_rect);
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
    SDL_SetRenderDrawColor(renderer, 24, 24, 37, 255);
    SDL_RenderDrawRect(renderer, &menu_start_bg_rect);
    SDL_RenderDrawRect(renderer, &menu_quit_bg_rect);
    SDL_SetRenderDrawColor(renderer, 239, 86, 27, 255);
    switch (currentSelectedMenuItem) {
      case 0:
        SDL_RenderFillRect(renderer, &menu_start_selected_rect);
        break;
      case 1:
        SDL_RenderFillRect(renderer, &menu_quit_selected_rect);
        break;
    } 
    SDL_RenderCopy(renderer, menu_start, NULL, &menu_start_rect);
    SDL_RenderCopy(renderer, menu_quit, NULL, &menu_quit_rect);
    SDL_RenderPresent(renderer);
    if (SDL_PointInRect(mousePos.toSDLPointPtr(), &menu_start_rect)) {
      currentSelectedMenuItem = 0;
      if (mouseClicked) {
        state = APP_STATE_GAME;
      }
    }
    if (SDL_PointInRect(mousePos.toSDLPointPtr(), &menu_quit_rect)) {
      currentSelectedMenuItem = 1;
      if (mouseClicked) {
        state = APP_STATE_QUIT;
      }
    }
    SDL_Delay(1000/30);
  }
  SDL_DestroyTexture(background_texture);
  SDL_DestroyTexture(title_texture);
  SDL_DestroyTexture(menu_start);
  SDL_DestroyTexture(menu_quit);
  ResLoad::FreeFont(hintFont);
}
