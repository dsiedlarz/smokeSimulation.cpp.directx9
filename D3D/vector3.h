#ifndef	__VECTOR3_H__
#define	__VECTOR3_H__

#include <math.h>		// for sqrtf, etc.

class Vector3
{
public:

	Vector3();
	Vector3(const Vector3& v);
	Vector3(float xx, float yy, float zz);
	Vector3& 		operator=	(const Vector3& v);
	bool			operator==  (const Vector3& v) const;
	bool			operator!=  (const Vector3& v) const;
	Vector3&		operator+=	(const Vector3& v);
	Vector3&		operator-=	(const Vector3& v);
	Vector3&		operator*=	(const float s);
	Vector3&		operator/=	(float s);
	float           Length3() { return x*x + y*y + z*z; }
	float           Length() { return sqrtf(Length3()); }
	Vector3         Normal();

	// The actual vector is stored as two floats.
	// could be private, but it's a lot simpler if it's not  
	float   x, y, z;

};

Vector3		operator+ (const Vector3& v1, const Vector3& v2);
Vector3		operator- (const Vector3& v1, const Vector3& v2);
Vector3		operator* (const Vector3& v1, const Vector3& v2);
Vector3		operator* (const Vector3& v, const float s);
Vector3		operator* (const float s, const Vector3& v);
Vector3		operator/ (const Vector3& v, const float s);
Vector3		operator- (const Vector3& v);

inline float       DotProduct(const Vector3& v1, const Vector3& v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

inline Vector3::Vector3()
{

}

inline Vector3::Vector3(float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}

inline Vector3::Vector3(const Vector3& v)
{
	*this = v;
}

inline Vector3&		Vector3::operator=	(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

inline bool		  	Vector3::operator== (const Vector3& v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z));
}

inline bool		  	Vector3::operator!= (const Vector3& v) const
{
	return !(*this == v);
}

inline Vector3&		Vector3::operator+= (const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Vector3&		Vector3::operator-= (const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Vector3&		Vector3::operator*= (const float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

inline Vector3&		Vector3::operator/= (float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

inline	Vector3	operator+	(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline	Vector3	operator-	(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline	Vector3	operator*	(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}


inline	Vector3	operator*	(const Vector3& v, const float s)
{
	return Vector3(v.x * s, v.y * s, v.z * s);
}

inline	Vector3	operator*	(const float s, const Vector3& v)
{
	return v * s;
}

inline	Vector3	operator/	(const Vector3& v, const float s)
{
	return v * (1.0f / s);
}

inline	Vector3	operator-	(const Vector3& v)
{
	return Vector3(-v.x, -v.y, -v.z);
}

inline Vector3 Vector3::Normal()
{
	Vector3 t;
	float len = Length();
	if (len > 0.0f)
	{
		t.x = x / len;
		t.y = y / len;
		t.z = z / len;
	}
	return t;
}

// End of Vector3
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif