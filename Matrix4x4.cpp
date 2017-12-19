#include <memory.h>
#include <math.h>
#include "Vector4D.h"
#include "Matrix4x4.h"

#define GET(x, y)(x*4 + y)

Matrix4x4::Matrix4x4()
{
	SetIdentity();
}

Matrix4x4::Matrix4x4(const Matrix4x4& rhs)
{
	size_t matrix_size = sizeof(m_element);

	memcpy_s((&m_element[0][0]), matrix_size, rhs.ToPtr(), matrix_size);
}

Matrix4x4::~Matrix4x4()
{

}

const float* Matrix4x4::ToPtr() const
{
	return &(m_element[0][0]);
}

void Matrix4x4::SetZero()
{
	memset(&m_element, 0, sizeof(m_element));
}

void Matrix4x4::SetIdentity()
{
	SetZero();
	m_element[0][0] = m_element[1][1] = m_element[2][2] = m_element[3][3] = 1.0f;
}

Matrix4x4 Matrix4x4::operator* (const Matrix4x4& rhs) const
{
	Matrix4x4 result;
	const float* other = rhs.ToPtr();
	result.Set(0, 0, m_element[0][0] * other[GET(0, 0)] + m_element[0][1] * other[GET(1, 0)] + m_element[0][2] * other[GET(2, 0)] + m_element[0][3] * other[GET(3, 0)]);
	result.Set(0, 1, m_element[0][0] * other[GET(0, 1)] + m_element[0][1] * other[GET(1, 1)] + m_element[0][2] * other[GET(2, 1)] + m_element[0][3] * other[GET(3, 1)]);
	result.Set(0, 2, m_element[0][0] * other[GET(0, 2)] + m_element[0][1] * other[GET(1, 2)] + m_element[0][2] * other[GET(2, 2)] + m_element[0][3] * other[GET(3, 2)]);
	result.Set(0, 3, m_element[0][0] * other[GET(0, 3)] + m_element[0][1] * other[GET(1, 3)] + m_element[0][2] * other[GET(2, 3)] + m_element[0][3] * other[GET(3, 3)]);

	result.Set(1, 0, m_element[1][0] * other[GET(0, 0)] + m_element[1][1] * other[GET(1, 0)] + m_element[1][2] * other[GET(2, 0)] + m_element[1][3] * other[GET(3, 0)]);
	result.Set(1, 1, m_element[1][0] * other[GET(0, 1)] + m_element[1][1] * other[GET(1, 1)] + m_element[1][2] * other[GET(2, 1)] + m_element[1][3] * other[GET(3, 1)]);
	result.Set(1, 2, m_element[1][0] * other[GET(0, 2)] + m_element[1][1] * other[GET(1, 2)] + m_element[1][2] * other[GET(2, 2)] + m_element[1][3] * other[GET(3, 2)]);
	result.Set(1, 3, m_element[1][0] * other[GET(0, 3)] + m_element[1][1] * other[GET(1, 3)] + m_element[1][2] * other[GET(2, 3)] + m_element[1][3] * other[GET(3, 3)]);

	result.Set(2, 0, m_element[2][0] * other[GET(0, 0)] + m_element[2][1] * other[GET(1, 0)] + m_element[2][2] * other[GET(2, 0)] + m_element[2][3] * other[GET(3, 0)]);
	result.Set(2, 1, m_element[2][0] * other[GET(0, 1)] + m_element[2][1] * other[GET(1, 1)] + m_element[2][2] * other[GET(2, 1)] + m_element[2][3] * other[GET(3, 1)]);
	result.Set(2, 2, m_element[2][0] * other[GET(0, 2)] + m_element[2][1] * other[GET(1, 2)] + m_element[2][2] * other[GET(2, 2)] + m_element[2][3] * other[GET(3, 2)]);
	result.Set(2, 3, m_element[2][0] * other[GET(0, 3)] + m_element[2][1] * other[GET(1, 3)] + m_element[2][2] * other[GET(2, 3)] + m_element[2][3] * other[GET(3, 3)]);

	result.Set(3, 0, m_element[3][0] * other[GET(0, 0)] + m_element[3][1] * other[GET(1, 0)] + m_element[3][2] * other[GET(2, 0)] + m_element[3][3] * other[GET(3, 0)]);
	result.Set(3, 1, m_element[3][0] * other[GET(0, 1)] + m_element[3][1] * other[GET(1, 1)] + m_element[3][2] * other[GET(2, 1)] + m_element[3][3] * other[GET(3, 1)]);
	result.Set(3, 2, m_element[3][0] * other[GET(0, 2)] + m_element[3][1] * other[GET(1, 2)] + m_element[3][2] * other[GET(2, 2)] + m_element[3][3] * other[GET(3, 2)]);
	result.Set(3, 3, m_element[3][0] * other[GET(0, 3)] + m_element[3][1] * other[GET(1, 3)] + m_element[3][2] * other[GET(2, 3)] + m_element[3][3] * other[GET(3, 3)]);

	return result;
}

