#ifndef POWERUP_H
#define POWERUP_H

#include "sprite.h"
#include "gametime.h"

class Powerup
{
  public:
    Powerup();
    virtual ~Powerup();
    void Create(int type);
    void Spawn(Vector2 position);
    void SetActive(bool flag) { _active = flag; }
    bool Active() { return _active; }
    void Process();
    int Type() { return _type; }
    Sprite& GetSprite() { return _sprite; }
    Vector2& Position() { return _position; }
  protected:
  private:
    Vector2 _position;

    const double _defaultScaleFactor = 0.4;
    const double _scaleIncrement = 0.001;
    const double _maximumScaleFactor = 0.5;
    const double _minimalScaleFactor = 0.3;

    double _scaleFactor;
    double _currentScaleIncrement;

    unsigned int _lifeTimeMs;
    unsigned int _timePassedMs;

    int _type;

    bool _active;

    Sprite _sprite;

    void CheckBounds();
};

#endif // POWERUP_H
