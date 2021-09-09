#include <stdio.h>
#include "Visualizer3D.h"
#include <GL/freeglut.h>
#include "Primitives.h"
#include "ObjLoader.h"
#include "Light.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glut.h"
#include "../imgui/imgui_impl_opengl2.h"

Camera Visualizer3D::camera;
Light* light;
ObjLoader* object;
ImVec4 objColor(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 clearColor(0.0f, 0.0f, 0.0f, 1.0f);
ImVec4 vLineColor(1.0f, 0.0f, 0.0f, 0.0f);
float fObjScale = 1.0f;
float fLineWidth = 1.0f;
float fGrounfScale = 1.0f;

bool bGL_fill = true;
bool bTexture = true;
bool bMulticolor = false;
bool yRotate = false;
bool bWireframe = false;
bool bFps60 = true;
bool bHideGround = false;
bool bFlat = false;
bool bSmooth = true;


unsigned WIDTH = 1200;
unsigned HEIGHT = 800;
//ground
GLfloat fGroundDifusa[] = { 0.5, 0.5, 0.0, 1.0 };
GLfloat fGroundSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat fGroundAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat fGroundPrilho[] = { 50.0 };

std::vector<vec3f>vec3GroundVertices;

Visualizer3D::Visualizer3D()
{
}

Visualizer3D::~Visualizer3D()
{
}


void Visualizer3D::Reshape(int w, int h)
{
	ImGui_ImplGLUT_ReshapeFunc(w, h);
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

void Visualizer3D::DrawImGuiWindows()
{

	static float f = 0.5f; //velocidade de rotaçao

	int wWidth = 250;
	int wHeight = 0;

	int viewport[4];
	ImVec4 titleColor(1.0f, 0.0f, 0.0f, 1.0f);
	//ImGui::ShowDemoWindow();
	glGetIntegerv(GL_VIEWPORT, viewport);
	//printf("%d %d %d %d\n", viewport[0], viewport[1], viewport[2], viewport[3]);
	ImGui::Begin("PROPRIEDADES");
	
	ImGui::PushItemWidth(ImGui::GetWindowWidth() - 20);
	if (!ImGui::GetIO().WantCaptureMouse) {
		ImGui::SetWindowSize(ImVec2((float)wWidth, (float)viewport[3]));
		ImGui::SetWindowPos(ImVec2((float)(viewport[2] - wWidth), 0));
	}
	ImGui::Checkbox("Limitar FPS(60)", &bFps60);
	ImGui::Text("Media da aplicacao %.3f ms\nFrame: (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Separator();
	ImGui::TextColored(titleColor, "PROPRIEDADES DO OBJETO.");
	ImGui::Text("Total de partes: %i meshs\n", object->GetTotalMeshs());
	ImGui::Text("Total de vertices: %i", object->GetTotalVertexCoord());
	ImGui::Text("Total de faces: %i", object->GetTotalFaces());
	//ImGui::Text("propriedades do objeto da tela.");

	ImGui::NewLine();
	ImGui::Checkbox("Rotacionar objeto", &object->m_bRotate);
	//rotacao do objeto
	if (object->m_bRotate) {
		ImGui::Text("Vel. rotacao");
		ImGui::SliderFloat("## vel. Rotacao", &object->m_fRotateVel, -100.0f, 100.0f);
	}
	ImGui::Text("Cor do Objeto 3D");
	ImGui::ColorEdit3("##obc", (float*)&objColor);
	ImGui::Text("Cor de Fundo");
	ImGui::ColorEdit3("##cb", (float*)&clearColor); // Edit 3 floats representing a color
	ImGui::Text("Escala do Objeto");
	ImGui::SliderFloat("##escala do objeto", &fObjScale, 0.0f, 10.0f);

	ImGui::Checkbox("habilitar Texturas", &bTexture);
	ImGui::Checkbox("habilitar MultiColor", &bMulticolor);
	ImGui::Checkbox("Girar em Y", &yRotate);
	if (yRotate)
	{
		ImGui::Text("Velocidade de Rotacao");
		ImGui::SliderFloat("##Velocidade De Rotacao", &f, -10.0f, 10.0f);
	}
	ImGui::Checkbox("Modo WireFrame", &bWireframe);
	if (bWireframe)
	{
		//glTexS
		ImGui::Checkbox("Cor de preenchiment", &bGL_fill);
		ImGui::Text("Cor da linha");
		ImGui::ColorEdit3("##Cor da linha", (float*)&vLineColor);
		ImGui::Text("Largura da linha");
		ImGui::SliderFloat("##Cor da linha", &fLineWidth, 1.0f, 5);
	}


	camera.SetRotate(yRotate);
	camera.SetRotateVelocity(f);

	ImGui::NewLine();
	//Plano
	ImGui::Separator();
	ImGui::TextColored(titleColor, "PROPRIEDADES DO PLANO");
	ImGui::Checkbox("Ocultar plano", &bHideGround);
	ImGui::Text("Escala do plano");
	ImGui::SliderFloat("##fGrounfScale", &fGrounfScale, 0, 10);
	ImGui::Text("Cor Difusa do plano");
	ImGui::ColorEdit3("##fGroundDifusa", fGroundDifusa);
	ImGui::Text("Cor Ambient do plano");
	ImGui::ColorEdit3("##fGroundAmbient", fGroundAmbient);
	ImGui::Text("Cor Specular do plano");
	ImGui::ColorEdit3("##fGroundSpecular", fGroundSpecular);
	ImGui::Text("Brilho do plano");
	ImGui::SliderFloat("##fGroundPrilho", &fGroundPrilho[0], 0, 200.0f);
	ImGui::Separator();
	ImGui::TextColored(titleColor, "PROPRIEDADES DA LUZ");
	ImGui::Checkbox("Habilitar Luz", &light->m_bLighting);
	if (light->m_bLighting)
	{
		ImGui::Checkbox("flat", &bFlat);
		bSmooth = !bFlat;
		ImGui::Checkbox("Smooth", &bSmooth);
		bFlat = !bSmooth;
		ImGui::Checkbox("Modo Lanterna", &light->m_bLanternMode);
		ImGui::Text("Luz difusa");
		ImGui::ColorEdit4("##cor difusa", light->m_lightDifuse);
		ImGui::Text("Luz ambiente");
		ImGui::ColorEdit4("##Luz Ambiente ", light->m_lightAmbient);
		ImGui::Text("Luz Especular ");
		ImGui::ColorEdit4("##Luz Especula", light->m_lightSpecular);
		//posicao da luz
		ImGui::Text("Posicao da luz");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() - 40);
		ImGui::SliderFloat("x ##pos", &light->m_lightPosition[0], -100, 100.0f);
		ImGui::SliderFloat("y ##pos", &light->m_lightPosition[1], -100, 100.0f);
		ImGui::SliderFloat("z ##pos", &light->m_lightPosition[2], -100, 100.0f);
		//direcao da luz
		ImGui::Text("direcao da luz");
		ImGui::SliderFloat("x ##dir", &light->m_lightDir[0], -100, 100.0f);
		ImGui::SliderFloat("y ##dir", &light->m_lightDir[1], -100, 100.0f);
		ImGui::SliderFloat("z ##dir", &light->m_lightDir[2], -100, 100.0f);
		ImGui::Text("Angulo de abertura Fonte de Luz");
		ImGui::SliderFloat("##an dir", &light->m_lightSpot, 0, 180.0f);
		//ImGui::PushItemWidth(ImGui::GetWindowWidth() - 20);
	}
	//ImGui::PushItemWidth(ImGui::GetWindowWidth() - 100);
	ImGui::NewLine();
	ImGui::Separator();
	ImGui::TextColored(titleColor, "PROPRIEDADES DA CAMERA");
	ImGui::SliderFloat("##translate x", &camera.tx, -100.0f, 100.0f, "Trans. x %0.2f");
	ImGui::SliderFloat("##translate y", &camera.ty, -100.0f, 100.0f, "Trans. y %0.2f");
	ImGui::SliderFloat("##translate z", &camera.tz, -100.0f, 100.0f, "Trans. z %0.2f");

	ImGui::SliderFloat("##Rotacionar x", &camera.rx, -100.0f, 100.0f, "Rot. x %0.2f");
	ImGui::SliderFloat("##Rotacionar y", &camera.ry, -100.0f, 100.0f, "Rot. y %0.2f");
	ImGui::SliderFloat("##Rotacionar z", &camera.rz, -100.0f, 100.0f, "Rot. z %0.2f");
	ImGui::End();
	
}

void Visualizer3D::DrawWireFrameMode()
{
	float color[6][3] = { {1.0, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f},{0.0f, 1.0f, 1.0f} };
	
	if (glIsEnabled(GL_LIGHTING)) {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	if(glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(fLineWidth);
	glColor3fv((float*)&vLineColor);

	glRotatef(object->m_fAngleRotate, 0.0f, 1.0f, 0.0f);
	//glColor3f(objColor.x, objColor.y, objColor.z);

	for (unsigned int i = 0; i < object->Meshs.size(); i++)
	{
		for (unsigned int j = 0; j < object->Meshs[i].faces.size(); j++)
		{
			unsigned int size = object->Meshs[i].faces[j].index.size();

			glBegin(size == 4 ? GL_QUADS : size == 3 ? GL_TRIANGLES : 0);

			for (unsigned int k = 0; k < size; k++)
			{
				int vindex = object->Meshs[i].faces[j].index[k].x;
				glVertex3f(object->Meshs[i].vCoords[vindex - 1].x, object->Meshs[i].vCoords[vindex - 1].y, object->Meshs[i].vCoords[vindex - 1].z);
			}
			glEnd();
		}
	}
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	light->m_bLighting ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	bTexture ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
}

void Visualizer3D::Display()
{
	
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	DrawImGuiWindows();
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, bGL_fill ? GL_FILL:GL_LINE);
	//
	// inicia ImGui Frame
	glPushMatrix();
	camera.Update();
	if (bFlat) {
		glShadeModel(GL_FLAT);
	}
	else if (bSmooth) {
		glShadeModel(GL_SMOOTH);
	}

	DrawGround();
	light->Draw();
	glScalef(fObjScale, fObjScale, fObjScale);

	if (bTexture) {
		if (!glIsEnabled(GL_TEXTURE_2D))
			glEnable(GL_TEXTURE_2D);
	}
	else if (glIsEnabled(GL_TEXTURE_2D)) {
		glDisable(GL_TEXTURE_2D);
	}

	if (object->m_bRotate)
		object->m_fAngleRotate += object->m_fRotateVel;

	if(bGL_fill)
		DrawObject();
	if (bWireframe)
		DrawWireFrameMode();
	else {
		bGL_fill = true;
	}

	
	glPopMatrix();
	// Rendering
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	glFinish();
	glFlush();
	glutSwapBuffers();
}
void Visualizer3D::CreateGround()
{
	int d = 50;
	float ratio = 0.5f;
	float tmp_x = 0.0f;
	float tmp_z = 0.0f;

	for (int i = -d; i < d; i++)
	{
		tmp_z = i * ratio;
		for (int j = -d; j < d; j++)
		{
			tmp_x = j * ratio;
			vec3GroundVertices.push_back(vec3f(tmp_x, -0.1f, tmp_z)); //top L
			vec3GroundVertices.push_back(vec3f(tmp_x, -0.1f, tmp_z + 1.0f)); //bottom L
			vec3GroundVertices.push_back(vec3f(tmp_x + 1.0f, -0.1f, tmp_z + 1.0f)); //botton r
			//vec3GroundVertices.push_back(vec3f(tmp_x + 1.0f, -0.1f,tmp_z + 1.0f)); //botton r
			vec3GroundVertices.push_back(vec3f(tmp_x + 1.0f, -0.1f, tmp_z)); //top r
			//vec3GroundVertices.push_back(vec3f(tmp_x, -0.1f, tmp_z)); //top L
		}
	}
}
void Visualizer3D::DrawGround()
{
	

	if (bHideGround)
		return;
	if (vec3GroundVertices.size() == 0)
		CreateGround();
	
	if(glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glScalef(fGrounfScale, fGrounfScale, fGrounfScale);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, fGroundDifusa);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fGroundSpecular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, fGroundAmbient);
	glMaterialfv(GL_FRONT, GL_SHININESS, fGroundPrilho);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fGroundDifusa);

	glColor3fv(fGroundDifusa);
	glNormal3f(0.0f, 1.0f, 0);
	glBegin(GL_QUADS);
	for (size_t i = 0; i < vec3GroundVertices.size(); i++)
	{
		glVertex3d(vec3GroundVertices[i].x, vec3GroundVertices[i].y, vec3GroundVertices[i].z);
	}
	glEnd();
	glPopMatrix();
	if (bTexture)
		glEnable(GL_TEXTURE_2D);
}
void Visualizer3D::DrawObject()
{
	float color[6][3] = { {1.0, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f},{0.0f, 1.0f, 1.0f} };
	static GLuint boundTexture = -1;

	glPushMatrix();
	glColor4fv((float*)&objColor);

	glRotatef(object->m_fAngleRotate, 0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < object->Meshs.size(); i++)
	{
		for (unsigned int j = 0; j < object->Meshs[i].faces.size(); j++)
		{
			unsigned int size = object->Meshs[i].faces[j].index.size();
			

			if (object->Meshs[i].texID > 0 && object->Meshs[i].texID != boundTexture && glIsEnabled(GL_TEXTURE_2D) == GL_TRUE) {
				glBindTexture(GL_TEXTURE_2D, object->Meshs[i].texID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				boundTexture = object->Meshs[i].texID;
			}
			if (object->Meshs[i].materialID >= 0) {
				/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (float*)&object->materials[object->Meshs[i].materialID].ambient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float*)&object->materials[object->Meshs[i].materialID].difuse);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float*)&object->materials[object->Meshs[i].materialID].specular);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (float*)&object->materials[object->Meshs[i].materialID].SpecularDensity);*/
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light->m_lightAmbient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float*)&objColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light->m_lightDifuse);
				//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f );
			}
			if(bMulticolor )
				glColor3fv(color[i % 6]);
			
			glBegin(size == 4 ? GL_QUADS : size == 3 ? GL_TRIANGLES : 0);
			
			for (unsigned int k = 0; k < size; k++)
			{
				int vindex = object->Meshs[i].faces[j].index[k].x;
				int tindex = object->Meshs[i].faces[j].index[k].y;
				int nindex = object->Meshs[i].faces[j].index[k].z;
				
				if(tindex > 0)
					glTexCoord2f(object->Meshs[i].tCoords[tindex - 1].x, 1.0f - object->Meshs[i].tCoords[tindex - 1].y);
				if(nindex > 0)
					glNormal3f(object->Meshs[i].nCoords[nindex - 1].x, object->Meshs[i].nCoords[nindex - 1].y, object->Meshs[i].nCoords[nindex - 1].z);
				if(vindex > 0)
					glVertex3f(object->Meshs[i].vCoords[vindex - 1].x, object->Meshs[i].vCoords[vindex - 1].y, object->Meshs[i].vCoords[vindex - 1].z);
			}
			glEnd();
		}
	}
	glPopMatrix();
	
}
void Visualizer3D::timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / (bFps60 ? 60 : 1000), timer, 0);
}

void Visualizer3D::run(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Visualizer 3D");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GLUT_MULTISAMPLE);
	if (argc > 1) {
		object = new ObjLoader(argv[1]);
		if (object)
			glutSetWindowTitle(argv[1]);
	}
	else {
		object = new ObjLoader("./Models/leon.obj");
	}
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(camera.KeyBoardEvent);
	glutMouseFunc(camera.MouseEvent);
	glutMotionFunc(camera.Motion);
	//glutIdleFunc(glutPostRedisplay);
	glutMouseWheelFunc(camera.WheelEvent);
	timer(0);
	//incialização da fonte de luz
	light = new Light();

	//Inicialização Imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	//ImGui_ImplGLUT_InstallFuncs(); incializa todos os callbacks
	ImGui_ImplOpenGL2_Init();
	
	glutMainLoop();
	// libera as resources do ImGui
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}


