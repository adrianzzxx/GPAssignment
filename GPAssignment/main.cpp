
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "Assignment"

bool Ran = false, CV = true;
int Rand = 0;

int Qnum = 1;

float R = 0.0, G = 0, B = 0;

float OLeft = -15.0, ORight = 15.0, ODown = -15.0, OUp = 15.0, ONear = -15.0, OFar = 15.0;
float PNear = 1.0, PFar = 20.0;
float SphereR = 3.0;
float ptX = 0.0, ptY = 0.0, ptSpeed = 0.1;
float prSpeed = 1.0;

float MoveX, MoveY, MoveZ;
float RotateS = 0.0, RotateX = 0.0, RotateY = 0.0, RotateZ = 0.0;
float ScaleX = 1.0, ScaleY = 1.0, ScaleZ = 1.0;


LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == VK_SPACE) {
			MoveX = 0;
			MoveY = 0;
			MoveZ = 0;
			RotateS = 0.0;
			RotateX = 0.0;
			RotateY = 0.0;
			RotateZ = 0.0;
			ScaleX = 1.0;
			ScaleY = 1.0;
			ScaleZ = 1.0;
			prSpeed = 0.0;
			ptX = 0.0;
			ptY = 0.0;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(ptX, ptY, 0.0);
	glRotatef(prSpeed, 0.0, 1.0, 0.0);

	if (CV == true) {
		//ortho view
		glOrtho(OLeft, ORight, ODown, OUp, ONear, OFar);
	}
	else {
		//perspective view
		gluPerspective(20.0, 1.0, -1.0, 1.0);
		glFrustum(-10.0, 10.0, -10.0, 10.0, PNear, PFar);
	}
}

void drawSphere(double r, int slice, int stack) {
	GLUquadricObj* sphere = NULL;   // create a quadric obj pointer
	sphere = gluNewQuadric();       // create a new quadric obj in the memory
	glLineWidth(1.0);
	gluQuadricDrawStyle(sphere, GLU_FILL);//in line style
	gluSphere(sphere, r, slice, stack); // draw sphere
	gluDeleteQuadric(sphere); //delete the quadric ob
}

void drawCylinder(double br, double tr, double h) {
	GLUquadricObj* cylinder = NULL;   // create a quadric obj pointer
	cylinder = gluNewQuadric();       // create a new quadric obj in the memory
	glLineWidth(1.0);
	gluQuadricDrawStyle(cylinder, GLU_FILL);//in line style ,GLU_POINT
	gluCylinder(cylinder, br, tr, h, 30, 30); // draw sphere
	gluDeleteQuadric(cylinder); //delete the quadric ob
}

void drawCone(double tr, double h) {
	GLUquadricObj* cone = NULL;   // create a quadric obj pointer
	cone = gluNewQuadric();       // create a new quadric obj in the memory

	glLineWidth(1.0);
	gluQuadricDrawStyle(cone, GLU_FILL);//in line style
	gluCylinder(cone, 0.0, tr, h, 30, 30); // draw sphere
	gluDeleteQuadric(cone);  //delete the quadric ob
}

