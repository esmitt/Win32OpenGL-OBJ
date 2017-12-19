
#include "TriangleMesh.h"
#include "Matrix4x4.h"
#include "OBJFileReader.h"
#include <windows.h>
#include <gl/GL.h>

TriangleMesh::TriangleMesh()
{
	m_vertices = NULL;
	m_triangles = NULL;
	m_numTriangles = 0;
	m_numVertices = 0;
}


TriangleMesh::~TriangleMesh()
{
	CleanUp();
}

void TriangleMesh::Render(const Matrix4x4* trans)
{
	//1. Rendered the TriangleMesh using the vertices and triangle data stored in m_vertices and m_triangle
	//2. Transformed the triangle mesh using the given transformation trans
	const int * indexes;
	Vector4D v1, v2, v3, n1, n2, n3, normal;
	for (int k = 0; k < m_numTriangles; k++)
	{
		indexes = m_triangles[k].GetVertexIndices();
		v1 = (*trans) * m_vertices[indexes[0]].m_position;
		v2 = (*trans) * m_vertices[indexes[1]].m_position;
		v3 = (*trans) * m_vertices[indexes[2]].m_position;
		n1 = m_vertices[indexes[0]].m_normal;
		n2 = m_vertices[indexes[1]].m_normal;
		n3 = m_vertices[indexes[2]].m_normal;
		Vector4D color;
		glBegin(GL_TRIANGLES);
			normal = m_triangles[k].GetNormal();
			//glNormal3f(normal[0], normal[1], normal[2]);	//per face

			glColor3f(n1[0], n1[1], n1[2]);
			glNormal3f(n1[0], n1[1], n1[2]);	//per vertex
			glVertex3f(v1[0], v1[1], v1[2]);

			glColor3f(n2[0], n2[1], n2[2]);
			glNormal3f(n2[0], n2[1], n2[2]);	//per vertex
			glVertex3f(v2[0], v2[1], v2[2]);

			glColor3f(n3[0], n3[1], n3[2]);
			glNormal3f(n3[0], n3[1], n3[2]);	//per vertex
			glVertex3f(v3[0], v3[1], v3[2]);
		glEnd();
	}
}

void TriangleMesh::ComputeTriangleNormals()
{
	//Compute the normal for each triangle stored in m_triangles
	Vector4D v1, v2, v3;
	Vector4D U, V, N;
	const int*  indexes;
	for (int k = 0; k < m_numTriangles; k++)
	{
		indexes = m_triangles[k].GetVertexIndices();
		v1 = m_vertices[indexes[0]].m_position;
		v2 = m_vertices[indexes[1]].m_position;
		v3 = m_vertices[indexes[2]].m_position;
		U = v2 - v1;
		V = v3 - v1;
		N[0] = U[1] * V[2] - U[2] * V[1];
		N[1] = U[2] * V[0] - U[0] * V[2];
		N[2] = U[0] * V[1] - U[1] * V[0];
		N.Normalise();
		m_triangles[k].SetNormal(N);
	}
}

void TriangleMesh::ComputeVertexNormals()
{
	//Compute the normal for each vertex stored in m_vertices
	Vector4D v1, v2, v3;
	Vector4D U, V, N;
	const int*  indexes;
	for (int k = 0; k < m_numTriangles; k++)
	{
		indexes = m_triangles[k].GetVertexIndices();
		m_vertices[indexes[0]].m_normal = m_vertices[indexes[0]].m_normal + m_triangles[k].GetNormal();
		m_vertices[indexes[1]].m_normal = m_vertices[indexes[1]].m_normal + m_triangles[k].GetNormal();
		m_vertices[indexes[2]].m_normal = m_vertices[indexes[2]].m_normal + m_triangles[k].GetNormal();
	}
	
	for (int k = 0; k < m_numVertices; k++)
	{
		m_vertices[k].m_normal.Normalise();
	}
}

bool TriangleMesh::LoadMeshFromOBJFile(const char* filename)
{
	//First pass: reading the OBJ to gether info related to the mesh
	int numVertices = 0;
	int numNormals = 0;
	int numTexCoords = 0;
	int numTriangles = 0;

	if (OBJFileReader::firstPassOBJRead(filename, &numVertices, &numNormals, &numTexCoords, &numTriangles))
	{
		printf("Error parsing file: %s\n", filename);
		return false;
	}

	//allocate memory for m_vertices and m_triangles based on the number of vertices and triangles from the first pass
	if (m_vertices && m_triangles)
	{
		printf("Vertex array and triangle array have already been allocated.\n");
		return false;
	}

	m_vertices = new Vertex[numVertices];
	m_triangles = new Triangle[numTriangles];

	if (OBJFileReader::secondPassOBJRead(filename, m_vertices, m_triangles))
	{
		printf("Error parsing file: %s\n", filename);
		return false;
	}

	m_numTriangles = numTriangles;
	m_numVertices = numVertices;


	ComputeTriangleNormals();
	ComputeVertexNormals();

	return true;
}

void TriangleMesh::CleanUp()
{
	if (m_vertices)
		delete[] m_vertices;
	if (m_triangles)
		delete[] m_triangles;

	m_vertices = NULL;
	m_triangles = NULL;
	m_numTriangles = 0;
	m_numVertices = 0;
}