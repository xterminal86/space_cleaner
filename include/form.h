#ifndef FORM_H
#define FORM_H

#include "globals.h"
#include "bitmapfont.h"

// Base class for maintaining title screen menus and submenus
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

// Any menu item
struct MenuItem
{
  int Id = 0;
  std::string TextValue;
  // Function to be called on ENTER key
  void (*SelectHandler)() = nullptr;
  // Function to be called on LEFT key
  void (*LeftHandler)() = nullptr;
  // Function to be called on RIGHT key
  void (*RightHandler)() = nullptr;
  // Repeated calls
  void (*LeftHandlerAuto)() = nullptr;
  void (*RightHandlerAuto)() = nullptr;
};

#endif // FORM_H
