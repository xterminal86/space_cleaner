#include "powerups_manager.h"

PowerupsManager::PowerupsManager()
{
  //ctor
}

PowerupsManager::~PowerupsManager()
{
  //dtor
}

void PowerupsManager::Init()
{
  int poolSizeHalf = _powerupsPoolSize / 2;

  for (int i = 0; i < _powerupsPoolSize; i++)
  {
    Powerup p;

    if (i % 8 == 0)
    {
      p.Create(Powerups::LIFE_POWERUP);
    }
    else if (i % 2 == 0)
    {
      p.Create(Powerups::HEALTH_POWERUP);
    }
    else if (i % 2 != 0)
    {
      p.Create(Powerups::SHIELD_POWERUP);
    }

    _powerupsPool.push_back(p);
  }
}

void PowerupsManager::Process()
{
  for (auto& powerup : _powerupsPool)
  {
    powerup.Process();
  }
}

void PowerupsManager::SpawnPowerup(int x, int y, int type)
{
  for (auto& p : _powerupsPool)
  {
    if (!p.Active() && p.Type() == type)
    {
      p.Spawn(Vector2(x, y));
      AnimationsManager::Get().Play(AnimationsIds::SPAWN_SMALL, p.Position().X(), p.Position().Y(), 2.0);
      SoundSystem::Get().PlaySound(Sounds::POWERUP_SPAWN);
      break;
    }
  }
}

void PowerupsManager::DespawnPowerups()
{
  for (auto& p : _powerupsPool)
  {
    if (p.Active())
    {
      p.SetActive(false);
    }
  }
}
