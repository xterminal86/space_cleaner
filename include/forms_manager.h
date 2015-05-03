#ifndef FORMSMANAGER_H
#define FORMSMANAGER_H

#include "globals.h"
#include "form.h"
#include "application.h"
#include "mainstate.h"

// Holds all menu forms (read - menus and submenus)
class FormsManager
{
  public:
    static FormsManager& Get()
    {
      static FormsManager instance;
      return instance;
    }
    void Init();
    void FormDraw();
    void FormHandleInput(Uint8* keyboardState);
    void ShowForm(int formIndex);
    void ExitGame() { _gameRef->SetRunningFlag(false); }
    void NewGame() { _gameRef->PushState(&MainState::Get()); }
    void SetApplicationPointer(Application* gameRef) { _gameRef = gameRef; }
    Application* GetApplicationPointer() { return _gameRef; }
    void CloseCurrentForm();
  protected:
  private:
    FormsManager();
    virtual ~FormsManager();
    FormsManager(const FormsManager&);
    FormsManager& operator= (const FormsManager&);

    Application* _gameRef;

    std::map<int, std::unique_ptr<Form>> _formsPool;
    std::vector<Form*> _formsQueue;
};

#endif // FORMSMANAGER_H
