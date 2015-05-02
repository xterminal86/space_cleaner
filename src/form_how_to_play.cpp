#include "form_how_to_play.h"

FormHowToPlay::FormHowToPlay()
{
  //ctor
}

FormHowToPlay::~FormHowToPlay()
{
  //dtor
}

void FormHowToPlay::Draw()
{
  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Print(VideoSystem::Get().ScreenDimensions().x / 2,
                     VideoSystem::Get().ScreenDimensions().y / 2,
                     BitmapFont::AlignCenter,
                    (char*)GlobalStrings::HowToPlayString.data());
}

void FormHowToPlay::HandleInput(Uint8* keyboardState)
{
}

void FormHowToPlay::Open()
{
}

void FormHowToPlay::Close()
{
}
