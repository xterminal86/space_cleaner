#include "form_high_scores.h"
#include "forms_manager.h"

FormHighScores::FormHighScores()
{
  //ctor
}

FormHighScores::~FormHighScores()
{
  //dtor
}

void FormHighScores::Draw()
{
  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(2.0);
  _bitmapFont->Printf(VideoSystem::Get().ScreenDimensions().x / 2 - 200, VideoSystem::Get().ScreenDimensions().y / 2 - 70, BitmapFont::AlignLeft, "No.");
  _bitmapFont->Printf(VideoSystem::Get().ScreenDimensions().x / 2 - 100, VideoSystem::Get().ScreenDimensions().y / 2 - 70, BitmapFont::AlignLeft, "Score");
  _bitmapFont->Printf(VideoSystem::Get().ScreenDimensions().x / 2 + 100, VideoSystem::Get().ScreenDimensions().y / 2 - 70, BitmapFont::AlignLeft, "Wave");
  _bitmapFont->SetScale(1.0);

  for (int i = 0; i < FormsManager::Get().GetApplicationPointer()->HighScores().size(); i++)
  {
    _bitmapFont->Printf(VideoSystem::Get().ScreenDimensions().x / 2 - 200, VideoSystem::Get().ScreenDimensions().y / 2 + i*(_bitmapFont->LetterWidth * _bitmapFont->ScaleFactor()), BitmapFont::AlignLeft, "%i", i + 1);
    _bitmapFont->Printf(VideoSystem::Get().ScreenDimensions().x / 2 - 100, VideoSystem::Get().ScreenDimensions().y / 2 + i*(_bitmapFont->LetterWidth * _bitmapFont->ScaleFactor()), BitmapFont::AlignLeft, "%i", FormsManager::Get().GetApplicationPointer()->HighScores().at(i).Score);
    _bitmapFont->Printf(VideoSystem::Get().ScreenDimensions().x / 2 + 100, VideoSystem::Get().ScreenDimensions().y / 2 + i*(_bitmapFont->LetterWidth * _bitmapFont->ScaleFactor()), BitmapFont::AlignLeft, "%i", FormsManager::Get().GetApplicationPointer()->HighScores().at(i).Wave);
  }
}

void FormHighScores::HandleInput(Uint8* keyboardState)
{
}

void FormHighScores::Open()
{
  FormsManager::Get().GetApplicationPointer()->LoadHighScores();
  FormsManager::Get().GetApplicationPointer()->SortHighScores();
}

void FormHighScores::Close()
{
}
