#ifndef MAINSTATE_H
#define MAINSTATE_H

#include "gamestate.h"

class MainState : public GameState
{
  public:
    static MainState& Get()
    {
      static MainState _instance;
      return _instance;
    }

    void Init() override;
    void Cleanup() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents(Application* game) override;
    void Update(Application* game) override;
    void Draw(Application* game) override;

  protected:
    MainState();
    virtual ~MainState();

  private:
    Uint8* _keyboardState;
};

#endif // MAINSTATE_H
