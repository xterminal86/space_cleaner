#ifndef FORM_MAIN_TITLE_H
#define FORM_MAIN_TITLE_H

#include "form.h"
#include "forms_manager.h"
#include "gametime.h"
#include "sound_system.h"

class FormMainTitle : public Form
{
  public:
    FormMainTitle();
    virtual ~FormMainTitle();
  protected:
    void Draw() override;
    void HandleInput(Uint8* keyboardState) override;
    void Open() override;
    void Close() override;
  private:
    const double _menuItemDefaultScale = 2.0;
    const double _menuItemMinimalScale = 1.5;
    const double _menuItemMaximumScale = 2.25;

    double _menuItemScaleIncrement = 0.005;
    double _menuItemScaleFactor;

    std::vector<std::string> _menuStrings;
    std::map<int, MenuItem> _menuItems;

    int _screenSizeX;
    int _screenSizeY;

    void PrintMenuText();
    void Update();

    static void NewGameHandler()
    {
      FormsManager::Get().NewGame();
    }

    static void OptionsHandler()
    {
      FormsManager::Get().ShowForm(Forms::OPTIONS);
    }

    static void HowToPlayHandler()
    {
      FormsManager::Get().ShowForm(Forms::HOW_TO_PLAY);
    }

    static void HighScoresHandler()
    {
      FormsManager::Get().ShowForm(Forms::HIGH_SCORES);
    }

    static void ExitGameHandler()
    {
      FormsManager::Get().ExitGame();
    }
};

#endif // FORM_MAIN_TITLE_H
