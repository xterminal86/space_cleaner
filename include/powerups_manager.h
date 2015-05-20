#ifndef POWERUPSMANAGER_H
#define POWERUPSMANAGER_H

#include "globals.h"
#include "powerup.h"
#include "animations_manager.h"
#include "sound_system.h"

// Holds all powerups and spawns them on demand
class PowerupsManager
{
  public:
    static PowerupsManager& Get()
    {
      static PowerupsManager instance;
      return instance;
    }
    void Init();
    void Process();
    void SpawnPowerup(int x, int y, int type);
    void DespawnPowerups();
    std::vector<Powerup>& PowerupsPool() { return _powerupsPool; }
    void ResetActivePowerupFlag() { _currentActivePowerup = Powerups::NONE; }
  protected:
  private:
    const int _powerupsPoolSize = 16;

    int _currentActivePowerup;

    PowerupsManager();
    virtual ~PowerupsManager();
    PowerupsManager& operator=(const PowerupsManager&);
    PowerupsManager(const PowerupsManager&);

    std::vector<Powerup> _powerupsPool;
};

#endif // POWERUPSMANAGER_H
