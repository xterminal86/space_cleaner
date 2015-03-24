#include "explosion.h"

Explosion::Explosion()
{
  //ctor
}

Explosion::~Explosion()
{
  //dtor
}

void Explosion::Init(PNGLoader* imageRef, int fragmentsNum)
{
  _imageRef = imageRef;
  _fragmentsNum = fragmentsNum;

  for (int i = 0; i < _fragmentsNum; i++)
  {
    Fragment f;

    f.Angle = 0.0;
    f.RandomAngleIncrement = Util::CreateRandomRotation();
    Util::CreateRandomDirection(f.Direction);

    /*
    f.SrcRect.x = 0;
    f.SrcRect.y = 0;
    f.SrcRect.w = imageRef->Width();
    f.SrcRect.h = imageRef->Height();
    */
  }
}
