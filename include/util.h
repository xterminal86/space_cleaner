#ifndef UTIL_H
#define UTIL_H

#include "vector2.h"
#include "vector2pair.h"
#include "collision_info.h"

class Util
{
  public:
    Util();
    virtual ~Util();

    static Vector2Pair ProjectPolygon(std::vector<SDL_Point>* polygon, Vector2& axe)
    {
      Vector2Pair proj;

      Vector2 projMin;
      Vector2 projMax;

      double min = polygon->at(0).x * axe.X() + polygon->at(0).y * axe.Y();
      double max = min;
      for (int i = 0; i < polygon->size(); i++)
      {
        Vector2 tmp;
        tmp.Set(polygon->at(i).x, polygon->at(i).y);

        double scalar = tmp * axe;

        if (scalar < min) min = scalar;
        else if (scalar > max) max = scalar;
      }

      projMin.Set(axe.X() * min, axe.Y() * min);
      projMax.Set(axe.X() * max, axe.Y() * max);

      proj.Set(projMin, projMax);

      return proj;
    }

    static bool TestIntersection(CollisionInfo& firstSprite, CollisionInfo& secondSprite)
    {
      auto firstSpriteAxes = firstSprite.SatAxesV2Ref;
      auto secondSpriteAxes = secondSprite.SatAxesV2Ref;

      auto firstSpriteCollider = firstSprite.TranslatedColliderRef;
      auto secondSpriteCollider = secondSprite.TranslatedColliderRef;

      auto firstSpriteColliderTr = firstSprite.TriangulatedTranslatedColliderRef;
      auto secondSpriteColliderTr = secondSprite.TriangulatedTranslatedColliderRef;

      bool collisionFlag = false;

      // First is triangulated, second - isn't
      if (firstSpriteCollider == nullptr && secondSpriteCollider != nullptr)
      {
        for (auto& t : *firstSpriteColliderTr)
        {
          collisionFlag = ( AreCollidersIntersecting(&t, secondSpriteCollider, firstSpriteAxes) && AreCollidersIntersecting(&t, secondSpriteCollider, secondSpriteAxes) );

          if (collisionFlag) return true;
        }
      }
      // First is not triangulated, second - is
      else if (firstSpriteCollider != nullptr && secondSpriteCollider == nullptr)
      {
        for (auto& t : *secondSpriteColliderTr)
        {
          collisionFlag = ( AreCollidersIntersecting(&t, firstSpriteCollider, firstSpriteAxes) && AreCollidersIntersecting(&t, firstSpriteCollider, secondSpriteAxes) );

          if (collisionFlag) return true;
        }
      }
      // Both are triangulated
      else if (firstSpriteCollider == nullptr && secondSpriteCollider == nullptr)
      {
        for (auto& t1 : *firstSpriteColliderTr)
        {
          for (auto& t2 : *secondSpriteColliderTr)
          {
            collisionFlag = ( AreCollidersIntersecting(&t1, &t2, firstSpriteAxes) && AreCollidersIntersecting(&t1, &t2, secondSpriteAxes) );

            if (collisionFlag) return true;
          }
        }
      }
      // Both aren't triangulated
      else
      {
        collisionFlag = ( AreCollidersIntersecting(firstSpriteCollider, secondSpriteCollider, firstSpriteAxes) && AreCollidersIntersecting(firstSpriteCollider, secondSpriteCollider, secondSpriteAxes) );
      }

      return collisionFlag;
    }

    static void CreateRandomPosition(Vector2& pos, int pxLimitX, int pxLimitY)
    {
      double px = (double)((RandomNumber() % pxLimitX - 40) + 40);
      double py = (double)((RandomNumber() % pxLimitY - 40) + 40);

      pos.Set(px, py);
    }

    static void CreateRandomDirection(Vector2& dir)
    {
      double dx = (double)(RandomNumber() % 10 + 1);
      double dy = (double)(RandomNumber() % 10 + 1);

      int sign = RandomNumber() % 11;

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
      double angleIncrement = 1.0 / (double)(RandomNumber() % GameMechanic::RandomRotationSpeedSpread + 1);

      int sign = RandomNumber() % 4 + 1;

      if (sign % 2 == 0)
      {
        angleIncrement *= -1;
      }

      return angleIncrement;
    }

    static unsigned int RandomNumber()
    {
      static unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
      static std::mt19937 myRandomGenerator(seed);
      return myRandomGenerator();
    }

    static double Vector2Cross(Vector2 v1, Vector2 v2)
    {
      return (v1.X() * v2.Y() - v1.Y() * v2.X());
    }

    static bool AreLinesIntersecting(Vector2Pair l1, Vector2Pair l2, bool testEndPoints = false)
    {
      Vector2 d1 = l1.Direction();
      Vector2 d2 = l2.Direction();

      double t2dividend = d1.X() * (l2.Min().Y() - l1.Min().Y()) + d1.Y() * (l1.Min().X() - l2.Min().X());
      double t2divisor = Util::Vector2Cross(d2, d1);

      if (t2divisor == 0)
      {
        //Debug.Log(l1 + " " + l2 + " => d2xd2 = 0");
        return false;
      }

      double t2 = t2dividend / t2divisor;

      // By default, we substitute t2 in equation (1) to find t1.
      // If d1(x) happens to be 0, we use equation (2)

      double t1dividend = 0.0f;
      double t1divisor = d1.X();

      if (t1divisor == 0)
      {
        t1dividend = l2.Min().Y() + t2 * d2.Y() - l1.Min().Y();
        t1divisor = d1.Y();
      }
      else
      {
        t1dividend = l2.Min().X() + t2 * d2.X() - l1.Min().X();
      }

      double t1 = t1dividend / t1divisor;

      //Debug.Log ("t2 -> Dividend: " + t2dividend + " | Divisor: " + t2divisor + " = " + t2);
      //Debug.Log ("t1 -> Dividend: " + t1dividend + " | Divisor: " + t1divisor + " = " + t1);

      if (testEndPoints)
      {
        return (t1 >= 0.0 && t1 <= 1.0 && t2 >= 0.0 && t2 <= 1.0);
      }
      else
      {
        return (t1 > 0.0 && t1 < 1.0 && t2 > 0.0 && t2 < 1.0);
      }
    }

    static bool IsPointOnTheLine(Vector2Pair line, Vector2 p)
    {
      double t1 = ( p.X() - line.Min().X() ) / line.Direction().X();
      double t2 = ( p.Y() - line.Min().Y() ) / line.Direction().Y();

      return (t1 == t2);
    }

  protected:
  private:
    static bool AreCollidersIntersecting(std::vector<SDL_Point>* collider1Ref, std::vector<SDL_Point>* collider2Ref, std::vector<Vector2>* satAxesRef)
    {
      bool intersectionFlag = true;
      for (int i = 0; i < satAxesRef->size(); i++)
      {
        Vector2Pair projection1 = ProjectPolygon(collider1Ref, satAxesRef->at(i));
        Vector2Pair projection2 = ProjectPolygon(collider2Ref, satAxesRef->at(i));

        intersectionFlag = (intersectionFlag && Vector2Pair::TestIntersection(projection1, projection2));

        if (!intersectionFlag)
        {
          return false;
        }
      }

      return true;
    }

};

#endif // UTIL_H
