#include "Primitives.h"

void Primitives::DrawTriangle()
{
	float vertex[3][9] = {
		{0.0f, 0.5f, 0.0f},
		{-0.5, -0.5f, 0.0f },
		{0.5f, -.5f, 0.0f}
	};

	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
		glBegin(GL_TRIANGLES);
		glVertex3fv(vertex[0]);
		glVertex3fv(vertex[1]);
		glVertex3fv(vertex[2]);
	glEnd();
	glPopMatrix();
	
}
