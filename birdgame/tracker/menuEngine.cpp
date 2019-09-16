#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include "sprite.h"
#include "gameData.h"
#include "menuEngine.h"
#include "renderContext.h"

MenuEngine::~MenuEngine() { }

MenuEngine::MenuEngine() :
  clock( Clock::getInstance() ),
  renderer( RenderContext::getInstance().getRenderer() ),
  menu( renderer ),
  optionChoice(-1),
  menuOption(false)
{}

void MenuEngine::draw() const {
  menu.draw();
  SDL_RenderPresent(renderer);
}

bool MenuEngine::menuOptionChosen() { 
  if ( menuOption ) {
    menuOption = false;
    return true; 
  }
  return false;
}

void MenuEngine::update(Uint32) {
}

int MenuEngine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]
          || keystate[SDL_SCANCODE_P]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_DOWN] ) menu.incrIcon();
        if (keystate[SDL_SCANCODE_UP] ) menu.decrIcon();
        if (keystate[SDL_SCANCODE_RETURN] ) {
          menu.lightOn();
          optionChoice = menu.getOptionNo();
          if ( optionChoice == 0 ) done = true;
          if ( optionChoice == 1 ) {
            menuOption = true;
            menu.help();
          }
          if (optionChoice == 2) {
            done = true;
            return 1;
          }
        }
      }
      if(event.type == SDL_KEYUP) {
        menu.lightOff();
      }
    }
    // In this section of the event loop we allow key bounce:
    draw();
    //update(ticks);
  }
  return 0;
}
