#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include "texture_manager.h"

class BitmapFont
{
  public:
    static BitmapFont& Get()
    {
      static BitmapFont instance;
      return instance;
    }
    void Init();

    const int LettersInRow = 16;
    const int LetterWidth = 16;
    const int StartingCharacter = 32;

    void Print(int x, int y, int anchor, std::string text);
    void Printf(int x, int y, int anchor, char* text, ...);

    void SetTextColor(SDL_Color newColor);
    void SetTextColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    void SetTextColor(unsigned int r, unsigned int g, unsigned int b);
    void SetScale(double scale);
    double ScaleFactor() { return _scaleFactor; }

    static const int AlignCenter = 0;
    static const int AlignLeft = 1;
    static const int AlignRight = 2;

  protected:
  private:
    BitmapFont();
    ~BitmapFont();
    BitmapFont(const BitmapFont&);
    BitmapFont& operator= (const BitmapFont&);

    PNGLoader* _font;

    SDL_Color _textColor;

    SDL_Rect _src, _dst;

    std::vector<std::string> _splittedString;

    int _lettersDistance;
    double _scaledLetterWidth;
    double _scaleFactor;

    void SplitText(std::string& text);
    void PrintString(std::string& strRef, int& anchor, int x, int y);
};

#endif // BITMAPFONT_H
