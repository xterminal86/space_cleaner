#include "forms_manager.h"
#include "form_main_title.h"
#include "form_how_to_play.h"
#include "form_high_scores.h"
#include "form_options.h"

FormsManager::FormsManager()
{
  //ctor
}

FormsManager::~FormsManager()
{
  //dtor
}

void FormsManager::Init()
{
  _formsPool[Forms::MAIN_TITLE] = std::unique_ptr<Form>(new FormMainTitle());
  _formsPool[Forms::HOW_TO_PLAY] = std::unique_ptr<Form>(new FormHowToPlay());
  _formsPool[Forms::HIGH_SCORES] = std::unique_ptr<Form>(new FormHighScores());
  _formsPool[Forms::OPTIONS] = std::unique_ptr<Form>(new FormOptions());

  _formsQueue.push_back(_formsPool[Forms::MAIN_TITLE].get());
}

void FormsManager::ShowForm(int formIndex)
{
  if (_formsPool.count(formIndex) == 1)
  {
    _formsPool[formIndex].get()->Open();
    _formsQueue.push_back(_formsPool[formIndex].get());
  }
}

void FormsManager::FormDraw()
{
  if (!_formsQueue.empty())
  {
    _formsQueue.back()->Draw();
  }
}

void FormsManager::FormHandleInput(Uint8* keyboardState)
{
  if (!_formsQueue.empty())
  {
    _formsQueue.back()->HandleInput(keyboardState);
  }
}

void FormsManager::CloseCurrentForm()
{
  if (!_formsQueue.empty())
  {
    if (_formsQueue.size() > 1)
    {
      SoundSystem::Get().PlaySound(Sounds::MENU_BACK);
      _formsQueue.back()->Close();
      _formsQueue.pop_back();
    }
  }
}
