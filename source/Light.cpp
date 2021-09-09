#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Light.h"

Light::Light():
	m_bLighting(false),
	m_bLanternMode(false),
	m_lightSpot(10.0f)
{
	m_lightDifuse[0] = 0.8f,	m_lightDifuse[1] = 0.8f,	m_lightDifuse[2] = 0.8f,	m_lightDifuse[3] = 1.0f;
	m_lightAmbient[0] = 0.0f,	m_lightAmbient[1] = 0.0f,	m_lightAmbient[2] = 0.0f,	m_lightAmbient[3] = 1.0f;
	m_lightSpecular[0] = 0.0f,	m_lightSpecular[1] = 0.0f,	m_lightSpecular[2] = 0.0f,	m_lightSpecular[3] = 1.0f;
	m_lightPosition[0] = 0.0f,	m_lightPosition[1] = 20.0f,	m_lightPosition[2] = 20.0f,	m_lightPosition[3] = 1.0f;
	m_lightDir[0] = 0.0f,		m_lightDir[1] = 0.0f,		m_lightDir[2] =	-1.0f,		m_lightDir[3] = 1.0f;

	//ShowValues();
}

void Light::Draw()
{

	if (m_bLighting)
	{
		glPushMatrix();
			//glEnable(GL_NORMALIZE);
			//a esfera que fica na posicao da luz
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glColor3fv(m_lightDifuse);
			glTranslatef(m_lightPosition[0], m_lightPosition[1], m_lightPosition[2]);
			glutSolidSphere(2.0f, 20, 20);
			//fim

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightDifuse);
			glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmbient);
			glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpecular);
			glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
			//glMaterialfv(GL_FRONT, GL_EMISSION, m_lightDifuse);
			if(!m_bLanternMode)
				glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, m_lightDir);
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, m_lightSpot);

		glPopMatrix();
	}
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	
}
Light::~Light()
{

}

//void Light::ShowValues()
//{
//	printf("lightDifuse: %.2f, %.2f, %.2f, %.2f\n", m_lightDifuse[0], m_lightDifuse[1], m_lightDifuse[2], m_lightDifuse[3]);
//	printf("m_lightAmbient: %.2f, %.2f, %.2f, %.2f\n", m_lightAmbient[0], m_lightAmbient[1], m_lightAmbient[2], m_lightAmbient[3]);
//	printf("m_lightSpecula: %.2f, %.2f, %.2f, %.2f\n", m_lightSpecular[0], m_lightSpecular[1], m_lightSpecular[2], m_lightSpecular[3]);
//	printf("m_lightPosition: %.2f, %.2f, %.2f, %.2f\n", m_lightPosition[0], m_lightPosition[1], m_lightPosition[2], m_lightPosition[3]);
//}