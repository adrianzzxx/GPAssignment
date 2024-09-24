
#include <Windows.h>
#include <windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <stdio.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "Assignment"

static int lastX = -1;
static int lastY = -1;

bool Ran = false, CV = true;

int Qnum = 1;

float height = 0;

float OLeft = -10.0, ORight = 10.0, ODown = -10.0, OUp = 10.0, ONear = -10.0, OFar = 10.0;
float PNear = 1.0, PFar = 20.0;
float SphereR = 3.0;
float ptX = 0.0, ptY = 0.0, ptZ = 0.0;

float MoveX, MoveY, MoveZ;
float RotateS = 0.0, RotateX = 0.0, RotateY = 0.0, RotateZ = 0.0;
float ScaleX = 1.0, ScaleY = 1.0, ScaleZ = 1.0;

float xPosition = 0.0f, yPosition = 0.0f, zPosition = 0.05f;


/////////////----------------------------------body movement variables

float txb = 0, tyb = 0, tzb = 0; // translate body

//////////////--------------------------------  leg movement variables
float ry = 1, rx = 0, rp = 0; // rotate checking purpose

float rlleg1 = 0, rlleg2 = 0, rlleg3 = 0; // rotate left leg
float rrleg1 = 0, rrleg2 = 0, rrleg3 = 0; // rotate right leg

float txwlleg = 0, tywlleg = 0, tzwlleg = 0; // translate whole left leg
float txwrleg = 0, tywrleg = 0, tzwrleg = 0; // translate whole right leg

float txlleg1 = 0, tylleg1 = 0, tzlleg1 = 0; // translate left upper knee
float txrleg1 = 0, tyrleg1 = 0, tzrleg1 = 0; // translate right upper knee

float txlleg2 = 0, tylleg2 = 0, tzlleg2 = 0; // translate left below knee
float txrleg2 = 0, tyrleg2 = 0, tzrleg2 = 0; // translate right below knee

float txwhole = 0, tywhole = 0, tzwhole = 0; // translate whole body
float left = 0, right = 0;
bool legcond = true, walking = true; //-------leg movement condition
float count = 0;

//projection
bool isOrtho = true;
float ty = 0, tx = 0, tz = 0, tSpeed = 1;
//float ONear = -3, OFar = 3;
float ptx = 0, pty = 0, ptSpeed = 0.1;
float prx = 0, pry = 0, prSpeed = 1;
float zoom = -7;
float rotateX = 0;
float rotateY = 0;
int textureChange = 1;
bool changing = false;

//Lighting
float amb[3] = { 1.0,0,0 }; // red color ambient light
float posA[3] = { 0.8,0.0,0.0 }; // amb light pos(x,y,z)
float dif[4] = { 1, 1.0, 1.0, 1.0 }; // Green diffuse light (RGBA)
float posD[4] = { 0.0, 0.0, 0.0, 1.0 }; // Diffuse light position (x, y, z, w)
float ambM[3] = { 1.0,0.0,0 }; // blue color ambient material
float difM[3] = { 1.0,1.0,1.0 }; // blue color diffuse material
bool isLightOn = false;
float angle = 0.0;

/////////////----------------------------------color variables
#define red 0.663,0.188,0.216
#define white 1.0,1.0,1.0
#define black 0.0,0.0,0.0
#define lightblack 0.2,0.2,0.2
#define yellow 0.93, 1.0, 0.26

/////////////----------------------------------arm movement variables
float LUARotateX = 0.0, RUARotateX = 0.0, LLARotateX = 0.0, RLARotateX = 0.0, LPRotate = 0.0, RPRotate = 0.0, FRotate = 0.0;
float LUARotateY = 0.0, RUARotateY = 0.0, LLARotateY = 0.0, RLARotateY = 0.0;
float LUARotateZ = 0.0, RUARotateZ = 0.0, LLARotateZ = 0.0, RLARotateZ = 0.0;

/////////////----------------------------------weapon movement variables
float WeaponSpeed = 0.05, WeaponS1 = 0.0, WeaponS2 = 0.0, WeaponS3 = 0.0;

/////////////----------------------------------action bool
bool TogFinger = false, TogWeapon = false, TogOnWeapon = false;
bool Act1 = false, Act2 = false, Act3 = false;
bool Slash = false, VSlash = false;
bool SwingFront = false;
bool TogWalk = false;

BITMAP BMP;  //bitmap structure
HBITMAP hBMP = NULL;   ///bitmap handle
int change = 0;

GLuint loadTexture(LPCSTR filename) {

    GLuint texture = 0; //texture name
    //step 3
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
        filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
        LR_LOADFROMFILE);

    GetObject(hBMP, sizeof(BMP), &BMP);


    //step 4
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
        BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

    DeleteObject(hBMP);

    return texture;
}

void resetleg() {
	// Reset left leg variables
	rlleg1 = 0;    // Reset rotation or position
	rlleg2 = 0;    // Reset any additional rotation or state
	left = 0;     // Reset the 'left' movement state
	tylleg2 = 0;   // Reset any transformation
	txlleg2 = 0;   // Reset any translation

	// Reset right leg variables
	rrleg1 = 0;    // Reset rotation or position
	rrleg2 = 0;    // Reset any additional rotation or state
	right = 0;    // Reset the 'right' movement state
	tyrleg2 = 0;   // Reset any transformation
	txrleg2 = 0;   // Reset any translation
}

void upleftleg() {
	if (rlleg1 < 0 && left >= 30) {
		rlleg1 += 0.3;
		if (rlleg1 > -20 && rlleg2 > 0) {
			rlleg2 -= 0.6;
			tylleg2 += 0.02;
			txlleg2 += 0.064;
		}
		if (rlleg1 >= 0) {
			left = 0;
		}
	}
	else if (rlleg1 < -30 && rlleg2 < 30) {
		rlleg1 = -30;
		rlleg2 += 0.6;
		tylleg2 -= 0.02;
		txlleg2 -= 0.064;
		left += 0.6;
	}
	else {
		rlleg1 -= 0.3;
	}


}


void backrightleg() {

	if (rrleg1 > 0 && right >= 30) {
		rrleg1 -= 0.3;
		if (rrleg1 < 20 && rrleg2 > 0) {
			rrleg2 -= 0.6;
			tyrleg2 += 0.02;
			txrleg2 += 0.064;
		}
		if (rrleg1 <= 0) {
			right = 0;
		}
	}
	else if (rrleg1 > 30 && rrleg2 < 30) {
		rrleg1 = 30;
		rrleg2 += 0.6;
		tyrleg2 -= 0.02;
		txrleg2 -= 0.064;
		right += 0.6;
	}
	else {
		rrleg1 += 0.3;
	}
}

void uprightleg() {
	if (rrleg1 < 0 && right >= 30) {
		rrleg1 += 0.3;
		if (rrleg1 > -20 && rrleg2 > 0) {
			rrleg2 -= 0.6;
			tyrleg2 += 0.02;
			txrleg2 += 0.064;
		}
		if (rrleg1 >= 0) {
			right = 0;
		}
	}
	else if (rrleg1 < -30 && rrleg2 < 30) {
		rrleg1 = -30;
		rrleg2 += 0.6;
		tyrleg2 -= 0.02;
		txrleg2 -= 0.064;
		right += 0.6;
	}
	else {
		rrleg1 -= 0.3;
	}
}

void backleftleg() {
	if (rlleg1 > 0 && left >= 30) {
		rlleg1 -= 0.3;
		if (rlleg1 < 20 && rlleg2 > 0) {
			rlleg2 -= 0.6;
			tylleg2 += 0.02;
			txlleg2 += 0.064;
		}
		if (rlleg1 <= 0) {
			left = 0;
		}
	}
	else if (rlleg1 > 30 && rlleg2 < 30) {
		rlleg1 = 30;
		rlleg2 += 0.6;
		tylleg2 -= 0.02;
		txlleg2 -= 0.064;
		left += 0.6;
	}
	else {
		rlleg1 += 0.3;
	}
}


void leftmovefront() {
	upleftleg();
	txwhole -= 0.1;
	backrightleg();

}

void rightmovefront() {
	uprightleg();
	txwhole -= 0.1;
	backleftleg();
}

void walk() {
	if (legcond == true) {
		leftmovefront();
		count += 1;
		if (count >= 297) {
			resetleg();
			count = 0;
			legcond = false;
		}
	}
	else {
		rightmovefront();
		count += 1;
		if (count >= 297) {
			resetleg();
			count = 0;
			legcond = true;
		}

	}
}

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN: {
		// Capture the initial X position when the drag starts
		lastX = GET_X_LPARAM(lParam);
		lastY = GET_Y_LPARAM(lParam);
		break;
	}

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {  // Ensure the left button is held down (dragging)
			int currentX = GET_X_LPARAM(lParam);  // Get the current X position

			if (lastX != -1) {  // Check if we have a valid previous position
				if (currentX > lastX) {
					// Mouse moved to the right, increase prSpeed
					pry += 1.0f;  // You can adjust the increment value as needed
				}
				else if (currentX < lastX) {
					// Mouse moved to the left, decrease prSpeed
					pry -= 1.0f;  // You can adjust the decrement value as needed
				}
			}

			int currentY = GET_Y_LPARAM(lParam);  // Get the current X position

			if (lastY != -1) {  // Check if we have a valid previous position
				if (currentY > lastY) {
					// Mouse moved to the right, increase prSpeed
					prx += 1.0f;  // You can adjust the increment value as needed
				}
				else if (currentY < lastY) {
					// Mouse moved to the left, decrease prSpeed
					prx -= 1.0f;  // You can adjust the decrement value as needed
				}
			}

			// Update the last X position
			lastX = currentX;
			lastY = currentY;
		}
		break;

	case WM_LBUTTONUP: {
		// Reset last X position when dragging ends
		lastX = -1;
		lastY = -1;
		break;
	}

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == VK_SPACE) {
			prSpeed = 0.0;
			ptX = 0.0;
			ptY = 0.0;
			ptZ = 0.0;
			LUARotateX = 0.0;
			LUARotateY = 0.0;
			LUARotateZ = 0.0;
			LLARotateX = 0.0;
			LLARotateY = 0.0;
			LLARotateZ = 0.0;
			RUARotateX = 0.0;
			RUARotateY = 0.0;
			RUARotateZ = 0.0;
			RLARotateX = 0.0;
			RLARotateY = 0.0;
			RLARotateZ = 0.0;
			TogFinger = false;
			TogWeapon = false;
			TogOnWeapon = false;
			Act1 = false;
			Act2 = false;
			Act3 = false;
			Slash = false;
			VSlash = false;
			SwingFront = false;
			resetleg();
		}
		else if (wParam == '2') {
			Qnum = 2;
		}
		else if (wParam == '3') {
			LUARotateX -= 1;
		}
		else if (wParam == '4') {
			LLARotateX -= 1;
		}
		else if (wParam == '5') {
			LPRotate -= 1;
		}
		else if (wParam == '6') {
			RUARotateX -= 1;
		}
		else if (wParam == '7') {
			RLARotateX -= 1;
		}
		else if (wParam == '8') {
			RPRotate -= 1;
		}
		else if (wParam == '9') {
			TogFinger = !TogFinger;
		}
		else if (wParam == 'Z') {
			TogFinger = !TogFinger;
			TogWeapon = !TogWeapon;
			TogOnWeapon = false;
		}
		else if (wParam == 'X') {
			TogOnWeapon = !TogOnWeapon;
		}
		else if (wParam == VK_NUMPAD1) {
			Act1 = !Act1;
		}
		else if (wParam == VK_NUMPAD2) {
			Act2 = !Act2;
		}
		else if (wParam == VK_NUMPAD3) {
			Act3 = !Act3;
		}

		else if (wParam == 'M') { //--------------walking
			TogWalk = !TogWalk;
			Act3 = !Act3;
		}
		else if (wParam == VK_UP)           //move robot up
		{
			if (isOrtho)
			{
				if (ty < 3)
				{
					ty += tSpeed;
				}
			}
			else
			{
				if (ty < 8)
				{
					ty += tSpeed;
				}
			}
		}
		else if (wParam == VK_DOWN)         //move robot down
		{
			if (isOrtho)
			{
				if (ty > -3)
				{
					ty -= tSpeed;
				}
			}
			else
			{
				if (ty > -8)
				{
					ty -= tSpeed;
				}
			}
		}
		else if (wParam == VK_LEFT)         //move robot left
		{
			if (isOrtho)
			{
				if (tx < 30)
				{
					tx += tSpeed;
				}
			}
			else
			{
				if (tx < 50)
				{
					tx += tSpeed;
				}
			}
		}
		else if (wParam == VK_RIGHT)        //move robot right
		{
			if (isOrtho)
			{
				if (tx > -30)
				{
					tx -= tSpeed;
				}
			}
			else
			{
				if (tx > -50)
				{
					tx -= tSpeed;
				}
			}
		}
		else if (wParam == 'O')
		{
			isOrtho = true;
			tx = 0, ty = 0, ptx = 0, pty = 0, prx = 0, pry = 0;
		}
		else if (wParam == 'P')
		{
			isOrtho = false;
			tx = 0, ty = 0, ptx = 0, pty = 0, prx = 0, pry = 0;
			zoom = -7;
		}
		else if (wParam == 'A')              //move projection left
		{
			if (isOrtho)
			{
				if (ptx > -0.35)
				{
					ptx -= ptSpeed;
				}
			}
			else
			{
				if (ptx > -0.8)
				{
					ptx -= ptSpeed;
				}
			}
		}
		else if (wParam == 'D')              //move projection right
		{
			if (isOrtho)
			{
				if (ptx < 0.35)
				{
					ptx += ptSpeed;
				}
			}
			else
			{
				if (ptx < 0.8)
				{
					ptx += ptSpeed;
				}
			}
		}
		else if (wParam == 'W')              //move projection up
		{
			if (isOrtho)
			{
				if (pty < 0.35)
				{
					pty += ptSpeed;
				}
			}
			else
			{
				if (pty < 0.8)
				{
					pty += ptSpeed;
				}
			}
		}
		else if (wParam == 'S')              //move projection down
		{
			if (isOrtho)
			{
				if (pty > -0.35)
				{
					pty -= ptSpeed;
				}
			}
			else {
				if (pty > -0.8)
				{
					pty -= ptSpeed;
				}
			}
		}
		else if (wParam == 'T')              //move robot nearer to view
		{
			if (!isOrtho)
			{
				if (zoom < -5.5)
				{
					zoom += 0.5;
				}
			}
		}
		else if (wParam == 'G')              //move robot farther to view
		{
			if (!isOrtho)
			{
				if (zoom > -54)
				{
					zoom -= 0.5;
				}
			}
		}
		else if (wParam == 'N')       
		{
			change += 1;
			if (change == 5)
				change = 0;
		}

		//light key
		else if (wParam == 'L') {
			isLightOn = !isLightOn;
		}
		else if (wParam == 'U') {
			posD[1] += 0.06;
		}
		else if (wParam == 'J') {
			posD[1] -= 0.06;
		}
		else if (wParam == 'H') {
			posD[0] -= 0.06;
		}
		else if (wParam == 'K') {
			posD[0] += 0.06;
		}
		else if (wParam == 'Y') {
			posD[2] -= 0.06;
		}
		else if (wParam == 'I') {
			posD[2] += 0.06;
		}

		else if (wParam == 0xDB) {
			angle += 5;
		}
		
		else if (wParam == 0xDD) {
			angle -= 5;
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
	glMatrixMode(GL_PROJECTION);			 //refer to the projection matrix
	glLoadIdentity();						 //reset the projection matrix
	glTranslatef(ptx, pty, 0);				 //translate x and y for projection			
	glRotatef(prx, 1, 0, 0);
	glRotatef(pry, 0, 1, 0);	             //rotate y for projection	

	if (isOrtho)
	{
		glOrtho(OLeft, ORight, ODown, OUp, ONear, OFar);
	}
	else
	{
		gluPerspective(70, 1, 0.6, 51);
		glTranslatef(0, 0, zoom);
		glRotatef(0, 0, 1, 0);
	}
}

