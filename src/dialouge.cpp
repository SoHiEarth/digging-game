#include <application.hpp>
#include <iostream>
#include <SDL.h>
#include <base.hpp>
#include <renderer_temp.hpp>
#include <config.h>
#include <character.hpp>

void Application::dialouge() {
  int currentDialougeIndex = 0;
  TTF_Font* font = TTF_OpenFont(FONT_GAME_DIALOUGE_PATH, 24);
  
  if (font == nullptr) {
    throw std::runtime_error("Failed to load font");
  }

  // Load character texture once before the loop
  if (currentHumanoid->characterTexture == nullptr) {
    currentHumanoid->characterTexture = IMG_LoadTexture(renderer, currentHumanoid->characterTexturePath.c_str());
    if (currentHumanoid->characterTexture == nullptr) {
      throw std::runtime_error("Failed to load character texture");
    }
  }

  SDL_Rect humanoidRect = { 100, 50, 600, 450 };

  // Create the dialogue background and text rectangle
  SDL_Rect dialougeRect = { 100, 450, 400, 100 };
  SDL_Rect dialougeBGRect = { dialougeRect.x - DIALOUGE_BG_BORDER_THICKNESS, 
                              dialougeRect.y - DIALOUGE_BG_BORDER_THICKNESS, 
                              dialougeRect.w + DIALOUGE_BG_BORDER_THICKNESS * 2, 
                              dialougeRect.h + DIALOUGE_BG_BORDER_THICKNESS * 2 };

  SDL_Texture* dialougeTexture = nullptr;
  SDL_Rect dialougeTextRect = {0, 0, 0, 0};

  // Dialogue loop
  while (state == APP_STATE_DIALOUGE) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        state = APP_STATE_QUIT;
      }
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN) {
          currentDialougeIndex++;
          if (currentDialougeIndex >= currentHumanoid->messages.size() || currentHumanoid->messages[currentDialougeIndex] == "%exit") {
            state = APP_STATE_GAME; // Transition to game state after dialogue
          }
        }
      }
    }

    // Render the dialogue text
    if (currentDialougeIndex < currentHumanoid->messages.size()) {
      dialougeTexture = renderText(currentHumanoid->messages[currentDialougeIndex].c_str(), font, {255, 255, 255});
      SDL_QueryTexture(dialougeTexture, NULL, NULL, &dialougeTextRect.w, &dialougeTextRect.h);
      dialougeTextRect.x = dialougeRect.x + 10;
      dialougeTextRect.y = dialougeRect.y + 10;
    }

    if (currentHumanoid->characterTexture == nullptr) {
      currentHumanoid->characterTexture = IMG_LoadTexture(renderer, currentHumanoid->characterTexturePath.c_str());
      if (currentHumanoid->characterTexture == NULL) {
        throw std::runtime_error("Failed to load character texture");
      }
    }

    // Clear the screen and render everything
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, currentHumanoid->characterTexture, NULL, &humanoidRect);

    // Draw the dialogue background and text
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &dialougeBGRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &dialougeRect);

    if (dialougeTexture != NULL) {
      SDL_RenderCopy(renderer, dialougeTexture, NULL, &dialougeTextRect);
      SDL_DestroyTexture(dialougeTexture);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60);
  }

  TTF_CloseFont(font);
}
