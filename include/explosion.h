#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "png_loader.h"

class Explosion
{
  public:
    Explosion();
    virtual ~Explosion();
  protected:
  private:
    PNGLoader* _imageRef;
};

#endif // EXPLOSION_H
