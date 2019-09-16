#ifndef MENUENGINE_H
#define MENUENGINE_H

#include <vector>
#include <SDL.h>
#include "menu.h"

class MenuEngine {
public:
  MenuEngine ();
  ~MenuEngine ();
  int play();
  int getOptionChoice() const { return optionChoice; }
  bool menuOptionChosen();

private:
  Clock& clock;
  SDL_Renderer * const renderer;
  Menu menu;
  int optionChoice;  
  bool menuOption;

  void draw() const;
  void update(Uint32);

  MenuEngine(const MenuEngine&);
  MenuEngine& operator=(const MenuEngine&);
};

#endif
