#include "bullet.h"

Bullet::Bullet()
{
  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletLaserRole);
  if (res != -1)
  {
    _bulletSprite.Init(res);
  }
}

Bullet::~Bullet()
{
  //dtor
}
