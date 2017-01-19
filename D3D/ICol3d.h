//////////////////////////////////////////////////
// ICol3D.h  - Interface to 2D collision
//
//
#ifndef __ICOL3D_H__
#define __ICOL3D_H__

// ICol2D is an interface class for collision detection
class	ICol3D
{
public:
	ICol3D();
	ICol3D(Vector3 &start, Vector3 &end);
	void        Set(Vector3 &start, Vector3 &end);
	virtual bool        CheckCollision() = 0;
	bool        CheckCollision(Vector3 &start, Vector3 &end);
	Vector3     GetNormal() { return m_normal; }
	Vector3     GetPoint() { return m_point; }
	Vector3     GetStart() { return m_start; }
	Vector3     GetEnd() { return m_end; }
protected:
	// Input data
	Vector3     m_start, m_end;
	// Output/Calculated Data
	Vector3     m_point, m_normal;
};

inline ICol3D::ICol3D()
{
	m_start.x = 0.0f;
	m_start.y = 0.0f;
	m_start.z = 0.0f;
	m_end.x = 1.0f;
	m_start.y = 1.0f;
	m_start.z = 1.0f;

	//m_point = m_normal = Vector3(0,0);
}

inline ICol3D::ICol3D(Vector3 &start, Vector3 &end)
{
	m_start = start;
	m_end = end;
}

inline void ICol3D::Set(Vector3 &start, Vector3 &end)
{
	m_start = start;
	m_end = end;
}

inline bool ICol3D::CheckCollision(Vector3 &start, Vector3 &end)
{
	Set(start, end);
	return CheckCollision();
}


#endif
//////////////////////////////////////////////////
// ICol3D.h  - Interface to 2D collision
//
//
#ifndef __ICOL3D_H__
#define __ICOL3D_H__

// ICol2D is an interface class for collision detection
class	ICol3D
{
public:
	ICol3D();
	ICol3D(Vector3 &start, Vector3 &end);
	void        Set(Vector3 &start, Vector3 &end);
	virtual bool        CheckCollision() = 0;
	bool        CheckCollision(Vector3 &start, Vector3 &end);
	Vector3     GetNormal() { return m_normal; }
	Vector3     GetPoint() { return m_point; }
	Vector3     GetStart() { return m_start; }
	Vector3     GetEnd() { return m_end; }
protected:
	// Input data
	Vector3     m_start, m_end;
	// Output/Calculated Data
	Vector3     m_point, m_normal;
};

inline ICol3D::ICol3D()
{
	m_start.x = 0.0f;
	m_start.y = 0.0f;
	m_start.z = 0.0f;
	m_end.x = 1.0f;
	m_start.y = 1.0f;
	m_start.z = 1.0f;

	//m_point = m_normal = Vector3(0,0);
}

inline ICol3D::ICol3D(Vector3 &start, Vector3 &end)
{
	m_start = start;
	m_end = end;
}

inline void ICol3D::Set(Vector3 &start, Vector3 &end)
{
	m_start = start;
	m_end = end;
}

inline bool ICol3D::CheckCollision(Vector3 &start, Vector3 &end)
{
	Set(start, end);
	return CheckCollision();
}


#endif
