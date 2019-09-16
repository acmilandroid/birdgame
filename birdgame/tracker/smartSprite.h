#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include "twowaymultisprite.h"

class SmartSprite : public TwoWayMultiSprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h, int mode);
  SmartSprite(const SmartSprite&);
  virtual ~SmartSprite() { } 

  virtual int update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

private:
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  int currentMode; //0 normal, 1 evade, 2 attack
  float safeDistance;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