void lighting() {
    if (isLightOn) {
        glEnable(GL_LIGHTING);     // enable lighting for the whole scene
    }
    else {
        glDisable(GL_LIGHTING);
    }

    // Rotate the light
    glPushMatrix(); // Save the current transformation matrix
    glRotatef(angle, 0.0f, 0.0f, 1.0f); // Rotate around the Y-axis

    //light 0 : white color ambient light at pos(0,0.8,0), above the sphere
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_POSITION, posA);
    //glEnable(GL_LIGHT0);

    //light 1 : white color diffuse light at pos(0.8,0.0,0), right the sphere
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
    glLightfv(GL_LIGHT1, GL_POSITION, posD);
    glEnable(GL_LIGHT1);

    glPopMatrix();

}

void drawCube1(float size) {
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-size, size, size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, size, -size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, -size, size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-size, size, size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size, -size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-size, -size, size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, -size, -size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, size);
	glEnd();
}

void drawSideCube(float size) {

	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size - (size * 0.2), size - (size * 0.2));
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size - (-size * 0.2), size - (size * 0.2));
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size - (size * 0.2), -size - (-size * 0.2));
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size - (-size * 0.2), -size - (-size * 0.2));
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-size, size - (size * 0.2), size - (size * 0.2));
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, size - (size * 0.2), -size - (-size * 0.2));
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, size, -size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, -size, size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size, -size - (-size * 0.2), size - (size * 0.2));
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size, -size - (-size * 0.2), -size - (-size * 0.2));
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-size - (-size * 0.2), size - (size * 0.2), size - (size * 0.2));
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-size - (-size * 0.2), size - (size * 0.2), -size - (-size * 0.2));
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-size - (-size * 0.2), -size - (-size * 0.2), -size - (-size * 0.2));
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-size - (-size * 0.2), -size - (-size * 0.2), size - (size * 0.2));

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, -size, -size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, -size, size);
	glEnd();
}

void drawSideCubeLine(float size) {

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0);

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, size);
	glVertex3f(-size, size - (size * 0.2), size - (size * 0.2));
	glVertex3f(-size, -size - (-size * 0.2), size - (size * 0.2));
	glVertex3f(size, -size, size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, -size);
	glVertex3f(-size, size - (size * 0.2), -size - (-size * 0.2));
	glVertex3f(-size, -size - (-size * 0.2), -size - (-size * 0.2));
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, size);
	glVertex3f(-size, size - (size * 0.2), size - (size * 0.2));
	glVertex3f(-size, size - (size * 0.2), -size - (-size * 0.2));
	glVertex3f(size, size, -size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, -size, size);
	glVertex3f(-size, -size - (-size * 0.2), size - (size * 0.2));
	glVertex3f(-size, -size - (-size * 0.2), -size - (-size * 0.2));
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-size - (-size * 0.2), size - (size * 0.2), size - (size * 0.2));
	glVertex3f(-size - (-size * 0.2), size - (size * 0.2), -size - (-size * 0.2));
	glVertex3f(-size - (-size * 0.2), -size - (-size * 0.2), -size - (-size * 0.2));
	glVertex3f(-size - (-size * 0.2), -size - (-size * 0.2), size - (size * 0.2));
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, size);
	glVertex3f(size, size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glEnd();
}

void drawCube1Line(float size) {
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, -size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-size, size, size);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size, size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, size);
	glVertex3f(size, size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glEnd();
}

void drawSphere(float r, int slice, int stack) {
	GLUquadricObj* sphere = NULL;   // create a quadric obj pointer
	sphere = gluNewQuadric();       // create a new quadric obj in the memory
	glLineWidth(1.0);
	gluQuadricDrawStyle(sphere, GLU_FILL);//in line style
	gluSphere(sphere, r, slice, stack); // draw sphere
	gluDeleteQuadric(sphere); //delete the quadric ob
}

void drawSphereLine(float r, int slice, int stack) {
	GLUquadricObj* sphere = NULL;   // create a quadric obj pointer
	sphere = gluNewQuadric();       // create a new quadric obj in the memory
	glLineWidth(1.0);
	gluQuadricDrawStyle(sphere, GLU_LINE);//in line style
	gluSphere(sphere, r, slice, stack); // draw sphere
	gluDeleteQuadric(sphere); //delete the quadric ob
}

void drawCylinder(float br, float tr, float h) {
	GLUquadricObj* cylinder = NULL;   // create a quadric obj pointer
	cylinder = gluNewQuadric();       // create a new quadric obj in the memory
	glLineWidth(1.0);
	gluQuadricDrawStyle(cylinder, GLU_FILL);//in line style ,GLU_POINT
	gluCylinder(cylinder, br, tr, h, 30, 30); // draw sphere
	gluDeleteQuadric(cylinder); //delete the quadric ob
}

void drawCylinderLine(float br, float tr, float h) {
	GLUquadricObj* cylinder = NULL;   // create a quadric obj pointer
	cylinder = gluNewQuadric();       // create a new quadric obj in the memory
	glLineWidth(1.5);
	gluQuadricDrawStyle(cylinder, GLU_LINE);//in line style ,GLU_POINT
	gluCylinder(cylinder, br, tr, h, 5, 5); // draw sphere
	gluDeleteQuadric(cylinder); //delete the quadric ob
}

void drawCone(float tr, float h) {
	GLUquadricObj* cone = NULL;   // create a quadric obj pointer
	cone = gluNewQuadric();       // create a new quadric obj in the memory

	glLineWidth(1.0);
	gluQuadricDrawStyle(cone, GLU_FILL);//in line style
	gluCylinder(cone, 0.0, tr, h, 30, 30); // draw sphere
	gluDeleteQuadric(cone);  //delete the quadric ob
}

void drawRTriPyramid(float size) {
	// Adjusting coordinates to make right-angled triangles
	float apexX = 0.0f;
	float apexY = size;
	float apexZ = 0.0f;

	float base1X = 0.0f;
	float base1Y = 0.0f;
	float base1Z = 0.0f;

	float base2X = size;
	float base2Y = 0.0f;
	float base2Z = 0.0f;

	float base3X = 0.0f;
	float base3Y = 0.0f;
	float base3Z = size;

	// Drawing the base using GL_TRIANGLES
	glBegin(GL_TRIANGLES);
	glVertex3f(base1X, base1Y, base1Z);
	glVertex3f(base2X, base2Y, base2Z);
	glVertex3f(base3X, base3Y, base3Z);
	glEnd();

	// Drawing the sides using GL_TRIANGLES
	glBegin(GL_TRIANGLES);
	glVertex3f(apexX, apexY, apexZ);
	glVertex3f(base1X, base1Y, base1Z);
	glVertex3f(base2X, base2Y, base2Z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(apexX, apexY, apexZ);
	glVertex3f(base2X, base2Y, base2Z);
	glVertex3f(base3X, base3Y, base3Z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(apexX, apexY, apexZ);
	glVertex3f(base3X, base3Y, base3Z);
	glVertex3f(base1X, base1Y, base1Z);
	glEnd();
}

void drawRTriPyramidLine(float size) {
	// Adjusting coordinates to make right-angled triangles
	float apexX = 0.0f;
	float apexY = size;
	float apexZ = 0.0f;

	float base1X = 0.0f;
	float base1Y = 0.0f;
	float base1Z = 0.0f;

	float base2X = size;
	float base2Y = 0.0f;
	float base2Z = 0.0f;

	float base3X = 0.0f;
	float base3Y = 0.0f;
	float base3Z = size;

	// Drawing the base using GL_TRIANGLES
	glBegin(GL_LINE_LOOP);
	glVertex3f(base1X, base1Y, base1Z);
	glVertex3f(base2X, base2Y, base2Z);
	glVertex3f(base3X, base3Y, base3Z);
	glEnd();

	// Drawing the sides using GL_TRIANGLES
	glBegin(GL_LINE_LOOP);
	glVertex3f(apexX, apexY, apexZ);
	glVertex3f(base1X, base1Y, base1Z);
	glVertex3f(base2X, base2Y, base2Z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(apexX, apexY, apexZ);
	glVertex3f(base2X, base2Y, base2Z);
	glVertex3f(base3X, base3Y, base3Z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(apexX, apexY, apexZ);
	glVertex3f(base3X, base3Y, base3Z);
	glVertex3f(base1X, base1Y, base1Z);
	glEnd();
}

void drawPyramidLine(float size) {
	// Drawing the base using GL_QUADS
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, size);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();

	// Drawing the sides using GL_TRIANGLES
	glBegin(GL_LINE_LOOP);
	glVertex3f(size / 2, size, size / 2);  // Top vertex common to all sides
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();
}

void drawPyramid(float size) {
	// Drawing the base using GL_QUADS
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, size);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();

	// Drawing the sides using GL_TRIANGLES
	glBegin(GL_TRIANGLES);
	glVertex3f(size / 2, size, size / 2);  // Top vertex common to all sides
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();
}

void drawTriangularPrism(float size) {
	// Vertices of the first triangle (base)
	float triangle1Vertex1[] = { 0.0f, 0.0f, 0.0f };
	float triangle1Vertex2[] = { size, 0.0f, 0.0f };
	float triangle1Vertex3[] = { size / 2, 0.0f, sqrt(3) * size / 2 }; // This forms an equilateral triangle

	// Height of the prism along the y-axis
	float height = size;

	// Drawing the first triangle (base)
	glBegin(GL_TRIANGLES);
	glVertex3fv(triangle1Vertex1);
	glVertex3fv(triangle1Vertex2);
	glVertex3fv(triangle1Vertex3);
	glEnd();

	// Vertices of the second triangle (parallel to the first)
	float triangle2Vertex1[] = { 0.0f, height, 0.0f };
	float triangle2Vertex2[] = { size, height, 0.0f };
	float triangle2Vertex3[] = { size / 2, height, sqrt(3) * size / 2 };

	// Drawing the second triangle (top)
	glBegin(GL_TRIANGLES);
	glVertex3fv(triangle2Vertex1);
	glVertex3fv(triangle2Vertex2);
	glVertex3fv(triangle2Vertex3);
	glEnd();

	// Drawing the connecting rectangles
	glBegin(GL_QUADS);
	// Connect vertex1 of both triangles
	glVertex3fv(triangle1Vertex1);
	glVertex3fv(triangle1Vertex2);
	glVertex3fv(triangle2Vertex2);
	glVertex3fv(triangle2Vertex1);
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex2 of both triangles
	glVertex3fv(triangle1Vertex2);
	glVertex3fv(triangle1Vertex3);
	glVertex3fv(triangle2Vertex3);
	glVertex3fv(triangle2Vertex2);
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex3 of both triangles
	glVertex3fv(triangle1Vertex3);
	glVertex3fv(triangle1Vertex1);
	glVertex3fv(triangle2Vertex1);
	glVertex3fv(triangle2Vertex3);
	glEnd();
}

void drawTriangularPrismLine(float size) {
	// Vertices of the first triangle (base)
	float triangle1Vertex1[] = { 0.0f, 0.0f, 0.0f };
	float triangle1Vertex2[] = { size, 0.0f, 0.0f };
	float triangle1Vertex3[] = { size / 2, 0.0f, sqrt(3) * size / 2 }; // This forms an equilateral triangle

	// Height of the prism along the y-axis
	float height = size;

	// Vertices of the second triangle (parallel to the first)
	float triangle2Vertex1[] = { 0.0f, height, 0.0f };
	float triangle2Vertex2[] = { size, height, 0.0f };
	float triangle2Vertex3[] = { size / 2, height, sqrt(3) * size / 2 };

	// Drawing the first triangle (base)
	glBegin(GL_LINE_LOOP);
	glVertex3fv(triangle1Vertex1);
	glVertex3fv(triangle1Vertex2);
	glVertex3fv(triangle1Vertex3);
	glEnd();

	// Drawing the second triangle (top)
	glBegin(GL_LINE_LOOP);
	glVertex3fv(triangle2Vertex1);
	glVertex3fv(triangle2Vertex2);
	glVertex3fv(triangle2Vertex3);
	glEnd();

	// Drawing the connecting lines between the triangles
	glBegin(GL_LINES);
	// Connect vertex1 of both triangles
	glVertex3fv(triangle1Vertex1);
	glVertex3fv(triangle2Vertex1);
	glEnd();

	glBegin(GL_LINES);
	// Connect vertex2 of both triangles
	glVertex3fv(triangle1Vertex2);
	glVertex3fv(triangle2Vertex2);
	glEnd();

	glBegin(GL_LINES);
	// Connect vertex3 of both triangles
	glVertex3fv(triangle1Vertex3);
	glVertex3fv(triangle2Vertex3);
	glEnd();
}

void drawCube(float size) {
	glBegin(GL_QUADS);
	// Face 1 : BOTTOM
	glVertex3f(0.0f, 0.0f, size);//1
	glVertex3f(size, 0.0f, size);//2
	glVertex3f(size, 0.0f, 0.0f);//3
	glVertex3f(0.0f, 0.0f, 0.0f);//4
	glEnd();
	// Face 2 : 
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);//4 last point of face one
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();
	// Face 3 : 
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, 0.0f, size);
	glEnd();
	// Face 4 : 
	glBegin(GL_QUADS);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glEnd();
	// Face 5 : 
	glBegin(GL_QUADS);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(size, size, 0.0f);
	glEnd();
	// Face 6 : 
	glBegin(GL_QUADS);
	glVertex3f(size, size, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glEnd();
}

void drawCubeLine(float size) {
	glBegin(GL_LINE_LOOP);
	// Face 1 : BOTTOM
	glVertex3f(0.0f, 0.0f, size);//1
	glVertex3f(size, 0.0f, size);//2
	glVertex3f(size, 0.0f, 0.0f);//3
	glVertex3f(0.0f, 0.0f, 0.0f);//4
	glEnd();
	// Face 2 : 
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, 0.0f);//4 last point of face one
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();
	// Face 3 : 
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, 0.0f, size);
	glEnd();
	// Face 4 : 
	glBegin(GL_LINE_LOOP);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glEnd();
	// Face 5 : 
	glBegin(GL_LINE_LOOP);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(size, size, 0.0f);
	glEnd();
	// Face 6 : 
	glBegin(GL_LINE_LOOP);
	glVertex3f(size, size, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glEnd();
}

