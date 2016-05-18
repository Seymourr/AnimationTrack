#include "vector.h"

using namespace MyOGLProg;

//Default constructor
Vector::Vector(void)
{
	this->x = this->y = this->z = 0.0;
}

//Constructor given coordinates (XYZ)
Vector::Vector(float x, float y, float z)
{
	this->x = x; this->y = y; this->z = z;
}

//Constructor given two positions
Vector::Vector(Position& start, Position& end)
{
	this->x = end.x - start.x;
	this->y = end.y - start.y;
	this->z = end.z - start.z;
}

//Add another vector to this vector and return the resulting vector
Vector Vector::addTo(const Vector &other) const
{
	Vector result;
	result.x = other.x + this->x;
	result.y = other.y + this->y;
	result.z = other.z + this->z;

	return result;
}

//Subtract another vector to this vector and return the resulting vector
Vector Vector::subtractFrom(const Vector &other) const
{
	Vector result;
	result.x = other.x - this->x;
	result.y = other.y - this->y;
	result.z = other.z - this->z;

	return result;
}

//Return the magnitude (length) of the vector
float Vector::getMagnitude(void) const
{
	
	float result;
	float xPow = pow(this->x, 2.0);
	float yPow = pow(this->y, 2.0);
	float zPow = pow(this->z, 2.0);
	result = sqrt(xPow + yPow + zPow);

	return result;
}

//Set the magnitude (length) of the vector
void Vector::setMagnitude(const float m)
{
	this->normalise();
	this->x = this->x * m;
	this->y = this->y * m;
	this->z = this->z * m;
}
	
//Return the dot product between this vector and the other given one
float Vector::getDotProduct(const Vector &other) const
{
	
	float result;
	float xx = other.x * this->x;
	float yy = other.y * this->y;
	float zz = other.z * this->z;
	result = xx + yy + zz;
	return result;
}

//Return the cross product between this vector and the other
Vector Vector::getCrossProduct(const Vector &other) const
{
	//RHS ? LHS ? Order matter! (Call order)
	Vector result;
	result.x = other.y * this->z - other.z * this->y;
	result.y = other.z * this->x - other.x * this->z;
	result.z = other.x * this->y - other.y * this->x;
	return result;
}

//Normalise this vector (set its length to 1)
void Vector::normalise(void)
{
	float length = this->getMagnitude();
	this->x = this->x / length;
	this->y = this->y / length;
	this->z = this->z / length;
}