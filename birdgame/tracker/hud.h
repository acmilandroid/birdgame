#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

class Hud {
public:
  static Hud& getInstance();
  void drawHelp(SDL_Renderer*, unsigned int, unsigned int, bool) const;
  void draw(int, int) const;
  Hud(const Hud&) = delete;
  Hud& operator=(const Hud&) = delete;
private:
  int r;
  int g;
  int b;
  int a;
  int width;
  int height;
  int boxEnable;
  int fpsEnable;
  int infoEnable;
  Hud();
};