void drawTextSphere(double r) {
	GLUquadricObj* sphere = NULL;   // create a quadric obj pointer
	sphere = gluNewQuadric();       // create a new quadric obj in the memory
	//glLineWidth(1.0);
	gluQuadricTexture(sphere, true);//in line style
	gluSphere(sphere, r, 30, 30); // draw sphere
	gluDeleteQuadric(sphere); //delete the quadric ob
}



void leg() {

	///------------------------------------------------ left leg

	glPushMatrix(); //----------------------------------------------------- rotate left leg 1 - whole leg
	glRotatef(rlleg1, 0, 0, 1);
	glTranslatef(txwlleg, tywlleg, tzwlleg);



	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(0, 0, -0.75);

	glPushMatrix();
	glTranslatef(0, -0.85, 3);
	glColor3f(0.247, 0.247, 0.251);
	drawSphere(1, 30, 30);
	glPopMatrix();

	// left upper leg

	glPushMatrix();
	glTranslatef(0, -1.2, 3);
	glColor3f(0.247, 0.247, 0.251);
	glRotatef(90, 1, 0, 0);
	drawCylinder(0.3, 0.3, 5);
	glPopMatrix();

	//--------------------------------------------------------------------------------------------------upper leg decorations


	glPushMatrix();
	glTranslatef(-1, -5.2, 2);
	//glRotatef(235, 0, 0, 1);
	glScalef(1, 1.8, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, -5.7, 2.25);
	//glRotatef(235, 0, 0, 1);
	glScalef(0.8, 1.5, 0.8);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.949, 0.949, 0.949);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -5.2, 3.25);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.2, 0.2, 1.3);
	glColor3f(0.949, 0.949, 0.949);
	drawCylinder(0.2, 0.2, 1.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -5.2, 2.75);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.2, 0.2, 1.3);
	glColor3f(0.949, 0.949, 0.949);
	drawCylinder(0.2, 0.2, 1.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -5.2, 3.25);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.2, 0.2, 1.3);
	glColor3f(0.949, 0.949, 0.949);
	drawCylinder(0.2, 0.2, 1.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -5.2, 2.75);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.2, 0.2, 1.3);
	glColor3f(0.949, 0.949, 0.949);
	drawCylinder(0.2, 0.2, 1.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.99999, -5.799999, 3.499999999);
	//glRotatef(235, 0, 0, 1);
	glScalef(0.99, 1.8, 0.25);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.99999, -5.799999, 2);
	//glRotatef(235, 0, 0, 1);
	glScalef(0.99, 1.8, 0.25);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.5, -5.799999, 2);
	//glRotatef(235, 0, 0, 1);
	glScalef(0.25, 1.8, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, -5.4, 1.9);
	//glRotatef(235, 0, 0, 1);
	//glScalef(0.25, 1.8, 1);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.3, 0.3, 2.2);
	glColor3f(0, 0, 0);
	drawCylinder(0.3, 0.3, 2.2);
	glPopMatrix();



	//--------------------------------------------------------------------------------------------------end of upper leg decorations



	// ---------------------------------------------------------left knee
	glPushMatrix();
	glTranslatef(-0.25, -6.8, 3);
	glColor3f(0.247, 0.247, 0.251);
	drawSphere(0.6, 30, 30);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix(); //------------------------------------------------- rotate left leg 2 - below knee
	glTranslatef(txlleg2, tylleg2, tzlleg2);
	glRotatef(rlleg2, 0, 0, 1);

	glPushMatrix();
	glScalef(1, 1, -1); // Mirroring transformation
	glTranslatef(0, 0, -0.75);

	// left bottom leg
	glPushMatrix();
	glTranslatef(0, -7, 3);
	glColor3f(0.247, 0.247, 0.251);
	glRotatef(90, 1, 0, 0);
	drawCylinder(0.3, 0.3, 5);
	glPopMatrix();

	//-------------------------------------------------------------------------------------------------------botton leg decorations

	//glPushMatrix();
	//glTranslatef(-1, -10, 2.75);// z - axis diff with left----front quad base pyramid1
	//glRotatef(215, 0, 0, 1);
	//glScalef(2, 0.5, 0.5);
	//glColor3f(0, 0, 0);
	//drawPyramidLine(1);
	//glColor3f(1, 1, 1);
	//drawPyramid(1);
	//glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -10.2, 2.75);// z - axis diff with left----front quad base pyramid1
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	/// first layer
	glPushMatrix();
	glTranslatef(-0.6, -10.8, 2.75);// z - axis diff with left----front quad base pyramid1
	glRotatef(215, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//----
	glPushMatrix();
	glTranslatef(-0.6, -10.9, 2.45);// z - axis diff with left-
	glRotatef(215, 0, 0, 1);
	glRotatef(15, 1, 0, 0);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -11.2, 2.25);// z - axis diff with left-
	glRotatef(215, 0, 0, 1);
	glRotatef(45, 1, 0, 0);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	//----

	glPushMatrix();
	glTranslatef(-0.6, -10.8, 3.05);// z - axis diff with left-
	glRotatef(215, 0, 0, 1);
	glRotatef(-15, 1, 0, 0);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -10.8, 3.35);// z - axis diff with left-
	glRotatef(215, 0, 0, 1);
	glRotatef(-45, 1, 0, 0);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.45, -10.2, 3.25);// z - axis diff with left
	glRotatef(180, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.35, -9.8, 2.65);
	//glRotatef(285, 0, 0, 1);
	glScalef(0.7, 2, 0.7);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	//-------------------------- layer front
	//glPushMatrix();
	//glTranslatef(-0.5, -9, 2.75);// z - axis diff with left
	//glRotatef(235, 0, 0, 1);
	//glScalef(1.5, 0.5, 0.5);
	//glColor3f(0, 0, 0);
	//drawPyramidLine(1);
	//glColor3f(1, 1, 1);
	//drawPyramid(1);
	//glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, -9, 2.75);// z - axis diff with left ---2
	glRotatef(285, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.25, -8, 2.75);// z - axis diff with left --3
	glRotatef(290, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.75, -7, 2.75);// z - axis diff with left --4
	glRotatef(295, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.25, -5.5, 2.75);// z - axis diff with left--5
	glRotatef(295, 0, 0, 1);
	glScalef(3, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//--------------------------end of layer front

	//-----------------------fifth layer side

	glPushMatrix();
	glTranslatef(0.6, -6.25, 3.25);// layer 5 back
	glRotatef(180, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, -6.25, 3.5);
	glRotatef(65, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, -6.25, 3.6);
	glRotatef(90, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, -6.25, 3.6);
	glRotatef(135, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -6.25, 2.35);
	glRotatef(-90, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, -6.25, 2.25);
	glRotatef(-65, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, -6.25, 2.7);
	glRotatef(-135, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	//-------------- end of fifth layer

	glPushMatrix();
	glTranslatef(-0.85, -7, 2.75);//layer 4 front 2
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, -7, 3.25);// layer 4 back
	glRotatef(180, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();



	//----------------------fourth layer side

	glPushMatrix();
	glTranslatef(-0.7, -7, 3.25);
	glRotatef(45, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, -7, 3.6);
	glRotatef(90, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, -7, 3.6);
	glRotatef(135, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -7, 2.35);
	glRotatef(-90, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, -7, 2.4);
	glRotatef(-45, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, -7, 2.7);
	glRotatef(-135, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();




	//----------------------end of fourth layer side


	glPushMatrix();
	glTranslatef(0.75, -8, 3.25);// third layer back
	glRotatef(180, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//----------------------third layer side

	glPushMatrix();
	glTranslatef(-0.75, -8, 3.35);
	glRotatef(45, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, -8, 3.75);
	glRotatef(90, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, -8, 3.65);
	glRotatef(135, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.35, -8, 2.35);
	glRotatef(-45, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.25, -8, 2.25);
	glRotatef(-90, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.65, -8, 2.65);
	glRotatef(-135, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();



	//----------------------end of third layer side



	//---------------------second layer side
	glPushMatrix();
	glTranslatef(0.75, -9, 3.25);// second layer back
	glRotatef(180, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, -9, 3.35);// right
	glRotatef(45, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, -9, 3.75);// right
	glRotatef(90, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, -9, 3.7);// right
	glRotatef(135, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.35, -9, 2.35);// left
	glRotatef(-25, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -9, 2.25);// left
	glRotatef(-90, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.75, -9, 2.65);// left
	glRotatef(-135, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//----------------------end of second layer size

	//------------------------ first layer side
	glPushMatrix();//-0.5
	glTranslatef(0, -10.2, 3.25);// z - axis diff with left
	glRotatef(90, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0, -10.2, 3.25);// z - axis diff with left
	glRotatef(112.5, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.15, -10.2, 3.25);// z - axis diff with left
	glRotatef(135, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.15, -10.2, 3.25);// z - axis diff with left
	glRotatef(157.5, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.5, -10.2, 3);// z - axis diff with left
	glRotatef(15, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.5, -10.2, 3);// z - axis diff with left
	glRotatef(50, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.35, -10.2, 3.15);// z - axis diff with left
	glRotatef(75, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//------------------------
	glPushMatrix();//-0.5
	glTranslatef(0.25, -10.2, 2.75);// z - axis diff with left
	glRotatef(-90, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.45, -10.2, 2.75);// z - axis diff with left
	glRotatef(-112.5, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.4, -10.2, 3);// z - axis diff with left
	glRotatef(-135, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	glPushMatrix();//-0.5
	glTranslatef(0.4, -10.2, 3.1);// z - axis diff with left
	glRotatef(-157.5, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.35, -10.2, 2.5);// z - axis diff with left
	glRotatef(-15, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.15, -10.2, 2.6);// z - axis diff with left
	glRotatef(-50, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.05, -10.2, 2.6);// z - axis diff with left
	glRotatef(-75, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	//---------------------end of first layer side


	glPushMatrix();
	glTranslatef(-0.6, -11.7, 2.75);// z - axis diff with left ----left 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(0.25, 1, 0.8);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -11.7, 3.25);// z - axis diff with left ----right 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glScalef(0.8, 1, 0.25);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-0.6, -11.7, 3);// z - axis diff with left ----left 90 degree triangle base pyramid
	//glRotatef(90, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	//glScalef(0.25, 1, 0.8);
	//glColor3f(0, 0, 0);
	//drawRTriPyramidLine(1);
	//glColor3f(1, 1, 1);
	//drawRTriPyramid(1);
	//glPopMatrix();



	//----------------------------------------------------------------------------------------------------------------------end of botton leg decorations


	// left ankle
	glPushMatrix();
	glTranslatef(0, -12.5, 2.75);
	glColor3f(0.678, 0.561, 0.451);
	drawCylinder(0.5, 0.5, 0.5);
	glPopMatrix();


	//left soles
	glPushMatrix();
	glTranslatef(-1.5, -13.5, 2.5);// z - axis diff with left
	glScalef(1, 0.3, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	//---------------------------------------left soles decorations

	//----------------- white part

	glPushMatrix();
	glTranslatef(-0.5, -12.8, 2.75);// z - axis diff with left---- cube
	//glRotatef(120, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(0.247, 0.247, 0.251);
	drawCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2, -13.8, 2.75);// z - axis diff with left---- cube behind front pyramid
	//glRotatef(120, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -13.8, 2.75);// z - axis diff with left---- cube front of back pyramid
	//glRotatef(120, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -13, 2.5);// z - axis diff with left---- cube front of back pyramid
	//glRotatef(120, 0, 0, 1);
	glScalef(1, 1, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -12.9, 3.25);// z - axis diff with left ----right 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glScalef(0.8, 2, 0.25);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -12.9, 2.75);// z - axis diff with left ----left 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(0.25, 2, 0.8);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.5, -12, 2.75);// z - axis diff with left----front quad base pyramid2
	glRotatef(215, 0, 0, 1);
	glScalef(2, 1, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, -12, 2.75);// z - axis diff with left----front quad base pyramid1
	glRotatef(215, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	/// ---------------red sole outside part


	glPushMatrix();
	glTranslatef(-2.25, -12.9, 2.75);// z - axis diff with left----front quad base pyramid
	glRotatef(215, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -13.8, 3.25);// z - axis diff with left ----right 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glScalef(1, 2, 1);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -13.8, 2.75);// z - axis diff with left ----left 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(1, 2, 1);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -13.8, 4.25);// sole base -front right side
	glRotatef(-130, 1, 0, 0);
	//glRotatef(-90, 0, 1, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -13.8, 4.25);// sole base -back right side
	glRotatef(-130, 1, 0, 0);
	//glRotatef(-90, 0, 1, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -13, 2.5);// sole base - front left side
	glRotatef(130, 1, 0, 0);
	//glRotatef(-90, 0, 1, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -13, 2.5);// sole base - back left side
	glRotatef(130, 1, 0, 0);
	//glRotatef(-90, 0, 1, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, -13, 2.5);// sole base - back side
	glRotatef(-90, 0, 1, 0);
	glRotatef(130, 1, 0, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -13.8, 3);// sole base -fill the hole between right back side
	//glRotatef(-90, 0, 1, 0);
	//glRotatef(130, 1, 0, 0);
	glScalef(1, 0.8, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -13.8, 2);// sole base -fill the hole between left back side
	//glRotatef(-90, 0, 1, 0);
	//glRotatef(130, 1, 0, 0);
	glScalef(1, 0.8, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	//--------------------------------------end of left soles decorations
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();

	///------------------------------------------------ end of left leg





	///------------------------------------------------ right leg

	glPushMatrix(); //----------------------------------------------------- rotate right leg 1 - whole leg
	glRotatef(rrleg1, 0, 0, 1);
	glTranslatef(txwrleg, tywrleg, tzwrleg);

	glPushMatrix();
	glTranslatef(0, 0, -0.75);

	glPushMatrix();
	glTranslatef(0, -0.85, 3);
	glColor3f(0.247, 0.247, 0.251);
	drawSphere(1, 30, 30);
	glPopMatrix();

	// right upper leg

	glPushMatrix();
	glTranslatef(0, -1.2, 3);
	glColor3f(0.247, 0.247, 0.251);
	glRotatef(90, 1, 0, 0);
	drawCylinder(0.3, 0.3, 5);
	glPopMatrix();

	//--------------------------------------------------------------------------------------------------upper leg decorations


	glPushMatrix();
	glTranslatef(-1, -5.2, 2);
	//glRotatef(235, 0, 0, 1);
	glScalef(1, 1.8, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, -5.7, 2.25);
	//glRotatef(235, 0, 0, 1);
	glScalef(0.8, 1.5, 0.8);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.949, 0.949, 0.949);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -5.2, 3.25);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.2, 0.2, 1.3);
	glColor3f(0.949, 0.949, 0.949);
	drawCylinder(0.2, 0.2, 1.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -5.2, 2.75);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.2, 0.2, 1.3);
	glColor3f(0.949, 0.949, 0.949);
	drawCylinder(0.2, 0.2, 1.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -5.2, 3.25);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.2, 0.2, 1.3);
	glColor3f(0.949, 0.949, 0.949);
	drawCylinder(0.2, 0.2, 1.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -5.2, 2.75);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.2, 0.2, 1.3);
	glColor3f(0.949, 0.949, 0.949);
	drawCylinder(0.2, 0.2, 1.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.99999, -5.799999, 3.499999999);
	//glRotatef(235, 0, 0, 1);
	glScalef(0.99, 1.8, 0.25);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.99999, -5.799999, 2);
	//glRotatef(235, 0, 0, 1);
	glScalef(0.99, 1.8, 0.25);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.5, -5.799999, 2);
	//glRotatef(235, 0, 0, 1);
	glScalef(0.25, 1.8, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, -5.4, 1.9);
	//glRotatef(235, 0, 0, 1);
	//glScalef(0.25, 1.8, 1);
	glColor3f(0, 0, 0);
	drawCylinderLine(0.3, 0.3, 2.2);
	glColor3f(0, 0, 0);
	drawCylinder(0.3, 0.3, 2.2);
	glPopMatrix();



	//--------------------------------------------------------------------------------------------------end of upper leg decorations



	// ---------------------------------------------------------right knee
	glPushMatrix();
	glTranslatef(-0.25, -6.8, 3);
	glColor3f(0.247, 0.247, 0.251);
	drawSphere(0.6, 30, 30);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix(); //------------------------------------------------- rotate right leg 2 - below knee
	glTranslatef(txrleg2, tyrleg2, tzrleg2);
	glRotatef(rrleg2, 0, 0, 1);

	glPushMatrix();
	glTranslatef(0, 0, -0.75);

	// right bottom leg
	glPushMatrix();
	glTranslatef(0, -7, 3);
	glColor3f(0.247, 0.247, 0.251);
	glRotatef(90, 1, 0, 0);
	drawCylinder(0.3, 0.3, 5);
	glPopMatrix();

	//-------------------------------------------------------------------------------------------------------botton leg decorations

	//glPushMatrix();
	//glTranslatef(-1, -10, 2.75);// z - axis diff with left----front quad base pyramid1
	//glRotatef(215, 0, 0, 1);
	//glScalef(2, 0.5, 0.5);
	//glColor3f(0, 0, 0);
	//drawPyramidLine(1);
	//glColor3f(1, 1, 1);
	//drawPyramid(1);
	//glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -10.2, 2.75);// z - axis diff with left----front quad base pyramid1
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	/// first layer
	glPushMatrix();
	glTranslatef(-0.6, -10.8, 2.75);// z - axis diff with left----front quad base pyramid1
	glRotatef(215, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//----
	glPushMatrix();
	glTranslatef(-0.6, -10.9, 2.45);// z - axis diff with left-
	glRotatef(215, 0, 0, 1);
	glRotatef(15, 1, 0, 0);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -11.2, 2.25);// z - axis diff with left-
	glRotatef(215, 0, 0, 1);
	glRotatef(45, 1, 0, 0);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	//----

	glPushMatrix();
	glTranslatef(-0.6, -10.8, 3.05);// z - axis diff with left-
	glRotatef(215, 0, 0, 1);
	glRotatef(-15, 1, 0, 0);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -10.8, 3.35);// z - axis diff with left-
	glRotatef(215, 0, 0, 1);
	glRotatef(-45, 1, 0, 0);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.45, -10.2, 3.25);// z - axis diff with left
	glRotatef(180, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.35, -9.8, 2.65);
	//glRotatef(285, 0, 0, 1);
	glScalef(0.7, 2, 0.7);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	//-------------------------- layer front
	//glPushMatrix();
	//glTranslatef(-0.5, -9, 2.75);// z - axis diff with left
	//glRotatef(235, 0, 0, 1);
	//glScalef(1.5, 0.5, 0.5);
	//glColor3f(0, 0, 0);
	//drawPyramidLine(1);
	//glColor3f(1, 1, 1);
	//drawPyramid(1);
	//glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, -9, 2.75);// z - axis diff with left ---2
	glRotatef(285, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.25, -8, 2.75);// z - axis diff with left --3
	glRotatef(290, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.75, -7, 2.75);// z - axis diff with left --4
	glRotatef(295, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.25, -5.5, 2.75);// z - axis diff with left--5
	glRotatef(295, 0, 0, 1);
	glScalef(3, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//--------------------------end of layer front

	//-----------------------fifth layer side

	glPushMatrix();
	glTranslatef(0.6, -6.25, 3.25);// layer 5 back
	glRotatef(180, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, -6.25, 3.5);
	glRotatef(65, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, -6.25, 3.6);
	glRotatef(90, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, -6.25, 3.6);
	glRotatef(135, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -6.25, 2.35);
	glRotatef(-90, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, -6.25, 2.25);
	glRotatef(-65, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, -6.25, 2.7);
	glRotatef(-135, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	//-------------- end of fifth layer

	glPushMatrix();
	glTranslatef(-0.85, -7, 2.75);//layer 4 front 2
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, -7, 3.25);// layer 4 back
	glRotatef(180, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();



	//----------------------fourth layer side

	glPushMatrix();
	glTranslatef(-0.7, -7, 3.25);
	glRotatef(45, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, -7, 3.6);
	glRotatef(90, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, -7, 3.6);
	glRotatef(135, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -7, 2.35);
	glRotatef(-90, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, -7, 2.4);
	glRotatef(-45, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, -7, 2.7);
	glRotatef(-135, 0, 1, 0);
	glRotatef(270, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();




	//----------------------end of fourth layer side


	glPushMatrix();
	glTranslatef(0.75, -8, 3.25);// third layer back
	glRotatef(180, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//----------------------third layer side

	glPushMatrix();
	glTranslatef(-0.75, -8, 3.35);
	glRotatef(45, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, -8, 3.75);
	glRotatef(90, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, -8, 3.65);
	glRotatef(135, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.35, -8, 2.35);
	glRotatef(-45, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.25, -8, 2.25);
	glRotatef(-90, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.65, -8, 2.65);
	glRotatef(-135, 0, 1, 0);
	glRotatef(275, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();



	//----------------------end of third layer side



	//---------------------second layer side
	glPushMatrix();
	glTranslatef(0.75, -9, 3.25);// second layer back
	glRotatef(180, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, -9, 3.35);// right
	glRotatef(45, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, -9, 3.75);// right
	glRotatef(90, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, -9, 3.7);// right
	glRotatef(135, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.35, -9, 2.35);// left
	glRotatef(-25, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -9, 2.25);// left
	glRotatef(-90, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.75, -9, 2.65);// left
	glRotatef(-135, 0, 1, 0);
	glRotatef(280, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//----------------------end of second layer size

	//------------------------ first layer side
	glPushMatrix();//-0.5
	glTranslatef(0, -10.2, 3.25);// z - axis diff with left
	glRotatef(90, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0, -10.2, 3.25);// z - axis diff with left
	glRotatef(112.5, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.15, -10.2, 3.25);// z - axis diff with left
	glRotatef(135, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.15, -10.2, 3.25);// z - axis diff with left
	glRotatef(157.5, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.5, -10.2, 3);// z - axis diff with left
	glRotatef(15, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.5, -10.2, 3);// z - axis diff with left
	glRotatef(50, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.35, -10.2, 3.15);// z - axis diff with left
	glRotatef(75, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	//------------------------
	glPushMatrix();//-0.5
	glTranslatef(0.25, -10.2, 2.75);// z - axis diff with left
	glRotatef(-90, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.45, -10.2, 2.75);// z - axis diff with left
	glRotatef(-112.5, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(0.4, -10.2, 3);// z - axis diff with left
	glRotatef(-135, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	glPushMatrix();//-0.5
	glTranslatef(0.4, -10.2, 3.1);// z - axis diff with left
	glRotatef(-157.5, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.35, -10.2, 2.5);// z - axis diff with left
	glRotatef(-15, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.15, -10.2, 2.6);// z - axis diff with left
	glRotatef(-50, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();//-0.5
	glTranslatef(-0.05, -10.2, 2.6);// z - axis diff with left
	glRotatef(-75, 0, 1, 0);
	glRotatef(235, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();


	//---------------------end of first layer side


	glPushMatrix();
	glTranslatef(-0.6, -11.7, 2.75);// z - axis diff with left ----left 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(0.25, 1, 0.8);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -11.7, 3.25);// z - axis diff with left ----right 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glScalef(0.8, 1, 0.25);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-0.6, -11.7, 3);// z - axis diff with left ----left 90 degree triangle base pyramid
	//glRotatef(90, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	//glScalef(0.25, 1, 0.8);
	//glColor3f(0, 0, 0);
	//drawRTriPyramidLine(1);
	//glColor3f(1, 1, 1);
	//drawRTriPyramid(1);
	//glPopMatrix();



	//----------------------------------------------------------------------------------------------------------------------end of botton leg decorations


	// right ankle
	glPushMatrix();
	glTranslatef(0, -12.5, 2.75);
	glColor3f(0.678, 0.561, 0.451);
	drawCylinder(0.5, 0.5, 0.5);
	glPopMatrix();


	//right soles
	glPushMatrix();
	glTranslatef(-1.5, -13.5, 2.5);// z - axis diff with left
	glScalef(1, 0.3, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	//---------------------------------------right soles decorations

	//----------------- white part

	glPushMatrix();
	glTranslatef(-0.5, -12.8, 2.75);// z - axis diff with left---- cube
	//glRotatef(120, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(0.247, 0.247, 0.251);
	drawCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2, -13.8, 2.75);// z - axis diff with left---- cube behind front pyramid
	//glRotatef(120, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -13.8, 2.75);// z - axis diff with left---- cube front of back pyramid
	//glRotatef(120, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -13, 2.5);// z - axis diff with left---- cube front of back pyramid
	//glRotatef(120, 0, 0, 1);
	glScalef(1, 1, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -12.9, 3.25);// z - axis diff with left ----right 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glScalef(0.8, 2, 0.25);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -12.9, 2.75);// z - axis diff with left ----left 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(0.25, 2, 0.8);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.5, -12, 2.75);// z - axis diff with left----front quad base pyramid2
	glRotatef(215, 0, 0, 1);
	glScalef(2, 1, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, -12, 2.75);// z - axis diff with left----front quad base pyramid1
	glRotatef(215, 0, 0, 1);
	glScalef(2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(1, 1, 1);
	drawPyramid(1);
	glPopMatrix();

	/// ---------------red sole outside part


	glPushMatrix();
	glTranslatef(-2.25, -12.9, 2.75);// z - axis diff with left----front quad base pyramid
	glRotatef(215, 0, 0, 1);
	glScalef(1.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -13.8, 3.25);// z - axis diff with left ----right 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glScalef(1, 2, 1);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -13.8, 2.75);// z - axis diff with left ----left 90 degree triangle base pyramid
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(1, 2, 1);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -13.8, 4.25);// sole base -front right side
	glRotatef(-130, 1, 0, 0);
	//glRotatef(-90, 0, 1, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -13.8, 4.25);// sole base -back right side
	glRotatef(-130, 1, 0, 0);
	//glRotatef(-90, 0, 1, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -13, 2.5);// sole base - front left side
	glRotatef(130, 1, 0, 0);
	//glRotatef(-90, 0, 1, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -13, 2.5);// sole base - back left side
	glRotatef(130, 1, 0, 0);
	//glRotatef(-90, 0, 1, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, -13, 2.5);// sole base - back side
	glRotatef(-90, 0, 1, 0);
	glRotatef(130, 1, 0, 0);
	glScalef(1, 0.5, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -13.8, 3);// sole base -fill the hole between right back side
	//glRotatef(-90, 0, 1, 0);
	//glRotatef(130, 1, 0, 0);
	glScalef(1, 0.8, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -13.8, 2);// sole base -fill the hole between left back side
	//glRotatef(-90, 0, 1, 0);
	//glRotatef(130, 1, 0, 0);
	glScalef(1, 0.8, 1);
	glColor3f(0, 0, 0);
	drawPyramidLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawPyramid(1);
	glPopMatrix();

	//--------------------------------------end of right soles decorations
	glPopMatrix();


	glPopMatrix();

	glPopMatrix();
	///------------------------------------------------ end of right leg

}

void body() {
	/// ---------------------------------------------- middle part -> to simulates body purposes

	glPushMatrix();
	glTranslatef(0, 6, 0);
	glColor3f(0.247, 0.247, 0.251);
	glRotatef(90, 1, 0, 0);
	drawCylinder(0.3, 0.3, 7);
	glPopMatrix();


	// ------------------------------------------------ 

	glPushMatrix();
	glTranslatef(0, -0.5, -2.5);
	glColor3f(0.247, 0.247, 0.251);
	drawCylinder(0.3, 0.3, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, -0, -3);
	//glRotatef(45, 0, 0, 1);
	glScalef(1, 0.5, 3);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, -2, -0.5);
	glRotatef(45, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.6, 0.35, -0.5);
	//glRotatef(45, 0, 0, 1);
	glScalef(0.8, 0.6, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7, -0.5, 0.5); //---- right red
	glRotatef(-10, 1, 0, 0);
	glRotatef(-20, 0, 0, 1);
	glScalef(0.5, 0.5, 1.3);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7, -0, -3); //---- left red
	glRotatef(10, 1, 0, 0);
	glRotatef(-20, 0, 0, 1);
	glScalef(0.5, 0.5, 1.3);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	//--------------------------------------------------------------------------------------------------------------------------

	glPushMatrix();
	glTranslatef(-2.2, -2.2, 0.8); //---- right white
	glRotatef(-10, 1, 0, 0);
	glRotatef(-20, 0, 0, 1);
	glScalef(0.3, 1, 1.3);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	// Face 1 : BOTTOM
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 2);//1
	glTexCoord2f(1, 1);
	glVertex3f(2, 0.0f, 2);//2
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 0.0f);//3
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);//4
	glEnd();
	// Face 2 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);//4 last point of face one
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 2);
	glEnd();
	// Face 3 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 2);
	glEnd();
	// Face 4 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(2, 0.0f, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(2, 0.0f, 0.0f);
	glEnd();
	// Face 5 : 
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 0.0f);
	glEnd();
	// Face 6 : 
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2.2, -1.6, -3.3); //---- right white
	glRotatef(10, 1, 0, 0);
	glRotatef(-20, 0, 0, 1);
	glScalef(0.3, 1, 1.3);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	// Face 1 : BOTTOM
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 2);//1
	glTexCoord2f(1, 1);
	glVertex3f(2, 0.0f, 2);//2
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 0.0f);//3
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);//4
	glEnd();
	// Face 2 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);//4 last point of face one
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 2);
	glEnd();
	// Face 3 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 2);
	glEnd();
	// Face 4 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(2, 0.0f, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(2, 0.0f, 0.0f);
	glEnd();
	// Face 5 : 
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 0.0f);
	glEnd();
	// Face 6 : 
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1.7, -1.8, -3.3); // left tri
	glRotatef(70, 0, 0, 1);
	glRotatef(260, 0, 1, 0);
	glRotatef(-5, 0, 0, 1);
	glScalef(1.3, 0.3, 1);
	glColor3f(0, 0, 0);
	drawTriangularPrismLine(2);
	glColor3f(1, 1, 1);
	height = 2;

	// Drawing the first triangle (base)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for first vertex
	glVertex3f(0.0f, 0.0f, 0.0f);                 // First vertex of base triangle

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for second vertex
	glVertex3f(2, 0.0f, 0.0f);                 // Second vertex of base triangle

	glTexCoord2f(0.5f, 1.0f); // Texture coordinate for third vertex
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Third vertex (equilateral triangle)
	glEnd();

	// Drawing the second triangle (top)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for first vertex
	glVertex3f(0.0f, height, 0.0f);                 // First vertex of top triangle

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for second vertex
	glVertex3f(2, height, 0.0f);                 // Second vertex of top triangle

	glTexCoord2f(0.5f, 1.0f); // Texture coordinate for third vertex
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Third vertex (equilateral triangle)
	glEnd();

	// Drawing the connecting rectangles
	glBegin(GL_QUADS);

	// Connect vertex1 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 1
	glVertex3f(0.0f, 0.0f, 0.0f);                  // Base triangle vertex 1

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 2
	glVertex3f(2, 0.0f, 0.0f);                  // Base triangle vertex 2

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 2
	glVertex3f(2, height, 0.0f);                // Top triangle vertex 2

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 1
	glVertex3f(0.0f, height, 0.0f);                // Top triangle vertex 1
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex2 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 2
	glVertex3f(2, 0.0f, 0.0f);                  // Base triangle vertex 2

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 3
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Base triangle vertex 3

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 3
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Top triangle vertex 3

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 2
	glVertex3f(2, height, 0.0f);                // Top triangle vertex 2
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex3 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 3
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Base triangle vertex 3

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 1
	glVertex3f(0.0f, 0.0f, 0.0f);                  // Base triangle vertex 1

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 1
	glVertex3f(0.0f, height, 0.0f);                // Top triangle vertex 1

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 3
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Top triangle vertex 3
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-1.65, -1.8, -3.4); // right tri
	glRotatef(70, 0, 0, 1);
	glRotatef(260, 0, 1, 0);
	glRotatef(-5, 0, 0, 1);
	glScalef(1.3, 0.3, 1);
	glColor3f(0, 0, 0);
	drawTriangularPrismLine(2);
	glColor3f(1, 1, 1);
	height = 2;

	// Drawing the first triangle (base)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for first vertex
	glVertex3f(0.0f, 0.0f, 0.0f);                 // First vertex of base triangle

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for second vertex
	glVertex3f(2, 0.0f, 0.0f);                 // Second vertex of base triangle

	glTexCoord2f(0.5f, 1.0f); // Texture coordinate for third vertex
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Third vertex (equilateral triangle)
	glEnd();

	// Drawing the second triangle (top)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for first vertex
	glVertex3f(0.0f, height, 0.0f);                 // First vertex of top triangle

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for second vertex
	glVertex3f(2, height, 0.0f);                 // Second vertex of top triangle

	glTexCoord2f(0.5f, 1.0f); // Texture coordinate for third vertex
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Third vertex (equilateral triangle)
	glEnd();

	// Drawing the connecting rectangles
	glBegin(GL_QUADS);

	// Connect vertex1 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 1
	glVertex3f(0.0f, 0.0f, 0.0f);                  // Base triangle vertex 1

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 2
	glVertex3f(2, 0.0f, 0.0f);                  // Base triangle vertex 2

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 2
	glVertex3f(2, height, 0.0f);                // Top triangle vertex 2

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 1
	glVertex3f(0.0f, height, 0.0f);                // Top triangle vertex 1
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex2 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 2
	glVertex3f(2, 0.0f, 0.0f);                  // Base triangle vertex 2

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 3
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Base triangle vertex 3

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 3
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Top triangle vertex 3

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 2
	glVertex3f(2, height, 0.0f);                // Top triangle vertex 2
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex3 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 3
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Base triangle vertex 3

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 1
	glVertex3f(0.0f, 0.0f, 0.0f);                  // Base triangle vertex 1

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 1
	glVertex3f(0.0f, height, 0.0f);                // Top triangle vertex 1

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 3
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Top triangle vertex 3
	glEnd();
	glPopMatrix();
	
	/// ----------------------bottom left side

	glPushMatrix();
	glRotatef(10, 1, 0, 0);
	glTranslatef(2.85, 1.8, -0.1);


	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-3.65, -1.8, -3.4);
	glRotatef(-90, 0, 0, 1);
	//glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(1, 0.3, 0.3);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(2);
	glColor3f(1, 1, 1);
	drawRTriPyramid(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-2.0, -1.8, -3.4);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(1, 0.3, 0.3);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(2);
	glColor3f(1, 1, 1);
	drawRTriPyramid(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-2.0, -1.8, -3.4);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(0.2, 0.5, 1.7);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-2.0, -1.8, -3.4);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(2, 0.2, 1.7);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-2.3, -1.5, -3.4);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(0.3, 0.3, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPopMatrix();
	//---------------------------------

	/// ----------------------bottom right side

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(10, 1, 0, 0);
	glTranslatef(2.85, 1.8, -0.1);


	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-3.65, -1.8, -3.4);
	glRotatef(-90, 0, 0, 1);
	//glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(1, 0.3, 0.3);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(2);
	glColor3f(1, 1, 1);
	drawRTriPyramid(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-2.0, -1.8, -3.4);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(1, 0.3, 0.3);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(2);
	glColor3f(1, 1, 1);
	drawRTriPyramid(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-2.0, -1.8, -3.4);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(0.2, 0.5, 1.7);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-2.0, -1.8, -3.4);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(2, 0.2, 1.7);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-2.3, -1.5, -3.4);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(0.3, 0.3, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPopMatrix();
	//---------------------------------
	//----------back part

	glPushMatrix();
	//glScalef(-1, 1, 1);
	glTranslatef(-0, -1.5, -0.5);
	glRotatef(10, 0, 0, 1);
	glScalef(1, 1, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	//glScalef(-1, 1, 1);
	glTranslatef(1.3, -1.8, -0.5);
	glRotatef(45, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();



	glPushMatrix();
	//glScalef(-1, 1, 1);
	glTranslatef(-1, -2.3, -0.5);
	glRotatef(10, 0, 0, 1);
	glScalef(1.2, 0.5, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	//back left part

	//--------------------------------------------------------------------------------------------------------------------------

	glPushMatrix();
	glTranslatef(1.4, -1.9, 0.8); //---- left white
	glRotatef(-5, 1, 0, 0);
	glRotatef(10, 0, 0, 1);
	glScalef(0.3, 1.3, 1.2);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	// Face 1 : BOTTOM
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 2);//1
	glTexCoord2f(1, 1);
	glVertex3f(2, 0.0f, 2);//2
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 0.0f);//3
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);//4
	glEnd();
	// Face 2 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);//4 last point of face one
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 2);
	glEnd();
	// Face 3 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 2);
	glEnd();
	// Face 4 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(2, 0.0f, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(2, 0.0f, 0.0f);
	glEnd();
	// Face 5 : 
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 0.0f);
	glEnd();
	// Face 6 : 
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(1.9, -1.6, -3.2); // left tri
	glRotatef(100, 0, 0, 1);
	glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1.2, 0.3, 1);
	glColor3f(0, 0, 0);
	drawTriangularPrismLine(2);
	glColor3f(1, 1, 1);
	height = 2;

	// Drawing the first triangle (base)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for first vertex
	glVertex3f(0.0f, 0.0f, 0.0f);                 // First vertex of base triangle

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for second vertex
	glVertex3f(2, 0.0f, 0.0f);                 // Second vertex of base triangle

	glTexCoord2f(0.5f, 1.0f); // Texture coordinate for third vertex
	glVertex3f(2 / 2, 0.0f, sqrt(3)* 2 / 2); // Third vertex (equilateral triangle)
	glEnd();

	// Drawing the second triangle (top)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for first vertex
	glVertex3f(0.0f, height, 0.0f);                 // First vertex of top triangle

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for second vertex
	glVertex3f(2, height, 0.0f);                 // Second vertex of top triangle

	glTexCoord2f(0.5f, 1.0f); // Texture coordinate for third vertex
	glVertex3f(2 / 2, height, sqrt(3)* 2 / 2); // Third vertex (equilateral triangle)
	glEnd();

	// Drawing the connecting rectangles
	glBegin(GL_QUADS);

	// Connect vertex1 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 1
	glVertex3f(0.0f, 0.0f, 0.0f);                  // Base triangle vertex 1

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 2
	glVertex3f(2, 0.0f, 0.0f);                  // Base triangle vertex 2

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 2
	glVertex3f(2, height, 0.0f);                // Top triangle vertex 2

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 1
	glVertex3f(0.0f, height, 0.0f);                // Top triangle vertex 1
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex2 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 2
	glVertex3f(2, 0.0f, 0.0f);                  // Base triangle vertex 2

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 3
	glVertex3f(2 / 2, 0.0f, sqrt(3)* 2 / 2); // Base triangle vertex 3

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 3
	glVertex3f(2 / 2, height, sqrt(3)* 2 / 2); // Top triangle vertex 3

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 2
	glVertex3f(2, height, 0.0f);                // Top triangle vertex 2
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex3 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 3
	glVertex3f(2 / 2, 0.0f, sqrt(3)* 2 / 2); // Base triangle vertex 3

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 1
	glVertex3f(0.0f, 0.0f, 0.0f);                  // Base triangle vertex 1

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 1
	glVertex3f(0.0f, height, 0.0f);                // Top triangle vertex 1

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 3
	glVertex3f(2 / 2, height, sqrt(3)* 2 / 2); // Top triangle vertex 3
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(1.9, -1.6, -3.2);
	glRotatef(100, 0, 0, 1);
	glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1.2, 0.3, 1);
	glColor3f(0, 0, 0);
	drawTriangularPrismLine(2);
	glColor3f(1, 1, 1);
	height = 2;

	// Drawing the first triangle (base)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for first vertex
	glVertex3f(0.0f, 0.0f, 0.0f);                 // First vertex of base triangle

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for second vertex
	glVertex3f(2, 0.0f, 0.0f);                 // Second vertex of base triangle

	glTexCoord2f(0.5f, 1.0f); // Texture coordinate for third vertex
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Third vertex (equilateral triangle)
	glEnd();

	// Drawing the second triangle (top)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for first vertex
	glVertex3f(0.0f, height, 0.0f);                 // First vertex of top triangle

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for second vertex
	glVertex3f(2, height, 0.0f);                 // Second vertex of top triangle

	glTexCoord2f(0.5f, 1.0f); // Texture coordinate for third vertex
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Third vertex (equilateral triangle)
	glEnd();

	// Drawing the connecting rectangles
	glBegin(GL_QUADS);

	// Connect vertex1 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 1
	glVertex3f(0.0f, 0.0f, 0.0f);                  // Base triangle vertex 1

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 2
	glVertex3f(2, 0.0f, 0.0f);                  // Base triangle vertex 2

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 2
	glVertex3f(2, height, 0.0f);                // Top triangle vertex 2

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 1
	glVertex3f(0.0f, height, 0.0f);                // Top triangle vertex 1
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex2 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 2
	glVertex3f(2, 0.0f, 0.0f);                  // Base triangle vertex 2

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 3
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Base triangle vertex 3

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 3
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Top triangle vertex 3

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 2
	glVertex3f(2, height, 0.0f);                // Top triangle vertex 2
	glEnd();

	glBegin(GL_QUADS);
	// Connect vertex3 of both triangles
	glTexCoord2f(0.0f, 0.0f); // Texture coordinate for base vertex 3
	glVertex3f(2 / 2, 0.0f, sqrt(3) * 2 / 2); // Base triangle vertex 3

	glTexCoord2f(1.0f, 0.0f); // Texture coordinate for base vertex 1
	glVertex3f(0.0f, 0.0f, 0.0f);                  // Base triangle vertex 1

	glTexCoord2f(1.0f, 1.0f); // Texture coordinate for top vertex 1
	glVertex3f(0.0f, height, 0.0f);                // Top triangle vertex 1

	glTexCoord2f(0.0f, 1.0f); // Texture coordinate for top vertex 3
	glVertex3f(2 / 2, height, sqrt(3) * 2 / 2); // Top triangle vertex 3
	glEnd();
	glPopMatrix();

	//--------------------------------------------------------------------------------------------------------------------------


	glPushMatrix();
	glTranslatef(-1.8, 2.3, -0.8);

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(2.9, -1.6, -3.2);
	glRotatef(45, 0, 0, 1);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1.3, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(3, -1.6, -3.2);
	glRotatef(315, 0, 0, 1);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1.1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(315, 0, 0, 1);
	glTranslatef(3.2, 1.9, -3.2);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1.1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(315, 0, 0, 1);
	glTranslatef(3.2, 2.2, -3.2);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1.1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(315, 0, 0, 1);
	glTranslatef(3.2, 2.2, -2.2);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1.1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(315, 0, 0, 1);
	glTranslatef(3.2, 2, -3);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(0.8, 0.8, 0.8);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(0, 0, 0);
	drawCube(1);
	glPopMatrix();

	glPopMatrix();

	//------------------------end of back right part

	//back right part
	glPushMatrix();
	glScalef(1, 1, -1);

	//---------------------------------------------------------------------------------------------------------------
	glPushMatrix();
	glTranslatef(1.4, -1.9, 0.8);
	glRotatef(-5, 1, 0, 0);
	glRotatef(10, 0, 0, 1);
	glScalef(0.3, 1.3, 1.2);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	// Face 1 : BOTTOM
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 2);//1
	glTexCoord2f(1, 1);
	glVertex3f(2, 0.0f, 2);//2
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 0.0f);//3
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);//4
	glEnd();
	// Face 2 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);//4 last point of face one
	glTexCoord2f(1, 0);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 0.0f, 2);
	glEnd();
	// Face 3 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 2);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 2);
	glEnd();
	// Face 4 : 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(2, 0.0f, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(2, 0.0f, 0.0f);
	glEnd();
	// Face 5 : 
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 0.0f);
	glEnd();
	// Face 6 : 
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 2, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 2, 2);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 2);
	glEnd();
	glPopMatrix();
	//---------------------------------------------------------------------------------------------------------------
	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(1.9, -1.6, -3.2);
	glRotatef(100, 0, 0, 1);
	glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1.2, 0.3, 1);
	glColor3f(0, 0, 0);
	drawTriangularPrismLine(2);
	glColor3f(1, 1, 1);
	drawTriangularPrism(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(1.9, -1.6, -3.2);
	glRotatef(100, 0, 0, 1);
	glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1.2, 0.3, 1);
	glColor3f(0, 0, 0);
	drawTriangularPrismLine(2);
	glColor3f(1, 1, 1);
	drawTriangularPrism(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.8, 2.3, -0.8);

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(2.9, -1.6, -3.2);
	glRotatef(45, 0, 0, 1);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1.3, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(3, -1.6, -3.2);
	glRotatef(315, 0, 0, 1);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1.1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(315, 0, 0, 1);
	glTranslatef(3.2, 1.9, -3.2);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glScalef(1, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1.1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(315, 0, 0, 1);
	glTranslatef(3.2, 2.2, -3.2);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1.1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(315, 0, 0, 1);
	glTranslatef(3.2, 2.2, -2.2);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.3, 1.1);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(1, 1, 1.1);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glRotatef(315, 0, 0, 1);
	glTranslatef(3.2, 2, -3);
	//glRotatef(265, 0, 1, 0);
	//glRotatef(15, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(0.8, 0.8, 0.8);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(0, 0, 0);
	drawCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
	//------------------------end of back right

	//------------------------------middle first base white


	glPushMatrix();
	glTranslatef(-0.8, 3.3, -1.85);
	//glRotatef(5, 1, 0, 0);
	glScalef(1, 1, 1.8);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 1, -2.2);
	glRotatef(5, 1, 0, 0);
	glScalef(0.7, 1.3, 1.1);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-0.6, 1, -2.2);
	glRotatef(5, 1, 0, 0);
	glScalef(0.7, 1.3, 1.1);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 1, -0.5);
	//glRotatef(5, 1, 0, 0);
	glScalef(0.3, 1.3, 0.5);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.9, 0.5, -0.7);
	//glRotatef(5, 1, 0, 0);
	glScalef(0.3, 0.5, 0.7);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 1);
	glTranslatef(-0.75, 1, -2.2);
	glRotatef(5, 1, 0, 0);
	//glRotatef(-90, 0, 0, 1);
	//glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(0.7, 0.7, 0.7);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-0.75, 1, -2.2);
	glRotatef(5, 1, 0, 0);
	//glRotatef(-90, 0, 0, 1);
	//glRotatef(90, 1, 0, 0);
	//Rotatef(-5, 0, 0, 1);
	glScalef(0.7, 0.7, 0.7);
	glColor3f(0, 0, 0);
	drawRTriPyramidLine(1);
	glColor3f(1, 1, 1);
	drawRTriPyramid(1);
	glPopMatrix();
	//--------------------

	//----first black color round

	glPushMatrix();
	glTranslatef(0.8, 1.8, -2.2);
	//glRotatef(5, 1, 0, 0);
	glScalef(0.1, 0.3, 2.2);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();//----side
	glScalef(1, 1, -1);

	glPushMatrix();
	glTranslatef(-0.8, 1.7, -2.1);
	glRotatef(90, 0, 1, 0);
	glScalef(0.1, 0.35, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8, 1.6, -1.8);
	glRotatef(90, 0, 1, 0);
	glScalef(0.2, 0.2, 0.2);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 1.6, -1.6);
	glRotatef(5, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	glScalef(0.1, 0.25, 0.3);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.65, 1.5, -0.5);
	glRotatef(5, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	glScalef(0.1, 0.25, 0.6);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, 1.6, -0.5);
	glRotatef(5, 0, 1, 0);
	glRotatef(180, 0, 1, 0);
	glScalef(0.2, 0.22, 0.1);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();

	glPopMatrix();


	glPushMatrix();//----side

	glPushMatrix();
	glTranslatef(-0.8, 1.7, -2.1);
	glRotatef(90, 0, 1, 0);
	glScalef(0.1, 0.35, 1);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8, 1.6, -1.8);
	glRotatef(90, 0, 1, 0);
	glScalef(0.2, 0.2, 0.2);
	glColor3f(0, 0, 0);
	drawCubeLine(1);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 1.6, -1.6);
	glRotatef(5, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	glScalef(0.1, 0.25, 0.3);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.65, 1.5, -0.5);
	glRotatef(5, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	glScalef(0.1, 0.25, 0.6);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, 1.6, -0.5);
	glRotatef(5, 0, 1, 0);
	glRotatef(180, 0, 1, 0);
	glScalef(0.2, 0.22, 0.1);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.322, 0.322, 0.322);
	drawCube(2);
	glPopMatrix();

	glPopMatrix();

	//    ----- second layer white

	glPushMatrix();
	glTranslatef(0.82, 2.5, -2.6);
	glRotatef(40, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.5, 0.22, 2.6);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.82, 5.3, -2.6);
	glRotatef(-50, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.4, 0.22, 2.6);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.9, 4.7, -2.6);
	glRotatef(50, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.4, 0.22, 2.6);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.6, 3.2, -2.6);
	glRotatef(90, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.9, 0.22, 2.6);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, 5.2, -2.6);
	//glRotatef(90, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.9, 0.22, 2.6);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.82, 2.3, -1.2);
	glRotatef(5, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(0.7, 0.22, 0.8);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-0.82, 2.3, -1.2);
	glRotatef(5, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(0.7, 0.22, 0.8);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.42, 2.8, -1.2);
	glRotatef(5, 1, 0, 0);
	glRotatef(-40, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(0.7, 0.22, 0.4);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-1.42, 2.8, -1.2);
	glRotatef(5, 1, 0, 0);
	glRotatef(-40, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glScalef(0.7, 0.22, 0.4);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.9, 2.9, -2.6);
	glRotatef(90, 0, 0, 1);
	glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(1, 0.22, 2.6);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 1, 1);
	drawCube(2);
	glPopMatrix();


	//----------- front inside red

	glPushMatrix();
	glTranslatef(-1.9, 3.2, -2.58);
	glRotatef(-25, 0, 0, 1);
	//glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.3, 0.22, 2.58);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7, 4.4, -2.58);
	glRotatef(35, 0, 0, 1);
	//glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.6, 0.22, 2.58);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1.8, 4.6, -2.58);
	glRotatef(90, 1, 0, 0);
	//glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.4, 0.22, 0.58);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-1.8, 4.6, -2.58);
	glRotatef(90, 1, 0, 0);
	//glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.4, 0.22, 0.58);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	//grey piece in inside red
	glPushMatrix();
	glTranslatef(-1.7, 4, -2.58);
	glRotatef(15, 0, 0, 1);
	//glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.4, 0.02, 2.58);//2.2
	glColor3f(0, 0, 0);
	drawPyramidLine(2);
	glColor3f(0.58, 0.58, 0.58);
	drawPyramid(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7, 4.2, -2.58);
	glRotatef(15, 0, 0, 1);
	//glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.4, 0.02, 2.58);//2.2
	glColor3f(0, 0, 0);
	drawPyramidLine(2);
	glColor3f(0.58, 0.58, 0.58);
	drawPyramid(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7, 3.8, -2.58);
	glRotatef(15, 0, 0, 1);
	//glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.4, 0.02, 2.58);//2.2
	glColor3f(0, 0, 0);
	drawPyramidLine(2);
	glColor3f(0.58, 0.58, 0.58);
	drawPyramid(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.7, 3.6, -2.58);
	glRotatef(15, 0, 0, 1);
	//glRotatef(-2, 0, 0, 1);
	//glRotatef(90, 0, 1, 0);
	glScalef(0.4, 0.02, 2.58);//2.2
	glColor3f(0, 0, 0);
	drawPyramidLine(2);
	glColor3f(0.58, 0.58, 0.58);
	drawPyramid(2);
	glPopMatrix();

	//------------------outside red

	glPushMatrix();
	glTranslatef(-1.2, 1.1, -0.5);
	//glRotatef(5, 1, 0, 0);
	glScalef(0.1, 0.9, 0.5);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.15, 2.4, -0.49);
	glRotatef(45, 0, 0, 1);
	glScalef(0.1, 0.6, 0.5);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.15, 1.1, -0.49);
	glRotatef(90, 0, 0, 1);
	glScalef(0.1, 0.2, 0.5);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.1, -0.35);
	glRotatef(90, 0, 1, 0);
	glScalef(0.1, 0.8, 0.2);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-1.5, 1.1, -0.35);
	glRotatef(90, 0, 1, 0);
	glScalef(0.1, 0.8, 0.2);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 2.3, -0.34);
	glRotatef(90, 0, 1, 0);
	glRotatef(-45, 1, 0, 0);
	glScalef(0.1, 0.6, 0.2);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-1.5, 2.3, -0.34);
	glRotatef(90, 0, 1, 0);
	glRotatef(-45, 1, 0, 0);
	glScalef(0.1, 0.6, 0.2);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	//--bottom red addition

	glPushMatrix();
	glTranslatef(-2.9, -0.4, -0.34);
	glRotatef(90, 0, 1, 0);
	glRotatef(45, 1, 0, 0);
	glScalef(0.1, 0.9, 0.1);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(-2.9, -0.4, -0.34);
	glRotatef(90, 0, 1, 0);
	glRotatef(45, 1, 0, 0);
	glScalef(0.1, 0.9, 0.1);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.9, -0.4, -0.35);
	glRotatef(90, 1, 0, 0);
	glRotatef(45, 0, 1, 0);
	glScalef(0.1, 0.4, 0.1);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.8, 0.9, -0.35);
	glRotatef(90, 1, 0, 0);
	//glRotatef(45, 0, 1, 0);
	glScalef(0.1, 0.4, 0.1);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.1, 0.4, -0.2);
	glRotatef(90, 1, 0, 0);
	glRotatef(45, 0, 1, 0);
	glScalef(0.1, 0.2, 0.05);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(1, 0.831, 0.294);
	drawCube(2);
	glPopMatrix();


	//---


	glPushMatrix();
	glTranslatef(-2.3, 3.2, -0.5);
	glRotatef(5, 0, 0, 1);
	glScalef(0.4, 0.8, 0.5);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.5, 4.8, -0.5);
	glRotatef(-50, 0, 0, 1);
	glScalef(0.2, 0.8, 0.5);//2.2
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2, 5.85, -0.5);
	glRotatef(-65, 0, 0, 1);
	glScalef(0.2, 0.3, 0.5);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.5, 5.6, -1.3);
	//glRotatef(-65, 0, 0, 1);
	glScalef(0.6, 0.2, 0.2);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, 5.6, -0.5);
	glRotatef(65, 0, 1, 0);
	glScalef(0.3, 0.2, 0.2);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 5.6, -1.3);
	//glRotatef(-65, 0, 0, 1);
	glScalef(0.6, 0.2, 0.2);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, -1);

	glPushMatrix();
	glTranslatef(-0.8, 5.6, -0.5);
	glRotatef(65, 0, 1, 0);
	glScalef(0.3, 0.2, 0.2);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 5.6, -1.3);
	//glRotatef(-65, 0, 0, 1);
	glScalef(0.6, 0.2, 0.2);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.75, 5.55, -1.3);
	glRotatef(15, 0, 0, 1);
	glScalef(0.15, 0.2, 1.3);
	glColor3f(0, 0, 0);
	drawCubeLine(2);
	glColor3f(0.663, 0.188, 0.216);
	drawCube(2);
	glPopMatrix();

	//--------------------------------------------------- end of  middle part
}

