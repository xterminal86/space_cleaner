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

  SDL_SetTextureColorMod(_font.get()->Texture(), r, g, b);
  SDL_SetTextureAlphaMod(_font.get()->Texture(), a);
}

void BitmapFont::SetTextColor(SDL_Color newColor)
{
  SetTextColor(newColor.r, newColor.g, newColor.b, newColor.a);
}

void BitmapFont::SetScale(float scale)
{
  _scaledLetterWidth = LetterWidth * scale;
  _lettersDistance = (int)(_scaledLetterWidth / 2) + 2;
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

    dst.x = x + i*_lettersDistance;

    switch (anchor)
    {
      case AlignRight:
        dst.x -= strLength * (int)_scaledLetterWidth;
        break;

      case AlignCenter:
        dst.x -= (strLength / 2) * (int)_scaledLetterWidth;
        break;

      case AlignLeft:
      default:
        break;
    }

    SDL_RenderCopy(VideoSystem::Get().Renderer(), _font.get()->Texture(), &src, &dst);
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
