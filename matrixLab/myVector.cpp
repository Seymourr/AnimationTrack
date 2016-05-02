#include "MyVector.h"

using namespace MyMathLab;

MyVector::MyVector(void)
{
	this->x = this->y = this->z = 0.0;
}

MyVector::MyVector(float x, float y, float z)
{
	this->x = x; this->y = y; this->z = z;
}

MyVector::MyVector(MyPosition& start, MyPosition& end)
{
	this->x = end.x - start.x;
	this->y = end.y - start.y;
	this->z = end.z - start.z;
}

MyVector MyVector::addTo(const MyVector &other) const
{
	MyVector result;
	result.x = other.x + this->x;
	result.y = other.y + this->y;
	result.z = other.y + this->z;

	return result;
}

MyVector MyVector::subtractFrom(const MyVector &other) const
{
	MyVector result;
	result.x = other.x - this->x;
	result.y = other.y - this->y;
	result.z = other.y - this->z;

	return result;
}
		
float MyVector::getMagnitude(void) const
{
	float result;
	float xPow = pow(this->x, 2.0);
	float yPow = pow(this->y, 2.0);
	float zPow = pow(this->z, 2.0);
	result = sqrt(xPow + yPow + zPow);

	return result;
}

void MyVector::uniformScale(const float m)
{
	this->x = this->x * m;
	this->y = this->y * m;
	this->z = this->z * m;
}

void MyVector::normalise(void)
{
	float length = this->getMagnitude();
	this->x = this->x / length;
	this->y = this->y / length;
	this->z = this->z / length;
}

void MyVector::setMagnitude(const float m)
{
	if (this->getMagnitude() > 0.0)
	{
		this->normalise();
		this->uniformScale(m);
	}
}
	
float MyVector::getDotProduct(const MyVector &other) const
{
	float result = 0.0;
	float xx = other.x * this->x;
	float yy = other.y * this->y;
	float zz = other.z * this->z;
	result = xx + yy + zz;
	return result;
}

MyVector MyVector::getCrossProduct(const MyVector &other) const
{
	//A = this, B = other ????
	MyVector result;
	result.x = this->y * other.z - this->z * other.y;
	result.y = this->z * other.x - this->x * other.z;
	result.z = this->x * other.y - this->y * other.x;
	return result;
}
