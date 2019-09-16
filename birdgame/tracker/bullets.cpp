#include <iostream>
#include <cmath>
#include "bullets.h"
#include "imageFactory.h"
#include "collisionStrategy.h"

Bullets::~Bullets() {
  std::list<Bullet>::const_iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr = bulletList.erase(ptr);
  }
  ptr = freeList.begin();
  while (ptr != freeList.end()) {
    ptr = freeList.erase(ptr);
  }
}

Bullets::Bullets(const std::string& n) :
  name(n),
  myVelocity(
    Gamedata::getInstance().getXmlInt(name+"/speed/x"), 
    Gamedata::getInstance().getXmlInt(name+"/speed/y")
  ),
  bulletList(),
  freeList(),
  strategy(NULL),
  scale(Gamedata::getInstance().getXmlFloat(name+"/scale"))
{ 
  const string thisStrategy = 
    Gamedata::getInstance().getXmlStr("collisionStrategy");
  if ( thisStrategy == "PerPixel" ) {
    strategy = new PerPixelCollisionStrategy;
  }
  else if ( thisStrategy == "Rectangular" ) {
    strategy = new RectangularCollisionStrategy;
  }
  else if ( thisStrategy == "MidPoint" ) {
    strategy = new MidPointCollisionStrategy;
  }
}

void Bullets::shoot(const Vector2f& pos, const Vector2f& objVel) {
  if (freeList.empty()) {
    Bullet b (name, pos, objVel);
    b.setScale(scale);
    bulletList.push_back(b);
  }
  else {
    Bullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.push_back(b);
  }
}

bool Bullets::collided(const Drawable* obj) { 
  std::list<Bullet>::const_iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void Bullets::draw() const { 
  for ( const auto& bullet : bulletList ) {
    bullet.draw();
  }
}

int Bullets::update(int ticks) { 
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a chunk
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
  return 0;
}
