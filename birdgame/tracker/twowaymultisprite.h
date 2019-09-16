#ifndef TwoWayMultiSprite__H
#define TwoWayMultiSprite__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "bullets.h"

class ExplodingSprite;

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  ~TwoWayMultiSprite();

  virtual void draw() const;
  virtual int update(Uint32 ticks); //returns 1 on explosion complete
  virtual void explode();

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
  
  virtual void shoot();
  bool checkForCollisions(TwoWayMultiSprite*);
  
  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }
  
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&) = delete;

protected:
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> images;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  
  std::string bulletName;
  Bullets bullets;
  float bulletSpeed;
  int bulletInterval;
  int timeSinceLastBullet;
};
#endif
