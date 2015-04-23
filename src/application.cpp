#include "application.h"

// If we move the following include to application.h, as we would like to intuitively,
// we'll get cyclic dependencies due to incomplete Application class definition.
// So we include all the interface of Application first and then include GameState definition,
// which is included in mainstate.h
//
// So, in short, we would get error either on GameState::ChangeState due to incomplete
// type of Application, or on Application::Start -> ChangeState(...) call
// (compilation error info is strange - something about missing } bracket).
//
// TLDR: The key to understand the problem, that is described by these comments,
// is to remember, that we pass .cpp files to the compiler.
#include "introstate.h"

#include "mainstate.h"

Application::Application()
{
  char buf[512];
  FILE* f = fopen("version", "r");
  if (f != nullptr)
  {
    fgets(buf, sizeof(buf), f);
  }
  fclose(f);

  _buildVersion = buf;

  LoadHighScores();

  /*
  for (auto& i : _highScores)
  {
    printf ("%i %i\n", i.Score, i.Wave);
  }
  */
}

Application::~Application()
{
}

void Application::Start()
{
  _running = true;

  ChangeState(&IntroState::Get());
  //PushState(&MainState::Get());

  while (_running)
  {
    GameTime::Get().MeasureBefore();

    HandleEvents();
    Update();
    Draw();

    GameTime::Get().MeasureAfter();
  }
}

void Application::ChangeState(GameState* newState)
{
	if ( !_states.empty() )
  {
		_states.back()->Cleanup();
		_states.pop_back();
	}

	_states.push_back(newState);
	_states.back()->Init(this);
}

void Application::PushState(GameState* newState)
{
	if ( !_states.empty() )
  {
		_states.back()->Pause();
	}

	_states.push_back(newState);
	_states.back()->Init(this);
}

void Application::PopState()
{
	if ( !_states.empty() )
  {
		_states.back()->Cleanup();
		_states.pop_back();
	}

	if ( !_states.empty() )
  {
		_states.back()->Resume();
	}
}

bool Application::LoadHighScores()
{
  _scoresSorted = false;

  _highScores.clear();

  FILE* f = fopen((char*)GlobalStrings::HighScoresFilename.data(), "rb");
  if (f != nullptr)
  {
    fseek(f, 0, SEEK_END);
    long int size = ftell(f);
    if (size == 0)
    {
      fclose(f);
      return false;
    }

    rewind(f);

    while (1)
    {
      HighScore score;
      int ret = fread(&score, sizeof(HighScore), 1, f);
      if (ret != 1) break;
      _highScores.push_back(score);
    }

    fclose(f);
    return true;
  }

  fclose(f);

  return false;
}

void Application::StoreHighScore(HighScore score)
{
  LoadHighScores();

  FILE* f = fopen((char*)GlobalStrings::HighScoresFilename.data(), "ab");
  if (f != nullptr)
  {
    if (_highScores.size() == 0 || _highScores.size() < _highScoreMaxEntries)
    {
      fwrite(&score, sizeof(HighScore), 1, f);
    }
    else if (_highScores.size() == _highScoreMaxEntries)
    {
      freopen((char*)GlobalStrings::HighScoresFilename.data(), "wb", f);

      for (auto& item : _highScores)
      {
        if (item.Score <= score.Score)
        {
          item.Score = score.Score;
          item.Wave = score.Wave;
          break;
        }
      }

      for (auto& item : _highScores)
      {
        fwrite(&item, sizeof(HighScore), 1, f);
      }
    }
  }

  fclose(f);
}

void Application::SortHighScores()
{
  if (!_scoresSorted)
  {
    for (int i = 0; i < _highScores.size(); i++)
    {
      for (int j = 0; j < _highScores.size() - 1; j++)
      {
        if (_highScores[j + 1].Score > _highScores[j].Score)
        {
          int tmpScore = _highScores[j + 1].Score;
          int tmpWave = _highScores[j + 1].Wave;
          _highScores[j + 1].Score = _highScores[j].Score;
          _highScores[j + 1].Wave = _highScores[j].Wave;

          _highScores[j].Score = tmpScore;
          _highScores[j].Wave = tmpWave;
        }
      }
    }

    _scoresSorted = true;
  }
}

// ==================== Private Methods =================== //

void Application::HandleEvents()
{
  _states.back()->HandleEvents(this);
}

void Application::Update()
{
  _states.back()->Update(this);
}

void Application::Draw()
{
  _states.back()->Draw(this);
}
