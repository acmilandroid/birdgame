#include "twowaymultisprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "sprite.h"
#include "explodingSprite.h"

TwoWayMultiSprite::~TwoWayMultiSprite() {
  delete explosion;
}

void TwoWayMultiSprite::explode() {
  if (!explosion) {
  	 Sprite sprite(getName(), getPosition(), getVelocity(), getImage());
  	 sprite.setScale(getScale());
  	 explosion = new ExplodingSprite(sprite);
  }
}

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  imagesRight( ImageFactory::getInstance().getImages(name) ),
  imagesLeft( ImageFactory::getInstance().getImages(name+"Left") ),
  images(imagesRight),
  explosion(nullptr),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullets(bulletName),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedY")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0)
{ }

TwoWayMultiSprite::TwoWayMultiSprite( const TwoWayMultiSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  imagesRight(s.imagesRight),
  imagesLeft(s.imagesLeft),
  images(imagesLeft),
  explosion(s.explosion),

  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  
  bulletName(s.bulletName),
  bullets(s.bullets),
  bulletSpeed(s.bulletSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(0)
{ }

/*
TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  
  bulletName = (s.bulletName),
  bullets = (s.bullets),
  bulletSpeed =(s.bulletSpeed),
  bulletInterval = (s.bulletInterval),
  timeSinceLastBullet = (0)
  return *this;
}*/

void TwoWayMultiSprite::shoot() { 
  // See if it's time to shoot a bullet:
  if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    float x; 
    float y = getY()+getScaledHeight()/4+11;
    if ( vel[0] > 0 ) {
      x = getX();
      vel[1] += bulletSpeed;
    }
    else {
      x = getX()+getScaledWidth()*.8;
      vel[1] += bulletSpeed;
    }
    bullets.shoot( Vector2f(x, y), vel ); 
	timeSinceLastBullet = 0;
  }
}

void TwoWayMultiSprite::draw() const { 
  if (explosion) {
    explosion->draw();
    return;
  }
  images[currentFrame]->draw(getX(), getY(), getScale());
  bullets.draw();
}

bool TwoWayMultiSprite::checkForCollisions(TwoWayMultiSprite* obj) {
  if (bullets.collided(obj)) {
    obj->explode();
    return true;
  }
  return false;
}

int TwoWayMultiSprite::update(Uint32 ticks) { 
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0) {
      delete explosion;
      explosion = nullptr;
      return 1;
    }
    return 0;
  }
  advanceFrame(ticks);
  timeSinceLastBullet += ticks;
  bullets.update(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0 - 100) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight + 100) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0 - 1000) {
    setVelocityX( fabs( getVelocityX() ) );
    images = imagesRight;
  }
  if ( getX() + getScaledWidth() > worldWidth + 1000) {
    setVelocityX( -fabs( getVelocityX() ) );
    images = imagesLeft;
  }  
  return 0;
}
