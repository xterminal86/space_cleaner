#ifndef FORM_H
#define FORM_H

#include "globals.h"
#include "bitmapfont.h"

class Form
{
  public:
    Form();
    virtual ~Form();

    virtual void Draw() = 0;
    virtual void HandleInput(Uint8* keyboardState) = 0;
    virtual void Open() = 0;
    virtual void Close() = 0;

    int CurrentMenuSelection() { return _currentMenuSelection; }
    void SetMenuSelection(int value) { _currentMenuSelection = value; }

  protected:
    BitmapFont* _bitmapFont = &BitmapFont::Get();
    int _currentMenuSelection;
    bool _keyPressed;

  private:
};

struct MenuItem
{
  int Id = 0;
  std::string TextValue;
  void (*SelectHandler)() = nullptr;
  void (*LeftHandler)() = nullptr;
  void (*RightHandler)() = nullptr;
};

#endif // FORM_H
