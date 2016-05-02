//Introductory quaternion class

#include "myVector.h"
#include "myMatrix.h"
#include <math.h>
namespace MyMathLab
{
	const double M_PI = 3.14159265358979323846;
	#define DEG2RAD(x) ((x)*M_PI/180.0)
	#define RAD2DEG(x) ((x)*180.0/M_PI)
class MyQuat
{
	public:
		MyQuat(void);
		MyQuat(float angleDeg, MyVector &axis);	//create from axis, angle
		MyQuat(MyPosition &p);	//create from position
		
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