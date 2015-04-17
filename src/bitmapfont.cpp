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
}

void BitmapFont::Print(int x, int y, int anchor, std::string text)
{
  size_t strLength = text.length();

  SDL_Rect src, dst;

  dst.x = x;
  dst.y = y;
  dst.w = (int)_scaledLetterWidth;
  dst.h = (int)_scaledLetterWidth;

  for (int i = 0; i < strLength; i++)
  {
    unsigned int code = text[i];
    code -= StartingCharacter;

    int py = code / LettersInRow;
    int px = code % LettersInRow;

    src.x = px * LetterWidth;
    src.y = py * LetterWidth;
    src.w = LetterWidth;
    src.h = LetterWidth;

    switch (anchor)
    {
      case AlignRight:
        dst.x = x - (strLength - i)*_lettersDistance;
        break;

      case AlignCenter:
        dst.x = (strLength % 2 == 0) ? x - (strLength / 2 - i)*_lettersDistance : (x - (strLength / 2 - i)*_lettersDistance) - _lettersDistance / 2;
        break;

      case AlignLeft:
      default:
        dst.x = x + i*_lettersDistance;
        break;
    }

    SDL_RenderCopy(VideoSystem::Get().Renderer(), _font->Texture(), &src, &dst);
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
