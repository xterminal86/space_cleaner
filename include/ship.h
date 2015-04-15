#ifndef SHIP_H
#define SHIP_H

#include "asteroid.h"
#include "vector2.h"
#include "sprite.h"
#include "bullet.h"

// TODO: think about extending the usage of this class: include support of many ships,
// i.e. make constructor with argument that is index on ship's sprite, or something like that.
class Ship
{
  public:
    Ship();
    virtual ~Ship();
    void Init(double posx, double posy);
    void Accelerate(double dspeed);
    void Move(int x, int y);
    void Move(Vector2 newPos);
    void Move();
    void Draw(bool drawCollider = false);
    void ComputeBullets();
    void ProcessCollision(Asteroid* collidedAsteroid);
    void ProcessShieldCollision(Asteroid* collidedAsteroid);
    void Rotate(double angle);
    void Fire();
    void Scale(double scaleFactor);
    void Respawn();
    void SetActive(bool status)
    {
      _active = status;

      if (!status)
      {
         _engineTrail.TurnOff();
      }
    }
    bool ShieldActive() { return _shieldPoints > 0; }
    bool Active() { return _active; }
    double Angle() { return _angle; }
    int HitPoints() { return _hitPoints; }
    int ShieldPoints() { return _shieldPoints; }
    void AddHitPoints(int value)
    {
      _hitPoints += value;

      if (_hitPoints >= ShipMaxHitPoints)
      {
        _hitPoints = ShipMaxHitPoints;
      }
    }
    void AddShieldPoints(int value)
    {
      _shieldPoints += value;

      if (_shieldPoints >= ShieldMaxPoints)
      {
        _shieldPoints = ShieldMaxPoints;
      }
    }
    bool HasBulletsActive();
    std::vector<std::unique_ptr<Bullet>>& GetBullets() { return _bullets; }

    std::string& HitPointsBar() { return _hitPointsBar; }
    std::string& ShieldPointsBar() { return _shieldPointsBar; }

    double Speed() { return _speed; }
    void SetSpeed(double val) { _speed = val; }

    Sprite& GetSprite() { return _shipSprite; }

    Vector2& Position() { return _position; }
    Vector2& Direction() { return _localDirection; }

    const int MaxBullets = 5;
    const int EngineTrailParticles = 50;
    const int ShipMaxHitPoints = 20;
    const int ShieldMaxPoints = 20;
    const double BulletSpeed = 0.5;
    const double ShipMaxSpeed = 4.0;
    const double RotationSpeed = 0.1;
    const double AccelerationSpeed = 0.005;
    const double DefaultParticleScale = 0.15;
    const double DefaultParticleScaleIncrement = 0.01;
    const int DefaultShieldRadius = 60;
    const int ShieldFadeSpeed = 5;

  protected:
  private:
    ParticleEngine _engineTrail;
    Sprite _shipSprite;
    Sprite _shieldSprite;

    SDL_Color _shieldColor;

    bool _active;
    bool _shieldHit;

    double _angle;
    double _speed;
    double _scaleFactor;

    int _hitPoints;
    int _shieldPoints;

    Vector2 _position;

    Vector2 _originalDirection;
    Vector2 _localDirection;

    Vector2 _originalEnginePoint;
    Vector2 _enginePointRotated;
    Vector2 _enginePointTranslated;

    std::vector<std::unique_ptr<Bullet>> _bullets;

    std::string _hitPointsBar;
    std::string _shieldPointsBar;

    void Draw(int x, int y, bool drawCollider = false);
    void ResetEnginePoint(Vector2 newEnginePoint);
    void DrawShield();
    void MakeBars();
};

#endif // SHIP_H