Vector4D Matrix4x4::operator* (const Vector4D& rhs) const
{
	Vector4D result;

	result = rhs;
	
	result[0] = m_element[0][0]*rhs[0] + m_element[0][1]*rhs[1] + m_element[0][2]*rhs[2] + m_element[0][3]*rhs[3];
	result[1] = m_element[1][0]*rhs[0] + m_element[1][1]*rhs[1] + m_element[1][2]*rhs[2] + m_element[1][3]*rhs[3];
	result[2] = m_element[2][0]*rhs[0] + m_element[2][1]*rhs[1] + m_element[2][2]*rhs[2] + m_element[2][3]*rhs[3];
	result[3] = m_element[3][0]*rhs[0] + m_element[3][1]*rhs[1] + m_element[3][2]*rhs[2] + m_element[3][3]*rhs[3];
	return result;
}

void Matrix4x4::SetRotationXAxis(float degrees)
{
	double angleRadians = degrees * M_PI / 180.f;
	SetIdentity();
	m_element[1][1] = cos(angleRadians);
	m_element[1][2] = -sin(angleRadians);
	m_element[2][1] = sin(angleRadians);
	m_element[2][2] = cos(angleRadians);
}

void Matrix4x4::SetRotationYAxis(float degrees)
{
	double angleRadians = degrees * M_PI / 180.f;
	SetIdentity();
	m_element[0][0] = cos(angleRadians);
	m_element[0][2] = sin(angleRadians);
	m_element[2][0] = -sin(angleRadians);
	m_element[2][2] = cos(angleRadians);
}

void Matrix4x4::SetRotationZAxis(float degrees)
{
	double angleRadians = degrees * M_PI / 180.f;
	SetIdentity();
	m_element[0][0] = cos(angleRadians);
	m_element[0][1] = -sin(angleRadians);
	m_element[1][0] = sin(angleRadians);
	m_element[1][1] = cos(angleRadians);
}

void Matrix4x4::SetTranslate(const Vector4D& tvec)
{
	SetIdentity();
	m_element[0][3] = tvec[0];
	m_element[1][3] = tvec[1];
	m_element[2][3] = tvec[2];
}

void Matrix4x4::SetScale(float sx, float sy, float sz)
{
	SetIdentity();
	m_element[0][0] = sx;
	m_element[1][1] = sy;
	m_element[2][2] = sz;
}

void Matrix4x4::SetViewMatrix(const Vector4D& camera_position, const Vector4D& view_vector, const Vector4D& up_vector)
{
	//Matrix as a view matrix based on the given camera_position, view_vector and up_vector
	//extracted from here http://www.cs.virginia.edu/~gfx/Courses/1999/intro.fall99.html/lookat.html
	Vector4D f(view_vector - camera_position);
	f.Normalise();
	Vector4D u_ = up_vector;
	u_.Normalise();
	Vector4D s, u;
	s = f.CrossProduct(u_);
	u = s.CrossProduct(f);

	Matrix4x4 M;
	M.SetIdentity();

	M.Set(0, 0, s[0]);
	M.Set(0, 1, s[1]);
	M.Set(0, 2, s[2]);

	M.Set(1, 0, u[0]);
	M.Set(1, 1, u[1]);
	M.Set(1, 2, u[2]);

	M.Set(2, 0, -f[0]);
	M.Set(2, 1, -f[1]);
	M.Set(2, 2, -f[2]);

	Matrix4x4 T;
	T.SetTranslate(-camera_position);

	(*this) = M * T;
}
