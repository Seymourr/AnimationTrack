#include "vector.h"

using namespace MyOGLProg;

Vector::Vector(void)
{
	this->x = this->y = this->z = 0.0;
}

Vector::Vector(float x, float y, float z)
{
	this->x = x; this->y = y; this->z = z;
}

Vector::Vector(Position& start, Position& end)
{
	//initialise this vector from two positions
	this->x = end.x - start.x;
	this->y = end.y - start.y;
	this->z = end.z - start.z;
}

Vector Vector::addTo(const Vector &other) const
{
	//add this vector to other and return the result
	Vector result;
	result.x = other.x + this->x;
	result.y = other.y + this->y;
	result.z = other.z + this->z;

	return result;
}

Vector Vector::subtractFrom(const Vector &other) const
{
	//subtract this vector from other and return the result
	Vector result;
	result.x = other.x - this->x;
	result.y = other.y - this->y;
	result.z = other.z - this->z;

	return result;
}
		
float Vector::getMagnitude(void) const
{
	//get the length of the vector
	float result;
	float xPow = pow(this->x, 2.0);
	float yPow = pow(this->y, 2.0);
	float zPow = pow(this->z, 2.0);
	result = sqrt(xPow + yPow + zPow);

	return result;
}

void Vector::setMagnitude(const float m)
{
	//set the length of the vector
	this->normalise();
	this->x = this->x * m;
	this->y = this->y * m;
	this->z = this->z * m;
}
	
float Vector::getDotProduct(const Vector &other) const
{
	//return the dot product between this vector and other
	float result;
	float xx = other.x * this->x;
	float yy = other.y * this->y;
	float zz = other.z * this->z;
	result = xx + yy + zz;
	return result;
}

Vector Vector::getCrossProduct(const Vector &other) const
{
	//return the cross product between this vector and other
	//A = this, B = other ????
	Vector result;
	result.x = other.y * this->z - other.z * this->y;
	result.y = other.z * this->x - other.x * this->z;
	result.z = other.x * this->y - other.y * this->x;
	return result;
}

void Vector::normalise(void)
{
	//normalise this vector (set its length to 1)
	float length = this->getMagnitude();
	this->x = this->x / length;
	this->y = this->y / length;
	this->z = this->z / length;
}

