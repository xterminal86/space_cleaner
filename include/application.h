#ifndef APPLICATION_H
#define APPLICATION_H

#include "globals.h"

class GameState;

class Application
{
  public:
    Application();
    virtual ~Application();
    void Start();
    void ChangeState(GameState* newState);
    void PushState(GameState* newState);
    void PopState();
    void SetRunningFlag(bool value) { _running = value; }
    std::string& BuildVersionString() { return _buildVersion; }
    GameState* GetCurrentState() { return _states.back(); }
    bool LoadHighScores();
    void StoreHighScore(HighScore score);
    std::vector<HighScore>& HighScores()
    {
      LoadHighScores();
      return _highScores;
    }
  protected:
  private:
    bool _running;

    std::string _buildVersion;

    std::vector<GameState*> _states;
    std::vector<HighScore> _highScores;

    const unsigned int _highScoreMaxEntries = 10;

    void HandleEvents();
    void Update();
    void Draw();
};

#endif // APPLICATION_H
