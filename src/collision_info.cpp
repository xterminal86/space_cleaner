#include "collision_info.h"

CollisionInfo::CollisionInfo()
{
  TranslatedColliderRef = nullptr;
  TriangulatedTranslatedColliderRef = nullptr;
  SatAxesV2Ref = nullptr;
}

CollisionInfo::CollisionInfo(std::vector<Vector2>* satAxesRef, std::vector<SDL_Point>* translatedColliderRef)
{
  TranslatedColliderRef = translatedColliderRef;
  SatAxesV2Ref = satAxesRef;
}

CollisionInfo::CollisionInfo(std::vector<Vector2>* satAxesRef, std::vector<std::vector<SDL_Point>>* translatedColliderRef)
{
  SatAxesV2Ref = satAxesRef;
  TriangulatedTranslatedColliderRef = translatedColliderRef;
}

CollisionInfo::~CollisionInfo()
{
  //dtor
}
