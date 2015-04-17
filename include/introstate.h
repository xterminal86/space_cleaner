#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "gamestate.h"

class IntroState : public GameState
{
  public:
    static IntroState* Get()
    {
      return &_instance;
    }

    void Init() override;
    void Cleanup() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents(Application* game) override;
    void Update(Application* game) override;
    void Draw(Application* game) override;

  protected:
    IntroState();
    virtual ~IntroState();

  private:
    static IntroState _instance;
};

#endif // INTROSTATE_H
