#ifndef UTIL_H
#define UTIL_H

#include "sprite.h"
#include "vector2.h"
#include "vector2pair.h"

class Util
{
  public:
    Util();
    virtual ~Util();

    static Vector2Pair ProjectPolygon(std::vector<SDL_Point>& polygon, Vector2& axe)
    {
      Vector2Pair proj;

      Vector2 projMin;
      Vector2 projMax;

      double min = polygon[0].x * axe.X() + polygon[0].y * axe.Y();
      double max = min;
      for (auto &i : polygon)
      {
        Vector2 tmp;
        tmp.Set(i.x, i.y);

        double scalar = tmp * axe;

        if (scalar < min) min = scalar;
        else if (scalar > max) max = scalar;
      }

      projMin.Set(axe.X() * min, axe.Y() * min);
      projMax.Set(axe.X() * max, axe.Y() * max);

      proj.Set(projMin, projMax);

      return proj;
    }

    static bool TestIntersection(Sprite& firstSprite, Sprite& secondSprite)
    {
      firstSprite.CalculateSATAxes();
      secondSprite.CalculateSATAxes();

      auto fisrstSpriteAxes = firstSprite.GetAxesV2();
      auto secondSpriteAxes = secondSprite.GetAxesV2();

      bool collisionFlag = true;

      for (auto &i : fisrstSpriteAxes)
      {
        Vector2Pair selfProjection = ProjectPolygon(firstSprite.TranslatedCollider(), i);
        Vector2Pair otherProjection = ProjectPolygon(secondSprite.TranslatedCollider(), i);

        collisionFlag = (collisionFlag && Vector2Pair::TestIntersection(selfProjection, otherProjection));

        if (!collisionFlag)
        {
          return false;
        }
      }

      for (auto &i : secondSpriteAxes)
      {
        Vector2Pair selfProjection = ProjectPolygon(secondSprite.TranslatedCollider(), i);
        Vector2Pair otherProjection = ProjectPolygon(firstSprite.TranslatedCollider(), i);

        collisionFlag = (collisionFlag && Vector2Pair::TestIntersection(selfProjection, otherProjection));

        if (!collisionFlag)
        {
          return false;
        }
      }

      return true;
    }

    static void CreateRandomPosition(Vector2& pos, int pxLimitX, int pxLimitY)
    {
      double px = (double)((rand() % pxLimitX - 40) + 40);
      double py = (double)((rand() % pxLimitY - 40) + 40);

      pos.Set(px, py);
    }

    static void CreateRandomDirection(Vector2& dir)
    {
      double dx = (double)(rand() % 10 + 1);
      double dy = (double)(rand() % 10 + 1);

      int sign = rand() % 11;

      if (sign % 2 == 0)
      {
        dx *= -1;
      }
      else if (sign % 3 == 0)
      {
        dy *= -1;
      }
      else
      {
        dx *= -1;
        dy *= -1;
      }

      dir.Set(dx, dy);
      dir.Normalize();
    }

    static double CreateRandomRotation()
    {
      double angleIncrement = 1.0 / (double)(rand() % RotationSpeedSpread + 1);

      int sign = rand() % 4 + 1;

      if (sign % 2 == 0)
      {
        angleIncrement *= -1;
      }

      return angleIncrement;
    }

    static const int SpeedSpread = 10;
    static const int RotationSpeedSpread = 10;

  protected:
  private:
};

#endif // UTIL_H
