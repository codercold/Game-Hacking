#include "stdafx.h"
// vector.cpp 


Vector::Vector()
{ 
	x = y = z = 0.0f; 
}

Vector::Vector(float X, float Y, float Z) 
{ 
	x = X; 
	y = Y; 
	z = Z; 
}

Vector::Vector(const Vector &v) 
{ 
	x = v.x; 
	y = v.y; 
	z = v.z; 
}

//Vector::Vector(float xx = 0.0, float yy = 0.0, float zz = 0.0) : x(xx), y(yy), z(zz) { }

float Vector::length() 
{ 
	return sqrt( x * x + y * y + z * z ); 
}

Vector* Vector::subtract( Vector* pOut, Vector pV1, Vector pV2 ) 
{ 
    pOut->x = pV1.x - pV2.x;
    pOut->y = pV1.y - pV2.y;
    pOut->z = pV1.z - pV2.z;
    return pOut;
}

void Vector::substract(Vector sub) 
{ 
	x -= sub.x; 
	y -= sub.y; 
	z -= sub.z; 
}

float Vector::dotproduct( CONST Vector pV1, CONST Vector pV2 )
{
    return pV1.x * pV2.x + pV1.y * pV2.y + pV1.z * pV2.z;
}

float Vector::dotproduct(Vector dot) 
{ 
	return ( x * dot.x + y * dot.y + z * dot.z); 
}

void Vector::normalize() 
{ 
	float l = 1/length(); 
	x *= l; 
	y *= l; 
	z *= l; 
}

float Vector::vectodegree(Vector to) 
{ 
	return ( ( 180.0f / 3.1415f ) * ( asinf( dotproduct( to )))); 
}

Vector Vector::RotateX(Vector in, float angle) 
{ 
	float a,c,s; 
	Vector out; 
	a = (float) ( angle * M_PI/180 ); 
	c = (float) cos( a ); 
	s = (float) sin( a ); 
	out.x = in.x; 
	out.y = c*in.y - s*in.z; 
	out.z = s*in.y + c*in.z; 

	return out; 
}

Vector Vector::RotateY(Vector in, float angle) 
{ 
	float a,c,s; 
	Vector out; 
	a = (float) ( angle * M_PI/180 ); 
	c = (float) cos( a ); 
	s = (float) sin( a ); 
	out.x = c * in.x + s * in.z; 
	out.y = in.y; 
	out.z = -s * in.x + c * in.z; 

	return out; 
}

Vector Vector::RotateZ(Vector in, float angle) 
{ 
	float a,c,s; 
	Vector out; 
	a = (float) ( angle * M_PI/180 ); 
	c = (float) cos( a ); 
	s = (float) sin( a ); 
	out.x = c * in.x - s * in.y; 
	out.y = s * in.x + c * in.y; 
	out.z = in.z;

	return out; 
}

void Vector::nullvec () 
{ 
	x = y = z = 0; 
}

void Vector::operator = (const float *farray) 
{ 
	x = farray[0]; 
	y = farray[1]; 
	z = farray[2]; 
}
	
void Vector::operator = (const float &val) 
{ 
	x = y = z = val; 
}

float Vector::operator [] (unsigned int element) 
{ 
	switch(element) 
	{ 
		case 1: 
			return y; 
		case 2: 
			return z; 
		default: 
			return x; 
	} 
}

Vector Vector::operator  +  (const Vector& add) const 
{ 
	return Vector( x + add.x, y + add.y, z + add.z); 
}
	
void Vector::operator  += (const Vector& add) 
{ 
	x += add.x; 
	y += add.y; 
	z += add.z; 
}
	
Vector Vector::operator  -  (const Vector& sub) const 
{ 
	return Vector( x - sub.x, y - sub.y, z - sub.z); 
}
	
void   Vector::operator  -= (const Vector& sub) 
{ 
	x -= sub.x; 
	y -= sub.y; 
	z -= sub.z; 
}
	
Vector Vector::operator  *  (const float mul)   const 
{ 
	return Vector( x * mul, y * mul, z * mul); 
}
	
void   Vector::operator  *= (const float mul)   
{ 
	x *= mul; 
	y *= mul; 
	z *= mul; 
}

Vector Vector::operator / (const float mul) const 
{ 
	return Vector( x / mul, y / mul, z / mul); 
}

void Vector::operator /= (const float mul) 
{ 
	x /= mul; 
	y /= mul; 
	z /= mul; 
}

bool Vector::operator == (const Vector& eqal) 
{ 
	return ( ( x == eqal.x ) && ( y == eqal.y ) && ( z == eqal.z )); 
}

bool Vector::operator != (const Vector& eqal) 
{ 
	return ( ( x != eqal.x ) && ( y != eqal.y ) && ( z != eqal.z )); 
}

bool Vector::operator > (const Vector& eqal) 
{ 
	return ( ( x > eqal.x ) && ( y > eqal.y ) && ( z > eqal.z )); 
}

bool Vector::operator < (const Vector& eqal) 
{ 
	return ( ( x < eqal.x ) && ( y < eqal.y ) && ( z < eqal.z )); 
}

bool Vector::operator >= (const Vector& eqal) 
{ 
	return ( ( x >= eqal.x ) && ( y >= eqal.y ) && ( z >= eqal.z )); 
}

bool Vector::operator <= (const Vector& eqal) 
{ 
	return ( ( x <= eqal.x ) && ( y <= eqal.y ) && ( z <= eqal.z )); 
}