#ifndef VECTOR2PAIR_H
#define VECTOR2PAIR_H

#include "vector2.h"

class Vector2Pair
{
  public:
    Vector2Pair();
    Vector2Pair(Vector2 min, Vector2 max);
    virtual ~Vector2Pair();

    void Set(Vector2 min, Vector2 max);

    Vector2& Min() { return _min; }
    Vector2& Max() { return _max; }
    Vector2& Direction()
    {
      double dx = _max.X() - _min.X();
      double dy = _max.Y() - _min.Y();

      _dir.Set(dx, dy);

      return _dir;
    }

    double& Length() { return _length; }

    void Swap()
    {
      Vector2 tmp;
      tmp.Set(_min.X(), _min.Y());
      _min = _max;
      _max = tmp;
    }

    static bool TestIntersection(Vector2Pair f, Vector2Pair s)
    {
      // Sometimes order of f.Min and f.Max might be reversed: e.g. right to left instead of left to right accordingly for X.
      // In such case we swap values.
      // This is done to avoid writing another if case for the above situation.

      if (f.Min().X() > f.Max().X() || f.Min().Y() > f.Max().Y())
      {
        f.Swap();
      }

      if (s.Min().X() > s.Max().X() || s.Min().Y() > s.Max().Y())
      {
        s.Swap();
      }

      // Using strict conditions leads to incorrect collision detection: e.g. when ship stayed on collision
      // with asteroid for some time, collision detection result fluctuated (in some frames it could be false,
      // while most of the time it was true). I replaced test clauses with non-strict conditional,
      // and it seems to have fixed the issue.
      // I guess the problem was in precision loss, because in the essence sprite colliders are just
      // SDL_Points (integers) which then are passed around to different methods for calculation in double precision,
      // so there might be a situation where SAT projection degenerated and then its value went on to calculation.
      // So, I thought, I might try to add non-strict conditions instead strict ones.

      if ( (f.Max().X() >= s.Min().X() && f.Max().X() <= s.Max().X()) ||
           (f.Max().X() >= s.Max().X() && f.Min().X() <= s.Min().X()) ||
           (f.Max().X() <= s.Max().X() && f.Min().X() >= s.Min().X()) ||
           (f.Min().X() >= s.Min().X() && f.Min().X() <= s.Max().X()) )
      {
        return true;
      }

      if ( (f.Max().Y() >= s.Min().Y() && f.Max().Y() <= s.Max().Y()) ||
           (f.Max().Y() >= s.Max().Y() && f.Min().Y() <= s.Min().Y()) ||
           (f.Max().Y() <= s.Max().Y() && f.Min().Y() >= s.Min().Y()) ||
           (f.Min().Y() >= s.Min().Y() && f.Min().Y() <= s.Max().Y()) )
      {
        return true;
      }

      return false;
    }

  protected:
  private:
    Vector2 _min;
    Vector2 _max;
    Vector2 _dir;

    double _length;
};

#endif // VECTOR2PAIR_H
