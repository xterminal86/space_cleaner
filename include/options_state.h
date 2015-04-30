#ifndef OPTIONS_STATE_H
#define OPTIONS_STATE_H

#include "gamestate.h"


class OptionsState : public GameState
{
  public:
    static OptionsState& Get()
    {
      static OptionsState instance;
      return instance;
    }
    void Init(Application* game) override;
    void Cleanup() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents(Application* game) override;
    void Update(Application* game) override;
    void Draw(Application* game) override;
  protected:
  private:
    OptionsState();
    virtual ~OptionsState();
    OptionsState(const OptionsState&);
    OptionsState& operator= (const OptionsState&);

    Uint8* _keyboardState;
};

#endif // OPTIONS_STATE_H
