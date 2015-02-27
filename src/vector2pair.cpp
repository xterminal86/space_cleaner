#include "vector2pair.h"

Vector2Pair::Vector2Pair()
{
  //ctor
}

Vector2Pair::Vector2Pair(Vector2 min, Vector2 max)
{
  _min = min;
  _max = max;

  Vector2 tmp = max - min;
  _length = tmp.GetLength();
}

Vector2Pair::~Vector2Pair()
{
  //dtor
}

void Vector2Pair::Set(Vector2 min, Vector2 max)
{
  _min = min;
  _max = max;

  Vector2 tmp = max - min;
  _length = tmp.GetLength();
}
