#include "BitmapFont.h"

BitmapFont::BitmapFont()
{
  //ctor
}

BitmapFont::~BitmapFont()
{
  //dtor
}

void BitmapFont::Init(std::string fontImageFilename)
{
  _font = std::unique_ptr<PNGLoader>(new PNGLoader(fontImageFilename));
}
