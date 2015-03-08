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

    const int LettersInRow = 16;
    const int LetterWidth = 32;
    const int StartingCharacter = 32;

    void Print(int x, int y, int anchor, std::string text);
    void Printf(int x, int y, int anchor, char* text, ...);

    void SetTextColor(SDL_Color newColor);
    void SetTextColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    void SetTextColor(unsigned int r, unsigned int g, unsigned int b);
    void SetScale(float scale);

    static const int AlignCenter = 0;
    static const int AlignLeft = 1;
    static const int AlignRight = 2;

  protected:
  private:
    BitmapFont();
    ~BitmapFont();
    BitmapFont(const BitmapFont&);
    BitmapFont& operator= (const BitmapFont&);

    std::unique_ptr<PNGLoader> _font;

    SDL_Color _textColor;

    int _lettersDistance;
    float _scaledLetterWidth;
};

#endif // BITMAPFONT_H
