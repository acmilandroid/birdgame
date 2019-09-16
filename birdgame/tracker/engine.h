#include <vector>
#include <SDL.h>
#include <string>
#include "ioMod.h"
#include "hud.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "menuEngine.h"

class CollisionStrategy;
class SmartSprite;
class SubjectSprite;
class TwoWayMultiSprite;
class SDLSound;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext& rc;
  const IoMod& io;
  
  const Hud& hud;
  int helpEnable;
  
  Clock& clock;

  SDL_Renderer * const renderer;
  World sky;
  World flying;
  World cloud;
  Viewport& viewport;
  
  MenuEngine menuEngine;

  SubjectSprite* player;
  std::vector<Drawable*> clouds;
  std::vector<TwoWayMultiSprite*> planes;
  std::vector<SmartSprite*> pigeons;
  std::vector<SmartSprite*> eagles;
  CollisionStrategy* strategy;
  std::string currentStrategy;
  bool collision;
  
  SDLSound* sound;
  
  int currentSprite;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
  
  bool god;
  int winCount;
};