void drawFinger(float FR, int LR) {
	glPushMatrix();////////////////////////
	if (LR == 1) {
		glTranslatef(-0.2, -0.5, 0.0);
		glRotatef(-45, 0.0, 0.0, 1.0);
	}
	if (LR == 2) {
		glTranslatef(0.75, 0.0, 0.0);
		glRotatef(45, 0.0, 0.0, 1.0);
	}
	glPushMatrix();
	glRotatef(FR, 1.0, 0.0, 0.0);
	/////////////////////////////////////////upper face
	glColor3f(lightblack);
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

	glPushMatrix();
	glTranslatef(0.0, -0.25, 0.0);
	glRotatef(FR, 1.0, 0.0, 0.0);
	/////////////////////////////////////////upper face
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

	glPushMatrix();
	glTranslatef(0.0, -0.25, 0.0);
	glRotatef(FR, 1.0, 0.0, 0.0);
	/////////////////////////////////////////upper face
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
	glPopMatrix();






	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	glRotatef(FR, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////draw finger 1-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(lightblack);
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
	glRotatef(FR, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////draw finger 1-2
	glPushMatrix();
	/////////////////////////////////////////upper face
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
	glRotatef(FR, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////draw finger 1-3
	glPushMatrix();
	/////////////////////////////////////////upper face
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
	glPopMatrix();
}

void drawFingerLine(float FR, int LR) {
	glPushMatrix();////////////////////////
	if (LR == 1) {
		glTranslatef(-0.2, -0.5, 0.0);
		glRotatef(-45, 0.0, 0.0, 1.0);
	}
	if (LR == 2) {
		glTranslatef(0.75, 0.0, 0.0);
		glRotatef(45, 0.0, 0.0, 1.0);
	}
	glPushMatrix();
	glRotatef(FR, 1.0, 0.0, 0.0);
	/////////////////////////////////////////upper face
	glColor3f(lightblack);
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

	glPushMatrix();
	glTranslatef(0.0, -0.25, 0.0);
	glRotatef(FR, 1.0, 0.0, 0.0);
	/////////////////////////////////////////upper face
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

	glPushMatrix();
	glTranslatef(0.0, -0.25, 0.0);
	glRotatef(FR, 1.0, 0.0, 0.0);
	/////////////////////////////////////////upper face
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
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	glRotatef(FR, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////draw finger 1-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(black);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 2-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 3-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.0, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 4-1
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.25, 0.0);
	glRotatef(FR, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////draw finger 1-2
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 2-2
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.0, -0.25, 0.0);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.0, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 3-2
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.0, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 4-2
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.25, 0.0);
	glRotatef(FR, 1.0, 0.0, 0.0);
	//////////////////////////////////////////////////////////////draw finger 1-3
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.5, -0.0, 0.0);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.5, -0.25, 0.25);
	glVertex3f(-0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 2-3
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.25, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.25, 0.0);
	glVertex3f(-0.0, -0.25, 0.0);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.25, 0.0);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.25, -0.0, 0.0);
	glVertex3f(-0.25, -0.0, 0.25);
	glVertex3f(-0.25, -0.25, 0.25);
	glVertex3f(-0.25, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(-0.0, -0.0, 0.0);
	glVertex3f(-0.0, -0.0, 0.25);
	glVertex3f(-0.0, -0.25, 0.25);
	glVertex3f(-0.0, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 3-3
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.0, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -0.0, 0.0);
	glVertex3f(0.0, -0.0, 0.25);
	glVertex3f(0.0, -0.25, 0.25);
	glVertex3f(0.0, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw finger 4-3
	glPushMatrix();
	/////////////////////////////////////////upper face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.5, -0.0, 0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.25, 0.0);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, -0.0, 0.0);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.25, 0.25);
	glVertex3f(0.5, -0.25, 0.0);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.25, -0.0, 0.0);
	glVertex3f(0.25, -0.0, 0.25);
	glVertex3f(0.25, -0.25, 0.25);
	glVertex3f(0.25, -0.25, 0.0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void armShoulder(float armorZ, int FB) { // FB = front = 1 back = 2
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	// Front face
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(0.0f, 0.0f, armorZ);          // right top
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-1.5f, 0.0f, armorZ);         // left top
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-1.5f, -1.0f, armorZ);        // left bottom
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(0.0f, -1.0f, armorZ);         // right bottom
	glEnd();

	// Side face (only if FB == 1)
	if (FB == 1) {
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(0.0f, 0.0f, armorZ);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(-1.5f, 0.0f, armorZ);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(-1.5f, 0.0f, -armorZ * 5);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(0.0f, 0.0f, -armorZ * 5);
		glEnd();
	}

	// Front triangle
	glBegin(GL_TRIANGLES);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-1.5f, 0.0f, armorZ);
	glTexCoord2f(0.5f, 0.5f); 
	glVertex3f(-1.7f, -0.3f, armorZ);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-1.5f, -1.0f, armorZ);
	glEnd();

	// Another Quad
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-1.5f, 0.0f, armorZ);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-1.7f, 0.0f, armorZ / 2);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-1.9f, -0.3f, armorZ / 2);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-1.7f, -0.3f, armorZ);
	glEnd();

	// Another Quad
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-1.7f, -0.3f, armorZ);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-1.9f, -0.3f, armorZ / 2);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-1.65f, -1.2f, armorZ / 2);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-1.5f, -1.0f, armorZ);
	glEnd();

	// Front triangle bottom
	glBegin(GL_TRIANGLES);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-1.5f, -1.0f, armorZ);
	glTexCoord2f(0.5f, 0.5f); 
	glVertex3f(-0.4f, -1.3f, armorZ);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(0.0f, -1.0f, armorZ);
	glEnd();

	// Bottom quad
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-1.5f, -1.0f, armorZ);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-1.65f, -1.2f, armorZ / 2);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-0.4f, -1.5f, armorZ / 2);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-0.4f, -1.3f, armorZ);
	glEnd();

	// Connecting Quad
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(0.0f, -1.0f, armorZ);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(0.0f, -1.2f, armorZ / 2);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-0.4f, -1.5f, armorZ / 2);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-0.4f, -1.3f, armorZ);
	glEnd();

	// Front larger face
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(0.0f, 0.0f, armorZ / 2);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-3.0f, 0.0f, armorZ / 2);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-3.0f, -2.0f, armorZ / 2);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(0.0f, -2.0f, armorZ / 2);
	glEnd();

	// Side triangles and quads
	glBegin(GL_TRIANGLES);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-3.0f, 0.0f, armorZ / 2);
	glTexCoord2f(0.5f, 0.5f); 
	glVertex3f(-3.4f, -0.6f, armorZ / 2);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-3.0f, -2.0f, armorZ / 2);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-3.0f, 0.0f, armorZ / 2);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-3.4f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-3.8f, -0.6f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-3.4f, -0.6f, armorZ / 2);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-3.4f, -0.6f, armorZ / 2);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-3.8f, -0.6f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-3.3f, -2.4f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-3.0f, -2.0f, armorZ / 2);
	glEnd();

	// Bottom connecting triangles
	glBegin(GL_TRIANGLES);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-3.0f, -2.0f, armorZ / 2);
	glTexCoord2f(0.5f, 0.5f); 
	glVertex3f(-0.8f, -2.6f, armorZ / 2);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(0.0f, -2.0f, armorZ / 2);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(-3.0f, -2.0f, armorZ / 2);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(-3.3f, -2.4f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-0.8f, -3.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-0.8f, -2.6f, armorZ / 2);
	glEnd();

	// Final Quad
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex3f(0.0f, -2.0f, armorZ / 2);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex3f(0.0f, -2.4f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-0.8f, -3.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex3f(-0.8f, -2.6f, armorZ / 2);
	glEnd();

	/////////part's line
	glColor3f(0.0, 0.0, 0.0);

	if (FB == 1) {
		glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, armorZ);
		glVertex3f(-1.5, 0.0, armorZ);
		glVertex3f(-1.5, 0.0, -armorZ * 5);
		glVertex3f(0.0, 0.0, -armorZ * 5);
		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	glLineWidth(2.0);
	glVertex3f(0.0, 0.0, armorZ); //right top
	glVertex3f(-1.5, 0.0, armorZ); //left top
	glVertex3f(-1.7, -0.3, armorZ);//left mid
	glVertex3f(-1.5, -1.0, armorZ); //right btm
	glVertex3f(-0.4, -1.3, armorZ); //mid btm
	glVertex3f(0.0, -1.0, armorZ); //right top
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.5, 0.0, armorZ);
	glVertex3f(-1.7, 0.0, armorZ / 2);
	glVertex3f(-1.9, -0.3, armorZ / 2);
	glVertex3f(-1.7, -0.3, armorZ);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.7, -0.3, armorZ);
	glVertex3f(-1.9, -0.3, armorZ / 2);
	glVertex3f(-1.65, -1.2, armorZ / 2);
	glVertex3f(-1.5, -1.0, armorZ);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.5, -1.0, armorZ);
	glVertex3f(-1.65, -1.2, armorZ / 2);
	glVertex3f(-0.4, -1.5, armorZ / 2);
	glVertex3f(-0.4, -1.3, armorZ);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -1.0, armorZ);
	glVertex3f(0.0, -1.2, armorZ / 2);
	glVertex3f(-0.4, -1.5, armorZ / 2);
	glVertex3f(-0.4, -1.3, armorZ);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glLineWidth(2.0);
	glVertex3f(0.0, 0.0, armorZ / 2); //right top
	glVertex3f(-3.0, 0.0, armorZ / 2); //left top
	glVertex3f(-3.4, -0.6, armorZ / 2);//left mid
	glVertex3f(-3.0, -2.0, armorZ / 2); //right btm
	glVertex3f(-0.8, -2.6, armorZ / 2); //mid btm
	glVertex3f(0.0, -2.0, armorZ / 2); //right top
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-3.0, 0.0, armorZ / 2);
	glVertex3f(-3.4, 0.0, 0.0);
	glVertex3f(-3.8, -0.6, 0.0);
	glVertex3f(-3.4, -0.6, armorZ / 2);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-3.4, -0.6, armorZ / 2);
	glVertex3f(-3.8, -0.6, 0.0);
	glVertex3f(-3.3, -2.4, 0.0);
	glVertex3f(-3.0, -2.0, armorZ / 2);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(-3.0, -2.0, armorZ / 2);
	glVertex3f(-3.3, -2.4, 0.0);
	glVertex3f(-0.8, -3.0, 0.0);
	glVertex3f(-0.8, -2.6, armorZ / 2);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, -2.0, armorZ / 2);
	glVertex3f(0.0, -2.4, 0.0);
	glVertex3f(-0.8, -3.0, 0.0);
	glVertex3f(-0.8, -2.6, armorZ / 2);
	glEnd();
	glPopMatrix();
}

