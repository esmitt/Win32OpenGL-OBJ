#include "Resource.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "OGLWindow.h"
#include "Matrix4x4.h"
#include "Vector4D.h"

OGLWindow::OGLWindow()
{
	frotationX = frotationY = frotationZ = 0.f;
}

OGLWindow::~OGLWindow()
{
	//Clean up the renderable
	delete m_triangleMesh;
	delete m_cube;
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height);
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyOGLWindow()
{
	DestroyOGLContext();

	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{
	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	RECT clientrect;

	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"OGLWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	m_hdc = GetDC( m_hwnd );

	GetClientRect(m_hwnd, &clientrect);

	SetWindowPos(m_hwnd, NULL, 10, 10, (width << 1) - clientrect.right, (height << 1) - clientrect.bottom, SWP_HIDEWINDOW | SWP_NOREDRAW);
		
	m_width = width;
	m_height = height;

	if (!(m_hglrc = CreateOGLContext(m_hdc)))
		return FALSE;

	//Instantiate a Renderable as OGLCube
	m_cube = new OGLCube();
	m_triangleMesh = new TriangleMesh();
	if (!m_triangleMesh->LoadMeshFromOBJFile("models/teddy.obj"))
	{
		MessageBox(m_hwnd, L"Couldn't load the OBJ file", L"Error", MB_OK);
	}
	InitOGLState();
	return TRUE;
}

void OGLWindow::SetVisible ( BOOL visible )
{
	ShowWindow ( m_hwnd, visible? SW_SHOW : SW_HIDE );
}

void OGLWindow::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	
	Matrix4x4 transformationOBJ, transformationCube;
	Matrix4x4 temp, mRotations;

	Matrix4x4 transformationX, transformationY, transformationZ;
	Matrix4x4 mTranslate, mScale;
	
	//1. Set the matrix 'transformation' as a rotation about the Z axis
	temp.SetRotationZAxis(frotationZ);
	mRotations = temp;

	//
	//2. Set the matrix 'transformation' as a more complex transformation by concatenating, e.g. Rz Rx Ry
	temp.SetRotationYAxis(frotationY);
	mRotations = temp * mRotations;
	temp.SetRotationXAxis(frotationX);
	mRotations = temp * mRotations;

	mTranslate.SetTranslate(Vector4D(0.6f, 0.f, -2.f));
	mScale.SetScale(0.02f, 0.02f, 0.02f);
	transformationOBJ = mTranslate * mRotations * mScale;
	
	mTranslate.SetTranslate(Vector4D(-0.6f, 0.f, -2.f));
	mScale.SetScale(0.8f, 0.8f, 0.8f);
	transformationCube = mTranslate * mRotations * mScale;

	m_cube->Render(&transformationCube);
	m_triangleMesh->Render(&transformationOBJ);

	frotationX += 0.1f;
	if (frotationX > 360.f)
		frotationX -= 360.f;

	frotationZ += 0.05f;
	if (frotationZ > 360.f)
		frotationZ -= 360.f;
	
	frotationY += 0.12f;
	if (frotationY > 360.f)
		frotationY -= 360.f;

	GLfloat lightpos[4] = { 0, 0, 2, 1 };
	Vector4D t(lightpos[0], lightpos[1], lightpos[2]);
	Vector4D r = transformationCube * t;
	float* vectorf = &r[0];
	glLightfv(GL_LIGHT0, GL_POSITION, &r[0]);

	glFlush();

	SwapBuffers(m_hdc);
	return;
}

void OGLWindow::Resize( int width, int height )
{
	RECT clientrect;

	GetClientRect(m_hwnd, &clientrect);

	int left = width - clientrect.right;
	int top = height - clientrect.bottom;

	m_width = width - left;
	m_height = height - top;

	double aspectratio = (double)m_width / (double)m_height;

	glViewport(left >> 1, top >>1 , m_width, m_height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//glOrtho(aspectratio*-2.0, aspectratio*2.0, -2.0, 2.0, -50.0, 50.0);
	
	gluPerspective(60.f, aspectratio, 0.05f, 50.f);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
}

BOOL OGLWindow::MouseLBDown ( int x, int y )
{
	
	return TRUE;
}

BOOL OGLWindow::MouseLBUp ( int x, int y )
{
	return TRUE;
}

BOOL OGLWindow::MouseMove ( int x, int y )
{
	return TRUE;
}