#ifndef VECTOR2_H
#define VECTOR2_H

#include "globals.h"

class Vector2
{
  public:
	Vector2();
	Vector2(const Vector2& vector2) : _x(vector2._x), _y(vector2._y) {}
	Vector2(double array[])
	{
		_x = array[0];
		_y = array[1];
	}
	Vector2(double ax, double ay)
	{
		_x = ax;
		_y = ay;
	}
	virtual ~Vector2();

	double X() { return _x; }
	double Y() { return _y; }

	Vector2 Get() { return Vector2(_x, _y); }
	void Set(double ax, double ay, bool normalize = false)
	{
		_x = ax;
		_y = ay;

		if (normalize)
    {
      Normalize();
    }
	}
	void Set(Vector2 vector)
	{
		Set(vector._x, vector._y);
	}

	void ToZero()
	{
		_x = 0.0;
		_y = 0.0;
	}

	void Normalize()
	{
		double len = sqrt(_x*_x + _y*_y);

		_x /= len;
		_y /= len;
	}

	double GetLength() { return sqrt(_x*_x + _y*_y); }

	Vector2 operator- (Vector2& rhs)
	{
		return Vector2(_x - rhs.X(), _y - rhs.Y());
	}

	Vector2 operator+ (Vector2& rhs)
	{
		return Vector2(_x + rhs.X(), _y + rhs.Y());
	}

  // Dot product
	double operator* (Vector2& rhs)
	{
		return _x*rhs.X() + _y*rhs.Y();
	}

  // This is a hack: in R2 there is no such thing as "cross product", but for utility sake
  // we imply, that we are actually dealing with R3 with zero in z coordinate of our vectors.
  // Thus we just return direction component of Z (vector pointing outside or inside the screen).
  // s1 = y1z2 - z1y2, s2 = z1x2 - x1z2, *** s3 = x1y2 - y1x2 ***
  double CrossProduct(Vector2& rhs)
  {
    return (_x*rhs.Y() - rhs.X()*_y);
  }

  // Scale
	Vector2 operator* (double scalar)
	{
		return Vector2(_x * scalar, _y * scalar);
	}

  Vector2& operator= (Vector2& rhs)
  {
    _x = rhs.X();
    _y = rhs.Y();

    return *this;
  }

	Vector2 GetPerpendicular()
	{
		return Vector2(-_y, _x);
	}

  void Invert(bool invertX)
  {
    if (invertX)
    {
      _x = -_x;
    }
    else
    {
      _y = -_y;
    }
  }

	static Vector2 Zero() {	return Vector2(0.0, 0.0); }
  static Vector2 Up() { return Vector2(0.0, 1.0); }

  // Explanation from previous case:
  //
  // By default rotation is assumed around (0; 0). It is located in the top left corner of the image.
  // Our actual rotation center is image's (w / 2; h / 2). So, first, we move it to (0; 0) (which means, that collider's coordinates are "offseted"),
  // then rotate collider around it, and then move it back ("offset" collider's coordinates back).

  static void RotateVector(Vector2& result, Vector2 around, Vector2 what, double angle)
  {
    double nx = around.X() + (what.X() - around.X()) * SDL_cos(angle * Math::PIOVER180) - (what.Y() - around.Y()) * SDL_sin(angle * Math::PIOVER180);
    double ny = around.Y() + (what.X() - around.X()) * SDL_sin(angle * Math::PIOVER180) + (what.Y() - around.Y()) * SDL_cos(angle * Math::PIOVER180);

    result.Set(nx, ny);
  }

  Vector2 ProjectPoint(Vector2 point)
  {
    Normalize();

    double scalar = _x*point.X() + _y*point.Y();

    return Vector2(_x*scalar, _y*scalar);
  }

  SDL_Point& ToSDL_Point()
  {
    _point.x = (int)_x;
    _point.y = (int)_y;

    return _point;
  }

private:
	double _x;
	double _y;

	SDL_Point _point;
};

#endif // VECTOR2_H
