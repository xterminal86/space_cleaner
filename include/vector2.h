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
		_x = 0.0f;
		_y = 0.0f;
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

	float operator* (Vector2& rhs)
	{
		return _x*rhs.X() + _y*rhs.Y();
	}

	Vector2 operator* (double scalar)
	{
		return Vector2(_x * scalar, _y * scalar);
	}

	Vector2 GetPerpendicular()
	{
		return Vector2(-_y, _x);
	}

	std::vector<SDL_Point>* ProjectTriangle(std::vector<SDL_Point>* triangle, Vector2 axis)
	{
	  return &_triangleProjection;
	}

	static Vector2 Zero() {	return Vector2(0.0, 0.0); }
  static Vector2 Up() { return Vector2(0.0, 1.0); }

  // Explanation from previous case:
  //
  // By default rotation is assumed around (0; 0). It is located in the top left corner of the image.
  // Our actual rotation center is image's (w / 2; h / 2). So, first, we move it to (0; 0) (which means, that collider's coordinates are "offseted"),
  // then rotate collider around it, and then move it back ("offset" collider's coordinates back).

  static Vector2 RotateVector(Vector2 around, Vector2 what, double angle, Vector2& result)
  {
    double nx = around.X() + (what.X() - around.X()) * SDL_cos(angle * PIOVER180) - (what.Y() - around.Y()) * SDL_sin(angle * PIOVER180);
    double ny = around.Y() + (what.X() - around.X()) * SDL_sin(angle * PIOVER180) + (what.Y() - around.Y()) * SDL_cos(angle * PIOVER180);

    result.Set(nx, ny);
  }
private:
	double _x;
	double _y;

	std::vector<SDL_Point> _triangleProjection;
};

#endif // VECTOR2_H
