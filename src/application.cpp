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
}

Application::~Application()
{
}

void Application::Start()
{
  _running = true;

  ChangeState(&IntroState::Get());

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
