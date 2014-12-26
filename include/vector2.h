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

	float X() { return _x; }
	float Y() { return _y; }

	Vector2 Get() { return Vector2(_x, _y); }
	void Set(double ax, double ay)
	{
		_x = ax;
		_y = ay;
	}
	void Set(Vector2 vector)
	{
		_x = vector._x;
		_y = vector._y;
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

private:
	double _x;
	double _y;

	std::vector<SDL_Point> _triangleProjection;
};

#endif // VECTOR2_H
