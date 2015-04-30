#ifndef POWERUPSMANAGER_H
#define POWERUPSMANAGER_H

#include "globals.h"
#include "powerup.h"
#include "animations_manager.h"
#include "sound_system.h"

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
  protected:
  private:
    const int _powerupsPoolSize = 16;

    PowerupsManager();
    virtual ~PowerupsManager();
    PowerupsManager& operator=(const PowerupsManager&);
    PowerupsManager(const PowerupsManager&);

    std::vector<Powerup> _powerupsPool;
};

#endif // POWERUPSMANAGER_H