void robot() {
	glClearColor(0.75, 0.75, 0.75, 0.0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projection();

	glMatrixMode(GL_MODELVIEW);
	
	//simply write something
	
}

void drawFinger() {
	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	//glRotatef(-90.0, 1.0, 0.0, 0.0);
			//////////////////////////////////////////////////////////////draw finger 1-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 2-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 3-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.0, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 4-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.25, 0.0);
	glRotatef(-45, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////draw finger 1-2
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 2-2
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.0, -0.25, 0.0);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.0, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 3-2
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.0, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 4-2
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();

	plPushMatrix
	//////////////////////////////////////////////////////////////draw finger 1-3
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 2-3
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.0, -0.25, 0.0);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.0, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 3-3
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.0, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 4-3
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void arm() {
	glClearColor(0.75, 0.75, 0.75, 0.0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(90, 0.0, 1.0, 0.0);
	glPushMatrix();//////////////////////////////////////////////////////////////////////move whole arm
	glRotatef(-45, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////upper arm joint to body
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0, 0.5, 0.0);
		drawSphere(0.5, 30, 30);
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw left upper arm
	glPushMatrix();
		/////////////////////////////////////////upper face
		glBegin(GL_QUADS);	
		glColor3f(0.0, 0.0, 0.0);
			glVertex3f(-0.5, 0.0, 0.5);
			glVertex3f(0.5, 0.0, 0.5);
			glVertex3f(0.5, 0.0, -0.5);
			glVertex3f(-0.5, 0.0, -0.5);
		glEnd();
		/////////////////////////////////////////lower face
		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 0.0);
			glVertex3f(-0.5, -4.0, 0.5);
			glVertex3f(0.5, -4.0, 0.5);
			glVertex3f(0.5, -4.0, -0.5);
			glVertex3f(-0.5, -4.0, -0.5);
		glEnd();
		/////////////////////////////////////////front face
		glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
			glVertex3f(-0.5, 0.0, 0.5);
			glVertex3f(0.5, 0.0, 0.5);
			glVertex3f(0.5, -4.0, 0.5);
			glVertex3f(-0.5, -4.0, 0.5);
		glEnd();
		/////////////////////////////////////////back face
		glBegin(GL_QUADS);
		glColor3f(0.0, 1.0, 0.0);
			glVertex3f(-0.5, 0.0, -0.5);
			glVertex3f(0.5, 0.0, -0.5);
			glVertex3f(0.5, -4.0, -0.5);
			glVertex3f(-0.5, -4.0, -0.5);
		glEnd();
		/////////////////////////////////////////left face
		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 1.0);
			glVertex3f(-0.5, 0.0, 0.5);
			glVertex3f(-0.5, 0.0, -0.5);
			glVertex3f(-0.5, -4.0, -0.5);
			glVertex3f(-0.5, -4.0, 0.5);
		glEnd();
		/////////////////////////////////////////right face
		glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 0.0);
			glVertex3f(0.5, 0.0, 0.5);
			glVertex3f(0.5, 0.0, -0.5);
			glVertex3f(0.5, -4.0, -0.5);
			glVertex3f(0.5, -4.0, 0.5);
		glEnd();
	glPopMatrix();
	glPushMatrix();//////////////////////////////////////////////////////////////////////move Lower Arm
	glTranslatef(0.0, -4.5, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////upper arm joint to lower arm
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		drawSphere(0.5, 30, 30);
	glPopMatrix();	
	//////////////////////////////////////////////////////////////draw left lower arm
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0);
		/////////////////////////////////////////upper face
		glBegin(GL_QUADS);
			glColor3f(0.0, 0.0, 0.0);
			glVertex3f(-0.5, -0.0, 0.5);
			glVertex3f(0.5, -0.0, 0.5);
			glVertex3f(0.5, -0.0, -0.5);
			glVertex3f(-0.5, -0.0, -0.5);
		glEnd();
		/////////////////////////////////////////lower face
		glBegin(GL_QUADS);
			glColor3f(0.0, 0.0, 0.0);
			glVertex3f(-0.5, -3.0, 0.5);
			glVertex3f(0.5, -3.0, 0.5);
			glVertex3f(0.5, -3.0, -0.5);
			glVertex3f(-0.5, -3.0, -0.5);
		glEnd();
		/////////////////////////////////////////front face
		glBegin(GL_QUADS);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(-0.5, -0.0, 0.5);
			glVertex3f(0.5, -0.0, 0.5);
			glVertex3f(0.5, -3.0, 0.5);
			glVertex3f(-0.5, -3.0, 0.5);
		glEnd();
		/////////////////////////////////////////back face
		glBegin(GL_QUADS);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(-0.5, -0.0, -0.5);
			glVertex3f(0.5, -0.0, -0.5);
			glVertex3f(0.5, -3.0, -0.5);
			glVertex3f(-0.5, -3.0, -0.5);
		glEnd();
		/////////////////////////////////////////left face
		glBegin(GL_QUADS);
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(-0.5, -0.0, 0.5);
			glVertex3f(-0.5, -0.0, -0.5);
			glVertex3f(-0.5, -3.0, -0.5);
			glVertex3f(-0.5, -3.0, 0.5);
		glEnd();
		/////////////////////////////////////////right face
		glBegin(GL_QUADS);
			glColor3f(1.0, 1.0, 0.0);
			glVertex3f(0.5, -0.0, 0.5);
			glVertex3f(0.5, -0.0, -0.5);
			glVertex3f(0.5, -3.0, -0.5);
			glVertex3f(0.5, -3.0, 0.5);
		glEnd();
	glPopMatrix();
	glPushMatrix();//////////////////////////////////////////////////////////////////////move hand
	glTranslatef(0.0, -3.5, 0.25);
	glRotatef(-90, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////palm
	glPushMatrix();
		/////////////////////////////////////////upper face
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glVertex3f(-0.5, -0.0, 0.25);
			glVertex3f(0.5, -0.0, 0.25);
			glVertex3f(0.5, -0.0, -0.25);
			glVertex3f(-0.5, -0.0, -0.25);
		glEnd();
		/////////////////////////////////////////lower face
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glVertex3f(-0.5, -1.0, 0.25);
			glVertex3f(0.5, -1.0, 0.25);
			glVertex3f(0.5, -1.0, -0.25);
			glVertex3f(-0.5, -1.0, -0.25);
		glEnd();
		/////////////////////////////////////////front face
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glVertex3f(-0.5, -0.0, 0.25);
			glVertex3f(0.5, -0.0, 0.25);
			glVertex3f(0.5, -1.0, 0.25);
			glVertex3f(-0.5, -1.0, 0.25);
		glEnd();
		/////////////////////////////////////////back face
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glVertex3f(-0.5, -0.0, -0.25);
			glVertex3f(0.5, -0.0, -0.25);
			glVertex3f(0.5, -1.0, -0.25);
			glVertex3f(-0.5, -1.0, -0.25);
		glEnd();
		/////////////////////////////////////////left face
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glVertex3f(-0.5, -0.0, 0.25);
			glVertex3f(-0.5, -0.0, -0.25);
			glVertex3f(-0.5, -1.0, -0.25);
			glVertex3f(-0.5, -1.0, 0.25);
		glEnd();
		/////////////////////////////////////////right face
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glVertex3f(0.5, -0.0, 0.25);
			glVertex3f(0.5, -0.0, -0.25);
			glVertex3f(0.5, -1.0, -0.25);
			glVertex3f(0.5, -1.0, 0.25);
		glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger
	drawFinger();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void display()
{
	switch (Qnum) {
	case 1:
		arm();
		break;
	case 2:
		break;
	default:
		robot();
		break;
	}
}
//--------------------------------------------------------------------



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		1000, 10, 800, 800,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------