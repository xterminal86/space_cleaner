#include "collision_info.h"

CollisionInfo::CollisionInfo()
{
}

CollisionInfo::CollisionInfo(std::vector<Vector2>* satAxesRef, std::vector<SDL_Point>* translatedColliderRef)
{
  TranslatedColliderRef = translatedColliderRef;
  SatAxesV2Ref = satAxesRef;
}

CollisionInfo::~CollisionInfo()
{
  //dtor
}
