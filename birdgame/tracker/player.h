#ifndef PLAYER__H
#define PLAYER__H

#include "twowaymultisprite.h"

// In this example the player is derived from MultiSprite.
// However, there are many options.
class Player : public TwoWayMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual int update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&) = delete;

  void right();
  void left();
  void up();
  void down();
  void stop();
  virtual void explode();
  int getLives() { return lives; };
  int getKills() { return kills; }
  void addLife() { lives++; }
  void addKills() { kills++; }
private:
  bool collision;
  int lives;
  int kills;
  Vector2f initialVelocity;
};
#endif