void upperArmor() {
	glPushMatrix();
	glColor3f(white);
	glTranslatef(0.0, -2.0, 0.0);
	glScalef(1.5, 4.1, 1.5);
	drawCube1(0.5);
	drawCube1Line(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.9, -2.0, 0.0);
	glScalef(0.3, 4.1, 1.5);
	glColor3f(red);
	drawSideCube(0.5);
	glColor3f(black);
	drawSideCubeLine(0.5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	glTranslatef(-0.9, -2.0, 0.0);
	glScalef(0.3, 4.1, 1.5);
	glColor3f(red);
	drawSideCube(0.5);
	glColor3f(black);
	drawSideCubeLine(0.5);
	glPopMatrix();
}

void lowerArmor() {
	glPushMatrix();
	glColor3f(white);
	glTranslatef(0.0, -2.0, 0.0);
	glScalef(1.2, 3.1, 1.2);
	drawCube1(0.5);
	drawCube1Line(0.5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(-0.75, -2.0, 0.0);
	glScalef(0.3, 3.1, 1.3);
	glColor3f(red);
	drawSideCube(0.5);
	glColor3f(black);
	drawSideCubeLine(0.5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	glTranslatef(-0.75, -2.0, 0.0);
	glScalef(0.3, 3.1, 1.3);
	glColor3f(red);
	drawSideCube(0.5);
	glColor3f(black);
	drawSideCubeLine(0.5);
	glPopMatrix();
	glPopMatrix();
}

void palmArmor() {
	glPushMatrix();
	glColor3f(white);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glTranslatef(-0.3, -0.5, 0.0);
	glScalef(0.25, 1.0, 1.0);
	glColor3f(red);
	drawSideCube(0.5);
	glColor3f(black);
	drawSideCubeLine(0.5);
	glPopMatrix();
}

void weapon() {
	glPushMatrix();
	glColor3f(lightblack);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.25, 0.25, 3.0);
	glPushMatrix();
	if (TogOnWeapon == true) {
		WeaponS1 -= WeaponSpeed;
		if (WeaponS1 <= -3.0) {
			WeaponS1 = -3.0;
			WeaponS2 -= WeaponSpeed;
			if (WeaponS2 <= -3.0) {
				WeaponS2 = -3.0;
				WeaponS3 -= WeaponSpeed;
				if (WeaponS3 <= -3.0) {
					WeaponS3 = -3.0;
				}
			}
		}
	}
	else {
		WeaponS1 += WeaponSpeed;
		if (WeaponS1 >= 0.0) {
			WeaponS1 = 0.0;
			WeaponS2 += WeaponSpeed;
			if (WeaponS2 >= 0.0) {
				WeaponS2 = 0.0;
				WeaponS3 += WeaponSpeed;
				if (WeaponS3 >= 0.0) {
					WeaponS3 = 0.0;
				}
			}
		}
	}
	glTranslatef(0.0, 0.0, WeaponS1);
	glColor3f(red);
	drawCylinder(0.245, 0.245, 3.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, WeaponS2);
	drawCylinder(0.245, 0.245, 3.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, WeaponS3);
	drawCylinder(0.245, 0.245, 3.0);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void arm(float LUAX, float LUAY, float LUAZ, float LLAX, float LLAY, float LLAZ, float LP, float RUAX, float RUAY, float RUAZ, float RLAX, float RLAY, float RLAZ, float RP, int LR) {
	glPushMatrix();//////////////////////////////////////////////////////////////////////move whole arm
	glRotatef(LUAX + RUAX, 1.0, 0.0, 0.0); // raise upperarm
	glRotatef(LUAY + RUAY, 0.0, 1.0, 0.0); // raise upperarm
	glRotatef(LUAZ + RUAZ, 0.0, 0.0, 1.0); // raise upperarm
	glPushMatrix();
	if (LR == 2) {
		glRotatef(180.0, 0.0, 1.0, 0.0);
	}
	glTranslatef(1.0, 1.0, 1.0);
	armShoulder(0.5, 1);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.0);
	armShoulder(-0.5, 2);
	glPopMatrix();
	glPopMatrix();
	//////////////////////////////////////////////////////////////upper arm joint to body
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 0.5, 0.0);
	drawSphere(0.5, 30, 30);
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw left upper arm
	glPushMatrix();
	glPushMatrix();
	upperArmor();
	glPopMatrix();
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
	glRotatef(LLAX + RLAX, 1.0, 0.0, 0.0); //move lower arm
	glRotatef(LLAY + RLAY, 0.0, 1.0, 0.0); //move lower arm
	glRotatef(LLAZ + RLAZ, 0.0, 0.0, 1.0); //move lower arm
	//////////////////////////////////////////////////////////////upper arm joint to lower arm
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	drawSphere(0.5, 30, 30);
	glPopMatrix();
	//////////////////////////////////////////////////////////////draw left lower arm
	glPushMatrix();
	glPushMatrix();
	lowerArmor();
	glPopMatrix();
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
	glRotatef(LP + RP, 1.0, 0.0, 0.0); // move palm
	glPushMatrix();
	palmArmor();
	glPopMatrix();
	//////////////////////////////////////////////////////////////palm
	glPushMatrix();
	/////////////////////////////////////////upper face
	glColor3f(black);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.0, -0.25);
	glVertex3f(-0.5, -0.0, -0.25);
	glEnd();
	/////////////////////////////////////////lower face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -1.0, 0.25);
	glVertex3f(0.5, -1.0, 0.25);
	glVertex3f(0.5, -1.0, -0.25);
	glVertex3f(-0.5, -1.0, -0.25);
	glEnd();
	/////////////////////////////////////////front face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -1.0, 0.25);
	glVertex3f(-0.5, -1.0, 0.25);
	glEnd();
	/////////////////////////////////////////back face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, -0.25);
	glVertex3f(0.5, -0.0, -0.25);
	glVertex3f(0.5, -1.0, -0.25);
	glVertex3f(-0.5, -1.0, -0.25);
	glEnd();
	/////////////////////////////////////////left face
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.0, 0.25);
	glVertex3f(-0.5, -0.0, -0.25);
	glVertex3f(-0.5, -1.0, -0.25);
	glVertex3f(-0.5, -1.0, 0.25);
	glEnd();
	/////////////////////////////////////////right face
	glBegin(GL_QUADS);
	glVertex3f(0.5, -0.0, 0.25);
	glVertex3f(0.5, -0.0, -0.25);
	glVertex3f(0.5, -1.0, -0.25);
	glVertex3f(0.5, -1.0, 0.25);
	glEnd();
	glPopMatrix();
	if (TogWeapon == true) {
		if (LR == 1) {
			glPushMatrix();
			glTranslatef(-1.0, -0.8, 0.5);
			glRotatef(90, 0.0, 0.0, 1.0);
			weapon();
			glPopMatrix();
		}
	}
	//////////////////////////////////////////////////////////////draw finger
	drawFinger(FRotate, LR);
	drawFingerLine(FRotate, LR);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void action() {
	if (TogFinger) {  // TogFinger is true
		FRotate -= 1;  // Rotate counterclockwise (decrease)
		if (FRotate < -45) {
			FRotate = -45;  // Clamp the rotation to -45 degrees
		}
	}
	else {  // TogFinger is false
		FRotate += 1;  // Rotate clockwise (increase)
		if (FRotate > 1) {
			FRotate = 1;  // Clamp the rotation to 1 degree
		}
	}

	if (Act1) {
		if (TogOnWeapon) {
			if (Slash) {
				LLARotateZ += 1.0;
				if (LLARotateZ >= 0) {
					LLARotateZ = 0;
					LUARotateZ -= 1.0;
					if (LUARotateZ <= -90) {
						LUARotateZ = -90.0;
						Slash = false;
						Act1 = false;
					}
				}
			}
			else
			{
				LUARotateX -= 0.5;
				LLARotateY += 1.0;
				if (LUARotateX <= -75) {
					LUARotateX = -75;
					LLARotateY = 180;
					LLARotateZ -= 0.5;
					if (LLARotateZ <= -50) {
						LLARotateZ = -50;
						Slash = true;
					}
				}
			}
		}
	}
	if (Act2) {
		if (TogOnWeapon) {
			if (VSlash) {
				LLARotateX += 3.0;
				if (LLARotateX >= 0.0) {
					LLARotateX = 0.0;
					LUARotateX += 3.0;
					if (LUARotateX >= 0) {
						LUARotateX = 0;
						VSlash = false;
						Act2 = false;
					}
				}
			}
			else {
				LLARotateY += 1;
				if (LLARotateY >= 90) {
					LLARotateY = 90;
					LUARotateX -= 1.0;
					if (LUARotateX <= -90) {
						LUARotateX = -90;
						LLARotateX -= 1.0;
						if (LLARotateX <= -100) {
							LLARotateX = -100;
							VSlash = true;
						}
					}
				}
			}
		}
	}

	if (Act3) {
		if (SwingFront) {
			LUARotateX += 0.25;
			RUARotateX -= 0.25;
			if (LUARotateX >= 20) {
				SwingFront = false;
			}
		}
		else {
			LLARotateY += 1.0;
			RLARotateY -= 1.0;
			if (LLARotateY >= 90) {
				LLARotateY = 90;
				RLARotateY = -90;
				LUARotateX -= 0.25;
				RUARotateX += 0.25;
				LLARotateX -= 1.0;
				RLARotateX -= 1.0;
				if (LUARotateX <= -20) {
					SwingFront = true;
				}
				if (LLARotateX <= -20) {
					LLARotateX = -20;
					RLARotateX = -20;
				}
			}
		}
	}

	if (TogWalk) {
		walk();
	}
}

