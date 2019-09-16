#include "player.h"
#include "gameData.h"
#include "sprite.h"
#include "explodingSprite.h"

Player::Player( const std::string& name) :
  TwoWayMultiSprite(name),
  collision(false),
  lives(Gamedata::getInstance().getXmlInt(name+"/lives")),
  kills(0),
  initialVelocity(getVelocity())
{ }

Player::Player(const Player& s) :
  TwoWayMultiSprite(s), 
  collision(s.collision),
  lives(s.lives),
  kills(s.kills),
  initialVelocity(s.getVelocity())
{ }

void Player::stop() { 
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.96*getVelocityX() );
  setVelocityY(0);
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    images = imagesRight;
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    images = imagesLeft;
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

int Player::update(Uint32 ticks) {
  if ( !collision ) TwoWayMultiSprite::update(ticks);
  stop();
  return 0;
}

void Player::explode() {
  if (!explosion) {
  	 Sprite sprite(getName(), getPosition(), getVelocity(), getImage());
  	 sprite.setScale(getScale());
  	 lives--;
  	 explosion = new ExplodingSprite(sprite);
  }
}
