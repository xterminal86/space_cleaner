#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

#include "globals.h"
#include "vector2.h"

class CollisionInfo
{
public:
  CollisionInfo();
  CollisionInfo(std::vector<Vector2>* satAxesRef, std::vector<SDL_Point>* translatedColliderRef);
  CollisionInfo(std::vector<Vector2>* satAxesRef, std::vector<std::vector<SDL_Point>>* translatedColliderRef);
  virtual ~CollisionInfo();

  bool ConvexFlag;
  std::vector<Vector2>* SatAxesV2Ref;
  std::vector<SDL_Point>* TranslatedColliderRef;
  std::vector<std::vector<SDL_Point>>* TriangulatedTranslatedColliderRef;
};


#endif // COLLISIONINFO_H
