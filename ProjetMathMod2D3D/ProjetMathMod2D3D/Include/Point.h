#pragma once

#include <iostream>
#include <cmath>

class Point
{
	private:
		float _x;
		float _y;
		float _z;

	public:
		~Point(void);

		Point(void);
		Point(float x, float y, float z);
		Point(const Point & point);

		void add(const Point & point);

		void scale(float scale);

		void setX(float x);
		void setY(float y);
		void setZ(float z);

		float getX() const;
		float getY() const;
		float getZ() const;

		float dot(const Point& point)const;
		float norm()const;
		float distance(const Point & point) const;

		Point & operator+=(const Point & point);
		Point operator+(const Point & point) const;

		Point operator-() const;
		Point & operator-=(const Point & point);
		Point operator-(const Point & point) const;

		Point & operator*=(float value);
		Point operator*(float value) const;

		Point & operator/=(float value);
		Point operator/(float value) const;

		bool operator==(const Point & point);
		bool operator!=(const Point & point);
};

std::ostream & operator << (std::ostream & out, const Point & p_point);