void head(int LR) {
	glPushMatrix();
	//middle part
	glBegin(GL_QUADS);
	glColor3f(white);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.3, -0.5);
	glVertex3f(-1.0, 1.0, -0.5);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-1.0, 0.0, -0.5);
	glVertex3f(-1.0, 1.0, -0.5);
	glVertex3f(-1.2, 1.0, -1.5);
	glVertex3f(-1.2, 0.0, -1.5);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-1.2, 1.0, -1.5);
	glVertex3f(-1.2, 0.0, -1.5);
	glVertex3f(-1.0, 0.0, -2.5);
	glVertex3f(-1.0, 1.0, -2.5);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-1.0, 0.0, -2.5);
	glVertex3f(-1.0, 1.0, -2.5);
	glVertex3f(0.0, 1.0, -3.0);
	glVertex3f(0.0, 0.0, -3.0);
	glEnd();

	//lower part
	glBegin(GL_QUADS);
	glVertex3f(-1.0, 0.0, -0.5);
	glVertex3f(-1.2, 0.0, -1.5);
	glVertex3f(-1.2, -1.25, -1.5);
	glVertex3f(-1.0, -1.5, -0.5);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-1.2, 0.0, -1.5);
	glVertex3f(-1.0, -0.0, -2.5);
	glVertex3f(-1.0, -1.0, -2.5);
	glVertex3f(-1.2, -1.25, -1.5);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-1.0, 0.0, -2.5);
	glVertex3f(0.0, 0.0, -3.0);
	glVertex3f(0.0, -0.85, -3.0);
	glVertex3f(-1.0, -1.0, -2.5);
	glEnd();

	//upper part
	glBegin(GL_QUADS);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -0.5);
	glVertex3f(-1.0, 1.5, -0.75);
	glVertex3f(0.0, 1.5, -0.45);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-1.0, 1.0, -0.5);
	glVertex3f(-1.2, 1.0, -1.5);
	glVertex3f(-1.15, 1.5, -1.5);
	glVertex3f(-1.0, 1.5, -0.75);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-1.2, 1.0, -1.5);
	glVertex3f(-1.0, 1.0, -2.5);
	glVertex3f(-0.75, 1.5, -2.5);
	glVertex3f(-1.15, 1.5, -1.5);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-1.0, 1.0, -2.5);
	glVertex3f(0.0, 1.0, -3.0);
	glVertex3f(0.0, 1.5, -3.0);
	glVertex3f(-0.75, 1.5, -2.5);
	glEnd();
	glPopMatrix();

	//Line part
	glPushMatrix();
	glColor3f(black);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.3, -0.5);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.0, 0.3, -0.5);
	glVertex3f(-1.0, -1.5, -0.5);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.2, -1.25, -1.5);
	glVertex3f(-1.0, -1.5, -0.5);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.0, -1.0, -2.5);
	glVertex3f(-1.2, -1.25, -1.5);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, -0.85, -3.0);
	glVertex3f(-1.0, -1.0, -2.5);
	glEnd();
	glPopMatrix();

	//upper middle part
	if (LR == 1) {
		glPushMatrix();
		glTranslatef(0.0, 0.8, -0.2);
		glRotatef(45, 1.0, 0.0, 0.0);
		glScalef(0.5, 1.0, 1.0);
		glColor3f(red);
		drawCube1(0.5);
		glColor3f(black);
		drawCube1Line(0.5);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0, 2.0, -1.7);
		glRotatef(45, 1.0, 0.0, 0.0);
		glScalef(0.5, 2.0, 1.0);
		glColor3f(black);
		drawCube1(0.5);
		glColor3f(black);
		drawCube1Line(0.5);
		glPopMatrix();

		//inside face
		glPushMatrix();
		glRotatef(-90, 1.0, 0.0, 0.0);
		glTranslatef(0.0, 1.65, -1.0);
		drawCylinder(1.1, 1.1, 2);
		glPopMatrix();
	}
	// upper middle \\// part
	glPushMatrix();
	glTranslatef(-0.4, 0.8, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(-45, 0.0, 1.0, 0.0);
	glColor3f(red);
	drawCylinder(0.15, 0.0, 3);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-15, 0.0, 1.0, 0.0);
	glColor3f(yellow);
	drawCylinder(0.15, 0.0, 3);
	glPopMatrix();
	glPopMatrix();

	//mask
	glPushMatrix();
	glColor3f(red);
	glBegin(GL_QUADS);
	glVertex3f(0.0, -0.45, -0.5);
	glVertex3f(-1.0, -0.5, -0.65);
	glVertex3f(-1.0, -1.35, -0.65);
	glVertex3f(0.0, -1.5, -0.5);
	glEnd();
	glPopMatrix();

	//beside mask
	glPushMatrix();
	glRotatef(-20, 0.0, 1.0, 0.0);
	glRotatef(10, 1.0, 1.0, 0.0);
	glScalef(0.4, 1.0, 2.0);
	glTranslatef(-4.2, -0.25, -0.8);
	drawSideCube(0.5);
	drawSideCubeLine(0.5);
	glPopMatrix();
}

