#include "../Include/Point.h"

Point::~Point()
{

}

Point::Point()
{
	this->_x = 0.00f;
	this->_y = 0.00f;
	this->_z = 0.00f;
}

Point::Point(float x, float y, float z)
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
}

Point::Point(const Point & point)
{
	this->_x = point._x;
	this->_y = point._y;
	this->_z = point._z;
}

void Point::add(const Point & point)
{
	this->_x += point._x;
	this->_y += point._y;
	this->_z += point._z;
}

void Point::scale(float scale)
{
	this->_x *= scale;
	this->_y *= scale;
	this->_z *= scale;
}

void Point::setX(float x)
{
	this->_x = x;
}

void Point::setY(float y)
{
	this->_y = y;
}

void Point::setZ(float z)
{
	this->_z = z;
}

float Point::getX() const
{
	return this->_x;
}

float Point::getY() const
{
	return this->_y;
}

float Point::getZ() const
{
	return this->_z;
}

float Point::distance(const Point & point) const
{
	return sqrt(((this->_x - point._x)*(this->_x - point._x)) + ((this->_y - point._y)*(this->_y - point._y)) + ((this->_z - point._z)*(this->_z - point._z)));
}

Point & Point::operator+=(const Point & point)
{
	this->add(point);
	return *this;
}

Point Point::operator+(const Point & point) const
{
	Point p(*this);
	p += point;
	return p;
}

Point Point::operator-() const
{
	return Point(-this->_x, -this->_y, this->_z);
}

Point & Point::operator-=(const Point & point)
{
	return this->operator+=(-point);
}

Point Point::operator-(const Point & point) const
{
	return this->operator+(-point);
}

Point & Point::operator*=(float value)
{
	this->scale(value);
	return (*this);
}

Point Point::operator*(float value) const
{
	Point p(*this);
	p *= value;
	return p;
}

Point & Point::operator/=(float value)
{
	return ((*this) *= (1.00f / value));
}

Point Point::operator/(float value) const
{
	return ((*this)*(1.00f / value));
}

bool Point::operator==(const Point & point)
{
	if (this->_x != point._x)
		return false;
	if (this->_y != point._y)
		return false;
	if (this->_z != point._z)
		return false;
	return true;
}

bool Point::operator!=(const Point & point)
{
	return (!(*this == point));
}

std::ostream & operator << (std::ostream & out, const Point & point)
{
	return out << "Point data --> position(" << point.getX() << " ; " << point.getY() << " ; " << point.getZ() << ")";
}
