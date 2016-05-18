#include "myQuat.h"


//Default constructor
MyQuat::MyQuat() {
	this->w = 0.0;
	this->v = MyVector();
}

//Create from axis, angle
MyQuat::MyQuat(float angleDeg, MyVector &axis) {
	axis.normalise();
	float rad = DEG2RAD(angleDeg/2);
	this->w = cos(rad);
	this->v.x = axis.x * sin(rad);
	this->v.y = axis.y * sin(rad);
	this->v.z = axis.z * sin(rad);
}	

//Create from position
MyQuat::MyQuat(MyPosition &p) {
	this->w = 0;
	MyVector v(p.x, p.y, p.z);
	this->v = v;
}

//Add this quat to another quat and return the result
MyQuat MyQuat::addTo(const MyQuat &other) const{
	MyQuat res;
	res.w = this->w + other.w;
	res.v = this->v.addTo(other.v);

	return res;
}

//Multiply this quat by another quat and return the result
MyQuat MyQuat::multiplyBy(const MyQuat &other) const {

	MyQuat res;
	res.w = this->w*other.w - this->v.getDotProduct(other.v);
	
	MyVector a = this->v.getCrossProduct(other.v);
	
	MyVector b = other.v; b.uniformScale(this->w);
	
	MyVector c = this->v; c.uniformScale(other.w);
	res.v = a.addTo(b);
	res.v = res.v.addTo(c);

	return res;
}

//Get the magnitude (length) of this quat
float MyQuat::getMagnitude() const {
	
	float wPow = pow(this->w, 2.0);
	float xPow = pow(this->v.x, 2.0);
	float yPow = pow(this->v.y, 2.0);
	float zPow = pow(this->v.z, 2.0);
	float  result = sqrt(wPow + xPow + yPow + zPow);

	return result;
}

//Normalize this quat (put its length to 1)
void MyQuat::normalise() {
	float length = this->getMagnitude();
	this->w = this->w / length;
	this->v.x = this->v.x / length;
	this->v.y = this->v.y / length;
	this->v.z = this->v.z / length;
}

//Return the conjuage (negatived version) of this quat
MyQuat MyQuat::getConjugate() const {
	MyQuat res;
	res.w = this->w;
	res.v = this->v;
	res.v.x *= -1.0;
	res.v.y *= -1.0;
	res.v.z *= -1.0;
	return res;
}

//Return the inverse of this quat
MyQuat MyQuat::getInverse() const {
	MyQuat res = this->getConjugate();
	float len = pow(this->w, 2.0)
		+ pow(this->v.x, 2.0)
		+ pow(this->v.y, 2.0)
		+ pow(this->v.z, 2.0);
	res.w /= len;
	res.v.x /= len;
	res.v.y /= len;
	res.v.z /= len;
	res.normalise();
	return res;
}

//Convert this quat to a rotation matrix specified by complex numbers and real value w
MyMatrix MyQuat::convertToRotationMatrix() const{
	static GLfloat res[16] =
	{
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,0.0,1.0
	};
	res[0] = 1 - 2 * pow(this->v.y, 2.0) - 2 * pow(this->v.z, 2.0);
	res[1] = 2 * this->v.x*this->v.y - 2 * this->w*this->v.z;
	res[2] = 2 * this->v.x*this->v.z + 2 * this->w*this->v.y;

	res[4] = 2 * this->v.x*this->v.y + 2 * this->w*this->v.z;
	res[5] = 1 - 2 * pow(this->v.x, 2.0) - 2 * pow(this->v.z, 2.0);
	res[6] = 2 * this->v.y*this->v.z + 2 * this->w*this->v.x;

	res[8] = 2 * this->v.x*this->v.z - 2 * this->w*this->v.y;
	res[9] = 2 * this->v.y*this->v.z + 2 * this->w*this->v.x;
	res[10] = 1 - 2 * pow(this->v.x, 2.0) - 2 * pow(this->v.y, 2.0);
	MyMatrix tmp;
	tmp.getGLModelviewMatrix();
	MyMatrix resM;
	glLoadMatrixf(res);
	resM.getGLModelviewMatrix();
	tmp.setGLMatrix();
	return resM;
}