void robot() {
	glClearColor(0.75, 0.75, 0.75, 0.0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	projection();

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	lighting();
	glPopMatrix();

	GLuint textureArr[6];

	//------------------------------------------------------------------------------------------------------background
	/*glPushMatrix();
	glTranslatef(0, 0, 0);
	textureArr[5] = loadTexture("s.bmp");
	glColor3f(1, 1, 1);
	drawTextSphere(20);
	glDeleteTextures(1, &textureArr[5]);
	glPopMatrix();*/


	glPushMatrix();
	glTranslatef(xPosition, yPosition, zPosition);
	glRotatef(rotateY, 0, 1, 0);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(90, 0.0, 1.0, 0.0);

	///--------------------------------translate whole
	if (txwhole < -8.5) {
		txwhole = 8.5;
	}
	glPushMatrix();
	glRotatef(rp, 0, 1, 0.0);
	glTranslatef(txwhole, tywhole, tzwhole);


	

	switch (change) {
		case 1:
			textureArr[0] = loadTexture("a.bmp");
			break;
		case 2:
			textureArr[1] = loadTexture("b.bmp");
			break;
		case 3:
			textureArr[2] = loadTexture("c.bmp");
			break;
		case 4:
			textureArr[3] = loadTexture("d.bmp");
			break;
	}



	glPushMatrix();
	glScalef(0.5 ,0.5, 0.5);

	glPushMatrix();
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-10, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 7.25, 2.5);
	glPushMatrix();
	glColor3f(white);
	glScalef(2.3, 2.0, 2.5);
	glTranslatef(0.0, 0.6, -0.675);
	drawSphere(0.5, 30, 30);
	glPopMatrix();
	head(1);
	glPushMatrix();
	glScalef(-1.0, 1.0, 1.0);
	head(2);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(-4.0, 5.0, 0.0);
	arm(LUARotateX, LUARotateY, LUARotateZ, LLARotateX, LLARotateY, LLARotateZ, LPRotate, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1); // left arm
	glPopMatrix();
	glPushMatrix();
	glTranslatef(4.0, 5.0, 0.0);
	arm(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, RUARotateX, RUARotateY, RUARotateZ, RLARotateX, RLARotateY, RLARotateZ, RPRotate, 2); // right arm
	glPopMatrix();
	glPopMatrix();
	body();
	leg();
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();

	glDeleteTextures(1, &textureArr[0]);
	glDeleteTextures(1, &textureArr[1]);
	glDeleteTextures(1, &textureArr[2]);
	glDeleteTextures(1, &textureArr[3]);
	glDeleteTextures(1, &textureArr[4]);
	glDisable(GL_TEXTURE_2D);


	action();



}

void display()
{
	switch (Qnum) {
	case 1:
		robot();
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
