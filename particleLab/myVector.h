//Introductory vector class

#include <math.h>
#include "myFormulas.h"

#ifndef _VECTOR_H
#define _VECTOR_H


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

		// New: Overloaded Operators...
		MyVector operator + (const MyVector &other);
		MyVector operator - (const MyVector &other);
		MyVector operator * (const MyVector &other);
		MyVector operator / (const MyVector &other);

		MyVector operator * (const float scalar);
		friend MyVector operator * (const float scalar, const MyVector &other);
	    
		MyVector& operator = (const MyVector &other);
		MyVector& operator += (const MyVector &other);
		MyVector& operator -= (const MyVector &other);

		MyVector operator + (void) const;
		MyVector operator - (void) const;

		float x;
		float y;
		float z;
};




#endif