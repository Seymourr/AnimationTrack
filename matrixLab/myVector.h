//Introductory vector class

#include <math.h>
#include "myFormulas.h"

namespace MyMathLab
{



struct MyPosition
{
	float x;
	float y;
	float z;
};

class MyVector
{
	public:
		MyVector(void);
		MyVector(float x, float y, float z);
		MyVector(MyPosition& start, MyPosition& end);
		~MyVector(void){;}

		MyVector addTo(const MyVector &other) const;
		MyVector subtractFrom(const MyVector &other) const;
		
		void uniformScale(const float m);

		float getMagnitude(void) const;
		void setMagnitude(const float m);
		
		float getDotProduct(const MyVector &other) const;
		MyVector getCrossProduct(const MyVector &other) const;
		
		void normalise(void);		

		float x;
		float y;
		float z;
};


}