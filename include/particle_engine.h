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
    void Init(int particlesNumber, int lifeTimeMsMin, int lifetimeMsMax, double particleScaleIncrement, PNGLoader* particleImage);
    bool Active() { return _active; }
    void SetActive(bool flag) { _active = flag; }
    void SetUp(Vector2 pos, Vector2 dir, double bulletSpeed, double angle);
    void MoveOrigin(Vector2 newPos) { _position = newPos; }
    void Emit();
  protected:
  private:
    bool _active;
    int _numParticles;
    int _particlesLifeTimeMsMin;
    int _particlesLifeTimeMsMax;
    double _bulletSpeed;
    double _particleScaleIncrement;
    Vector2 _position;
    Vector2 _direction;
    PNGLoader* _particleImage;

    SDL_Rect _srcRect;
    SDL_Rect _dstRect;

    std::vector<Particle> _particles;
};

#endif // PARTICLEENGINE_H
