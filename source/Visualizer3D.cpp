#include <stdio.h>
#include "Visualizer3D.h"
#include <GL/freeglut.h>
#include "Primitives.h"
#include "ObjLoader.h"

Camera Visualizer3D::camera;
ObjLoader* object;

Visualizer3D::Visualizer3D()
{
}

Visualizer3D::~Visualizer3D()
{
}


void Visualizer3D::Reshape(int w, int h)
{
	
	if (h == 0) h = 1;

	float aspect = (float)w / (float)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, aspect, 0.1f, -500.0f);
	glMatrixMode(GL_MODELVIEW);
}

void Visualizer3D::KeyBoard(unsigned c, int x, int y)
{

}

void Visualizer3D::Display()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, object->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	camera.Update();
	//Primitives::DrawTriangle();
	//glScalef(0.05f, 0.05f, 0.05f);
	DrawObject();
	glPopMatrix();
	glFlush();
	glFinish();
	glutSwapBuffers();
}

void Visualizer3D::DrawObject()
{
	float color[6][3] = { {1.0, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f},{0.0f, 1.0f, 1.0f} };


	glPushMatrix();
	if (object->texID < 1)
		glColor3fv(color[2]);


	for (unsigned int i = 0; i < object->Meshs.size(); i++)
	{
		for (unsigned int j = 0; j < object->Meshs[i].faces.size(); j++)
		{
			unsigned int size = object->Meshs[i].faces[j].index.size();

			glBegin(size == 4 ? GL_QUADS : size == 3 ? GL_TRIANGLES : 0);
			for (unsigned int k = 0; k < size; k++)
			{
				int vindex = object->Meshs[i].faces[j].index[k].x;
				int tindex = object->Meshs[i].faces[j].index[k].y;
				if (object->texID > 0) {
					glTexCoord2f(object->Meshs[i].tCoords[tindex - 1].x, object->Meshs[i].tCoords[tindex - 1].y);
				}
				glVertex3f(object->Meshs[i].vCoords[vindex - 1].x, object->Meshs[i].vCoords[vindex - 1].y, object->Meshs[i].vCoords[vindex - 1].z);
				//glVertex3f(object->Meshs[i].vCoords[vindex - 1].normalize().x, object->Meshs[i].vCoords[vindex - 1].normalize().y, object->Meshs[i].vCoords[vindex - 1].normalize().z);
			}
			glEnd();
		}
	}

	glPopMatrix();

}

void Visualizer3D::run(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Visualizer 3D");
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	object = new ObjLoader("Objects/Anime_charcter.obj");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(camera.KeyBoardEvent);
	glutMouseFunc(camera.MouseEvent);
	glutMotionFunc(camera.Motion);
	glutIdleFunc(glutPostRedisplay);

	
	glutMainLoop();
}


