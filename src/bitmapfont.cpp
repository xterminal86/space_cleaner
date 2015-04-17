#include "BitmapFont.h"

BitmapFont::BitmapFont()
{
  //ctor
}

BitmapFont::~BitmapFont()
{
  //dtor
}

void BitmapFont::Init()
{
  int index = TextureManager::Get().FindTextureByRole(GlobalStrings::FontRole);
  if (index != -1)
  {
    _font = TextureManager::Get().GetTextureWrapper(index);
  }

  SetScale(1.0f);
}

void BitmapFont::SetTextColor(unsigned int r, unsigned int g, unsigned int b)
{
  SetTextColor(r, g, b, 255);
}

void BitmapFont::SetTextColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
  _textColor.r = r;
  _textColor.g = g;
  _textColor.b = b;
  _textColor.a = a;

  SDL_SetTextureColorMod(_font->Texture(), r, g, b);
  SDL_SetTextureAlphaMod(_font->Texture(), a);
}

void BitmapFont::SetTextColor(SDL_Color newColor)
{
  SetTextColor(newColor.r, newColor.g, newColor.b, newColor.a);
}

void BitmapFont::SetScale(double scale)
{
  _scaleFactor = scale;
  _scaledLetterWidth = LetterWidth * scale;
  _lettersDistance = (int)(_scaledLetterWidth / 2);
  //_lettersDistance = (int)(_scaledLetterWidth / 1.5);
}

void BitmapFont::Print(int x, int y, int anchor, std::string text)
{
  if (text.find('\n') != std::string::npos)
  {
    SplitText(text);

    int counter = 0;
    for (auto& s : _splittedString)
    {
      PrintString(s, anchor, x, y + counter*(_lettersDistance + _lettersDistance / 2));
      counter++;
    }
  }
  else
  {
    PrintString(text, anchor, x, y);
  }
}

void BitmapFont::Printf(int x, int y, int anchor, char* text, ...)
{
  if (text == nullptr) return;

	char buf[256];
	va_list	ap;

	va_start(ap, text);
	vsprintf(buf, text, ap);
	va_end(ap);

	Print(x, y, anchor, buf);
}

// ==================== Private Methods =================== //

void BitmapFont::SplitText(std::string& strRef)
{
  _splittedString.clear();

  std::string tmp(strRef);

  size_t res = tmp.find('\n');
  while (res != std::string::npos)
  {
    _splittedString.push_back(tmp.substr(0, res));
    tmp = tmp.substr(res + 1, tmp.size() - res);
    res = tmp.find('\n');
  }

  _splittedString.push_back(tmp.substr(res + 1, tmp.size() - res));
}

void BitmapFont::PrintString(std::string& strRef, int& anchor, int x, int y)
{
  _dst.x = x;
  _dst.y = y;
  _dst.w = (int)_scaledLetterWidth;
  _dst.h = (int)_scaledLetterWidth;

  size_t strLength = strRef.length();
  for (int i = 0; i < strLength; i++)
  {
    unsigned int code = strRef[i];

    code -= StartingCharacter;

    int py = code / LettersInRow;
    int px = code % LettersInRow;

    _src.x = px * LetterWidth;
    _src.y = py * LetterWidth;
    _src.w = LetterWidth;
    _src.h = LetterWidth;

    switch (anchor)
    {
      case AlignRight:
        _dst.x = x - (strLength - i)*_lettersDistance;
        break;

      case AlignCenter:
        _dst.x = (strLength % 2 == 0) ? x - (strLength / 2 - i)*_lettersDistance : ((x - (strLength / 2 - i)*_lettersDistance)) - _lettersDistance / 2;
        break;

      case AlignLeft:
      default:
        _dst.x = x + i*_lettersDistance;
        break;
    }

    SDL_RenderCopy(VideoSystem::Get().Renderer(), _font->Texture(), &_src, &_dst);
  }
}
