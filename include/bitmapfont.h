#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include "png_loader.h"

class BitmapFont
{
  public:
    static BitmapFont& Get()
    {
      static BitmapFont instance;
      return instance;
    }
    void Init(std::string fontImageFilename);
  protected:
  private:
    BitmapFont();
    ~BitmapFont();
    BitmapFont(const BitmapFont&);
    BitmapFont& operator= (const BitmapFont&);

    std::unique_ptr<PNGLoader> _font;
};

#endif // BITMAPFONT_H
