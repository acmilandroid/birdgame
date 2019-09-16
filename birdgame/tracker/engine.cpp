#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include "multisprite.h"
#include "twowaymultisprite.h"
#include "player.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "sound.h"
#include "menuEngine.h"

Engine::~Engine() { 
  for (auto& it : clouds) delete it;
  for (auto& it : planes) delete it;
  for (auto& it : pigeons) delete it;
  for (auto& it : eagles) delete it;
  delete player;
  delete strategy;
  delete sound;
  SDL_DestroyRenderer(renderer);
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  hud( Hud::getInstance() ),
  helpEnable(1),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  flying("flying", Gamedata::getInstance().getXmlInt("flying/factor") ),
  cloud("cloud", Gamedata::getInstance().getXmlInt("cloud/factor") ),
  viewport( Viewport::getInstance() ),
  menuEngine(),
  player(new SubjectSprite("Player")),
  clouds(5),
  planes(5),
  pigeons(3),
  eagles(4),
  strategy(nullptr),
  currentStrategy(Gamedata::getInstance().getXmlStr("collisionStrategy")),
  collision(false),
  sound(new SDLSound()),
  currentSprite(0),
  god(true),
  winCount(Gamedata::getInstance().getXmlInt("Wincount"))
{
  player->setScale(.5);
  
  //make dumb cloud sprites
  clouds[0] = new Sprite("Cloud0");
  clouds[0]->setScale(.2);
  clouds[1] = new Sprite("Cloud1");
  clouds[1]->setScale(.3);
  clouds[2] = new Sprite("Cloud2");
  clouds[2]->setScale(.4);
  clouds[3] = new Sprite("Cloud3");
  clouds[3]->setScale(.5);
  clouds[4] = new Sprite("Cloud4");
  clouds[4]->setScale(.6);
  
  //make new plane sprites
  planes[0] = new TwoWayMultiSprite("PlaneBig");
  planes[0]->setScale(.3);
  planes[1] = new TwoWayMultiSprite("PlaneMedium");
  planes[1]->setScale(.6);
  planes[2] = new TwoWayMultiSprite("PlaneSmall");
  planes[2]->setScale(.2);
  planes[3] = new TwoWayMultiSprite("PlaneSmall2");
  planes[3]->setScale(.2);
  planes[4] = new TwoWayMultiSprite("PlaneMedium2");
  planes[4]->setScale(.6);
  
  //get player properties
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  
  //make pigeon sprites
  pigeons[0] = new SmartSprite("Pigeon", pos, w, h, 0);
  pigeons[0]->setScale(.5);
  pigeons[1] = new SmartSprite("PigeonSlow", pos, w, h, 0);
  pigeons[1]->setScale(.5);
  pigeons[2] = new SmartSprite("PigeonSlow2", pos, w, h, 0);
  pigeons[2]->setScale(.5);
  for (auto& it : pigeons) player->attach(it);
  
  //make eagle sprites
  eagles[0] = new SmartSprite("Eagle0", pos, w, h, 2);
  eagles[1] = new SmartSprite("Eagle1", pos, w, h, 2);
  eagles[2] = new SmartSprite("Eagle2", pos, w, h, 2);
  eagles[3] = new SmartSprite("Eagle3", pos, w, h, 2);
  for (auto& it : eagles) player->attach(it);
  
  //make collision strategy
  if ( currentStrategy == "PerPixel" ) {
    strategy = new PerPixelCollisionStrategy;
  }
  else if ( currentStrategy == "Rectangular" ) {
    strategy = new RectangularCollisionStrategy;
  }
  else if ( currentStrategy == "MidPoint" ) {
    strategy = new MidPointCollisionStrategy;
  }
  
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  flying.draw();
  cloud.draw();
  
  player->draw();
  for (auto& it : planes) it->draw();
  for (auto& it : pigeons) it->draw();
  for (auto& it : eagles) it->draw();
  for (auto& it : clouds) it->draw();
  if (helpEnable) hud.drawHelp(renderer, 
    player->bulletCount(), player->freeCount(), god);
  hud.draw(player->getLives(), player->getKills());
  viewport.draw();
  
  if (player->getLives() == 0) {
    io.writeText("Press R to restart, or P for the menu", 250, 200);
    clock.pause();
  }
  
  if (player->getKills() == winCount) {
    io.writeText("Great job pooping!", 250, 200);
    io.writeText("Press R to restart, or P for the menu", 250, 230);
    clock.pause();   
  }
  
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = pigeons.begin();
  while ( it != pigeons.end() ) { //check pigeon contact
    if ( strategy->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = pigeons.erase(it);
      player->addLife();
    }
    else ++it;
  }
  it = eagles.begin();
  while ( it != eagles.end() ) { //check eagle contact
    if (strategy->execute(*player, **it) && god) { //kills player and eagle
      //get player properties
      Vector2f pos = player->getPosition();
      int w = player->getScaledWidth();
      int h = player->getScaledHeight();
      sound[0][2];
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = eagles.erase(it);
      player->explode();
      doa = new SmartSprite("Eagle0", pos, w, h, 2); //respawns eagle
      player->attach(doa);
      eagles.push_back(doa);
    }
    else ++it;
  }
  for (auto& it : planes) {
    if (player->checkForCollisions(it)) { //check bullet collision with plane
      player->addKills();
      sound[0][1];
    }
    if (strategy->execute(*player, *it) && god) { //check player collisions
      sound[0][2];
      player->explode();
    }
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  sky.update();
  flying.update();
  cloud.update();
  player->update(ticks);
  for (auto& it : planes) {
    if (it->update(ticks)) {
      auto temp = new TwoWayMultiSprite(it->getName());
      temp->setScale(it->getScale());
      delete(it);
      it = temp;
    }
  }
  for (auto& it : pigeons) it->update(ticks);
  for (auto& it : eagles) it->update(ticks);
  for (auto& it : clouds) it->update(ticks);
  if (helpEnable) hud.drawHelp(renderer, 
    player->bulletCount(), player->freeCount(), god);
  hud.draw(player->getLives(), player->getKills());
  viewport.update(); // always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_SPACE]){
          sound[0][0];
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          clock.pause();
          int quit = menuEngine.play();
          if (quit == 1) {
            done = true;
            break;
          }
          clock.unpause();
        }
        if (keystate[SDL_SCANCODE_F1]) {
          if (helpEnable == 1) helpEnable = 0;
          else helpEnable = 1;
        }
        if (keystate[SDL_SCANCODE_R]) {
          clock.unpause();
          return true;
        }
        if (keystate[SDL_SCANCODE_G]) {
          god ^= true;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_SPACE]) {
        player->shoot();
      }
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      draw();
      update(ticks);
    }
  }
  return false;
}
