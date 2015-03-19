#ifndef ASTEROID_H
#define ASTEROID_H

#include "gametime.h"
#include "sprite.h"
#include "vector2.h"
#include "util.h"

class Asteroid
{
  public:
    Asteroid(double posx, double posy, int breakdownLevel, std::vector<std::unique_ptr<Asteroid>>* mainAsteroidsCollection);
    Asteroid(Vector2 pos, int breakdownLevel, std::vector<std::unique_ptr<Asteroid>>* mainAsteroidsCollection);
    virtual ~Asteroid();
    void Draw(bool drawCollider = false, bool drawAxes = false);
    void Move(Vector2 newPos);
    void Move(double x, double y);
    void Move();
    void Rotate(double angle);
    void Compute();
    void ProcessCollision();

    Vector2& Position() { return _position; }

    Sprite& GetSprite() { return _asteroidSprite; }

    bool Active() { return _active; }

  protected:
  private:
    Sprite _asteroidSprite;

    // Comes from Application
    std::vector<std::unique_ptr<Asteroid>>* _mainAsteroidsCollectionReference;

    bool _active;

    int _currentBreakdownLevel;

    double _speed;
    double _angle;
    double _angleIncrement;

    Vector2 _position;
    Vector2 _direction;

    void Breakdown();
    void Draw(int x, int y, bool drawCollider = false);
    void DrawAxes();
    void Init(Vector2 pos, int breakdownLevel, std::vector<std::unique_ptr<Asteroid>>* mainAsteroidsCollection);
};

#endif // ASTEROID_H
