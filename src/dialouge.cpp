#include <application.hpp>
#include <iostream>
#include <SDL.h>
#include <base.hpp>
#include <renderer_temp.hpp>
#include <config.h>
#include <character.hpp>
#include <assetbundleloader.hpp>

void Application::dialouge() {
  int currentDialougeIndex = 0;
  TTF_Font* font = TTF_OpenFont(assetBundle.FONT_GAME_DIALOUGE_PATH.c_str(), 24);
  TTF_Font* topFont = TTF_OpenFont(assetBundle.FONT_GAME_DIALOUGE_NAME_PATH.c_str(), 18);
  
  if (font == nullptr || topFont == nullptr) {
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
  SDL_Rect dialougeRect = { 150, 450, 500, 100 };
  SDL_Rect dialougeBGRect = { dialougeRect.x - assetBundle.DIALOUGE_BG_BORDER_THICKNESS, 
                              dialougeRect.y - assetBundle.DIALOUGE_BG_BORDER_THICKNESS, 
                              dialougeRect.w + assetBundle.DIALOUGE_BG_BORDER_THICKNESS * 2, 
                              dialougeRect.h + assetBundle.DIALOUGE_BG_BORDER_THICKNESS * 2 };
  SDL_Texture* characterName = nullptr;
  SDL_Rect characterNameRect = {0, 0, 0, 0}, characterNameBGRect = {0, 0, 0, 0};

  SDL_Texture* dialougeTexture = nullptr;
  SDL_Rect dialougeTextRect = {0, 0, 0, 0};

  // Dialogue loop
  while (state == APP_STATE_DIALOUGE) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        state = APP_STATE_QUIT;
      }
      if (event.type == SDL_KEYDOWN) {    
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          state = APP_STATE_MAIN_MENU;
        }
        currentDialougeIndex++; 
      }
    }

    if (currentDialougeIndex == currentHumanoid->messages.size()) {
      state = APP_STATE_GAME; // Transition to game state after dialogue
      std::cout << "Reached end of dialouge queue\n";
      break;
    }

    characterName = renderText(currentHumanoid->characterName.c_str(), topFont, {255, 255, 255, 255});
    SDL_QueryTexture(characterName, NULL, NULL, &characterNameRect.w, &characterNameRect.h);
    characterNameRect.x = dialougeRect.x + 5;
    characterNameRect.y = dialougeRect.y - (characterNameRect.h * 0.5);
    characterNameBGRect = characterNameRect;
    characterNameBGRect.x -= assetBundle.DIALOUGE_BG_BORDER_THICKNESS;
    characterNameBGRect.y -= assetBundle.DIALOUGE_BG_BORDER_THICKNESS;
    characterNameBGRect.w += assetBundle.DIALOUGE_BG_BORDER_THICKNESS * 2;
    characterNameBGRect.h += assetBundle.DIALOUGE_BG_BORDER_THICKNESS * 2;

    // Render the dialogue text
    if (currentDialougeIndex < currentHumanoid->messages.size()) {
      SDL_Surface* dialougeSurface = TTF_RenderText_Blended_Wrapped(font, currentHumanoid->messages[currentDialougeIndex].c_str(), {255, 255, 255}, 500);
      dialougeTexture = SDL_CreateTextureFromSurface(renderer, dialougeSurface);
      SDL_FreeSurface(dialougeSurface);
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, currentHumanoid->characterTexture, NULL, &humanoidRect);

    // Draw the dialogue background and text
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderFillRect(renderer, &dialougeBGRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &dialougeRect);

    if (characterName != NULL) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderFillRect(renderer, &characterNameBGRect);
      SDL_RenderCopy(renderer, characterName, NULL, &characterNameRect);
      SDL_DestroyTexture(characterName);
    }

    if (dialougeTexture != NULL) {
      SDL_RenderCopy(renderer, dialougeTexture, NULL, &dialougeTextRect);
      SDL_DestroyTexture(dialougeTexture);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60);
  }
  
  std::cout << "Unloading assets\n";
  TTF_CloseFont(font);
}
