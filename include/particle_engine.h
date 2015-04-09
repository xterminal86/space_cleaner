#ifndef PARTICLEENGINE_H
#define PARTICLEENGINE_H

#include "gametime.h"
#include "util.h"
#include "png_loader.h"
#include "vector2.h"

struct Particle
{
  int CurrentLifeTimeMs;
  int MaxLifeTimeMs;
  bool Active;
  Vector2 Position;
  Vector2 Direction;
  double Speed;
  double ScaleFactor;
  double Angle;
};

class ParticleEngine
{
  public:
    ParticleEngine();
    virtual ~ParticleEngine();
    void Init(int particlesNumber, int lifeTimeMsMin, int lifetimeMsMax, double particleScaleIncrement, double scaleFactor, PNGLoader* particleImage);
    bool Active() { return _active; }
    void TurnOff() { _active = false; }
    void TurnOn();
    void SetUp(Vector2 pos, Vector2 dir, double speed, double angle);
    void SetLifeAndSpeed(int lifeTimeMsMin, int lifeTimeMsMax, double speed);
    void MoveOrigin(Vector2 newPos) { _position = newPos; }
    void Emit();
  protected:
  private:
    bool _active;
    int _numParticles;
    int _particlesLifeTimeMsMin;
    int _particlesLifeTimeMsMax;
    double _speed;
    double _particleScaleIncrement;
    double _particleScaleFactor;
    Vector2 _position;
    Vector2 _direction;
    PNGLoader* _particleImage;

    SDL_Rect _srcRect;
    SDL_Rect _dstRect;

    std::vector<Particle> _particles;
};

#endif // PARTICLEENGINE_H
