//Introductory quaternion class

#include "myFormulas.h"
#include "myVector.h"
#include "myMatrix.h"
#include <math.h>
#include <iostream>
namespace MyMathLab
{

class MyQuat
{
	public:
		MyQuat(void);
		MyQuat(float angleDeg, MyVector &axis);	
		MyQuat(MyPosition &p);	
		
		~MyQuat(void){;}

		MyQuat addTo(const MyQuat &other) const;
		MyQuat multiplyBy(const MyQuat &other) const;

		float getMagnitude(void) const;	
		void normalise(void);
		MyQuat getConjugate(void) const;	
		MyQuat getInverse(void) const;

		MyMatrix convertToRotationMatrix(void) const; 

		float w;
		MyVector v;
};


}