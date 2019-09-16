#include "hud.h"
#include "ioMod.h"
#include "gameData.h"
#include "clock.h"
#include <sstream>
#include <SDL2/SDL.h>
#include "player.h"

Hud& Hud::getInstance() {
  static Hud instance;
  return instance;
}

Hud::Hud() :
  r(Gamedata::getInstance().getXmlInt("hud/red")),
  g(Gamedata::getInstance().getXmlInt("hud/green")),
  b(Gamedata::getInstance().getXmlInt("hud/blue")),
  a(Gamedata::getInstance().getXmlInt("hud/alpha")),
  width(Gamedata::getInstance().getXmlInt("hud/width")),
  height(Gamedata::getInstance().getXmlInt("hud/height")),
  boxEnable(Gamedata::getInstance().getXmlInt("hud/box")),
  fpsEnable(Gamedata::getInstance().getXmlInt("hud/fps")),
  infoEnable(Gamedata::getInstance().getXmlInt("hud/info")) {}

void Hud::drawHelp(SDL_Renderer* renderer, unsigned int bulletCount,
unsigned int freeCount, bool god) const {
  Clock& c = Clock::getInstance();
  std::ostringstream stream;
  SDL_Rect rect;
  rect.x = 2;
  rect.y = 2;
  rect.w = width;
  rect.h = height;
  SDL_Color textColor = {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};
  SDL_SetRenderDrawColor(renderer, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a);
  if (boxEnable) {
    SDL_RenderDrawRect(renderer, &rect);
  }
  if (fpsEnable) {
    stream << "Framerate: " << c.getFps();
    IoMod::getInstance().writeText(stream.str(), 5, 5, textColor);
    stream.str("");
  }
  if (infoEnable) {
    IoMod::getInstance().writeText("F1 - toggle help", 5, 35, textColor);
    IoMod::getInstance().writeText("WASD - move", 5, 65, textColor);
    IoMod::getInstance().writeText("P - pause / unpause", 5, 95, textColor);
    IoMod::getInstance().writeText("SPACE - poop", 5, 125, textColor);
    stream << "In Use: " << bulletCount;
    IoMod::getInstance().writeText(stream.str(), 5, 155, textColor);
    stream.str("");
    stream << "Pool: " << freeCount;
    IoMod::getInstance().writeText(stream.str(), 5, 185, textColor);
    stream.str("");
    stream << "G - God Mode: ";
    if (god) stream << "OFF" << std::endl;
    else stream << "ON" << std::endl;
    IoMod::getInstance().writeText(stream.str(), 5, 215, textColor);
    stream.str("");
  }
}

void Hud::draw(int lives, int kills) const {
  std::ostringstream stream;
  SDL_Color textColor = {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};

  stream << "Lives: " << lives;
  IoMod::getInstance().writeText(stream.str(), 330, 5, textColor);
  stream.str("");
  
  stream << "Planes Pooped: " << kills;
  IoMod::getInstance().writeText(stream.str(), 330, 35, textColor);
  stream.str("");
}
