// vector.h

#ifndef VECTOR_H
#define VECTOR_H

#define M_PI  3.14159265358979323846

class Vector
{

public:

	Vector();
	Vector(float X, float Y, float Z);
	Vector(const Vector &v);	
	//Vector(float xx = 0.0, float yy = 0.0, float zz = 0.0); 

	Vector RotateX (Vector in, float angle);
	Vector RotateY (Vector in, float angle);
	Vector RotateZ (Vector in, float angle);
	Vector operator / (const float mul) const;
	Vector operator * (const float mul) const; 
	Vector operator - (const Vector& sub) const;
	Vector operator + (const Vector& add) const;

	void nullvec (); 
	void normalize ();
	void substract (Vector sub);
	static Vector* subtract( Vector* pOut, Vector pV1, Vector pV2 );
	void operator = (const float *farray);	
	void operator = (const float &val);		
	void operator += (const Vector& add);		
	void operator -= (const Vector& sub); 		
	void operator *= (const float mul);  	 
	void operator /= (const float mul);

	bool operator == (const Vector& eqal); 	
	bool operator != (const Vector& eqal); 
	bool operator > (const Vector& eqal); 
	bool operator < (const Vector& eqal); 
	bool operator >= (const Vector& eqal); 	
	bool operator <= (const Vector& eqal); 
	
	float length ();	
	float dotproduct (Vector dot);
	static float dotproduct( CONST Vector pV1, CONST Vector pV2 );
	float vectodegree (Vector to);
	float operator [] (unsigned int element); 

	float x;	
	float y;
	float z;
};

#endif