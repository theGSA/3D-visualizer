#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include "../imgui/imgui_impl_glut.h"
#include "Camera.h"

#define M_PI 3.14159265358979323846

float Camera::rx = 0.0f;
float Camera::ry = 0.0f;
float Camera::rz = 0.0f;
float Camera::tx = -10.0f;
float Camera::ty = -10.0f;
float Camera::tz = -50.0f;
float Camera::upx = 0.0f;
float Camera::upy = 1.0f;
float Camera::upz = 0.0f;

float Camera::RotateVelocity = 0.5f;
bool Camera::rotate = false;

int mx = 0, my = 0;
bool LPressed = false;
bool RPressed = false;
Camera::Camera()
{
	
}

Camera::~Camera()
{
}

void Camera::Update()
{
	//static r = ;
	//static float x=  0 ;
	if (rotate)
		ry += RotateVelocity;

	glTranslatef(tx, ty, tz);
	glRotatef(rx, 1.0f, 0.0f, 0.0f);
	glRotatef(ry, 0.0f, 1.0f, 0.0f);
	glRotatef(rz, 0.0f, 0.0f, 1.0f);
	//glPushMatrix();
		//gluLookAt(tx, ty, tz, rz, ry, rz, upx, upy, upz);
		//x += 0.1;
	//glPopMatrix();
}

void Camera::KeyBoardEvent(unsigned char c, int x, int y)
{
	ImGui_ImplGLUT_KeyboardFunc(c, x, y);
	if (!ImGui::GetIO().WantCaptureKeyboard) {
		switch (c)
		{
		case 'w':
		case 'W':	tz += 0.5f; break;

		case 's':
		case 'S':	tz -= 0.5f; break;

		case 'a':
		case 'A':	tx += 0.5f; break;

		case 'd':
		case 'D':	tx -= 0.5f; break;

		default:
			break;
		}
	}
}

void Camera::MouseEvent(int b, int action, int x, int y)
{
	ImGui_ImplGLUT_MouseFunc(b, action, x, y);
	switch (b)
	{
	case GLUT_LEFT_BUTTON:
		LPressed =  !LPressed;
		if (action == GLUT_DOWN){
			mx = x, my = y;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		RPressed = !RPressed;
		if (action == GLUT_DOWN) {
			mx = x, my = y;
		}
		break;
	default:
		break;
	}
}

void Camera::Motion(int x, int y)
{
	ImGui_ImplGLUT_MotionFunc(x, y);
	float deltax = (x - mx) * 0.5f;
	float deltay = (y - my) * 0.5f;
	
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (LPressed)
		{
			rx += deltay;
			ry += deltax;

		}
		else if (RPressed)
		{
			ty -= deltay * 0.1f;
			tx += deltax * 0.1f;
		}
		mx = x;
		my = y;
	}
	
	
}

void Camera::WheelEvent(int b, int action, int x, int y)
{
	ImGui_ImplGLUT_MouseWheelFunc(b, action, x, y);
	if (!ImGui::GetIO().WantCaptureMouse) {
		tz += action * 2;
	}
}


