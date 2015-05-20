#include "application.h"

int main( int argc, char* args[])
{
  Application::Get().Init();
  Application::Get().Start();
	return 0;
}
