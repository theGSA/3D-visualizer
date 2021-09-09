#include <Windows.h>
#include <GL/freeglut.h>
#include <SOIL2.h>
#include "stb_image.h"
#include "ObjLoader.h"
#include "vec3.h"
#include <algorithm>

ObjLoader::ObjLoader(std::string filename):
m_filename(filename),
nVertices(0),
nTex(0),
nNormals(0),
m_bRotate(false),
m_fRotateVel(0.5),
m_fAngleRotate(0)
{
	std::replace(m_filename.begin(), m_filename.end(), '\\','/');
	m_arq = fopen(m_filename.c_str(),"r");
	if (!m_arq) {
		printf("não foi possivel abrir (%s)\n", m_filename.c_str());
		exit(0);
	}
	else {
		printf("arquivo (%s) aberto com sucesso\n", m_filename.c_str());
		ParseFile();
		CreateTexture();
	}
}

ObjLoader::~ObjLoader()
{

}

void ObjLoader::CreateTexture()
{
	//int w, h, c;
	//unsigned char* data = SOIL_load_image("objects\\texturas\\plant.jpg", &w, &h, &c, 0);
	printf("criando texturas\n");
	std::vector<std::pair<unsigned, std::string>> textures;
	std::string TexturePath = m_filename.substr(0, m_filename.rfind("/")+1) + "texturas/";

	for (size_t i = 0; i < Meshs.size(); i++)
	{
		printf("materialname (%s) %d\n", Meshs[i].materialName.c_str(), materials.size());
		for (size_t j = 0; j < materials.size(); j++)
		{

			if (Meshs[i].materialName == materials[j].name) {
				printf("\tmap_Kd %s\n", materials[j].mapDifuse.c_str());
			
				std::string filename = TexturePath + materials[j].mapDifuse;
				int width, height, nrChannels;
				bool exist = false;
				Meshs[i].materialID = j;

				for (size_t k = 0; k < textures.size(); k++)
				{
					if (textures[k].second == filename) {
						exist = true;
						Meshs[i].texID = textures[k].first;
						break;
					}
				}
				
				if(!exist)
				{
					unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, &nrChannels, 0);

					if (data)
					{
						glGenTextures(1, &Meshs[i].texID);
						glBindTexture(GL_TEXTURE_2D, Meshs[i].texID);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

						GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
						printf("criando textura(%s)(texID: %d)\n", filename.c_str(), Meshs[i].texID);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
						textures.push_back(std::make_pair(Meshs[i].texID, filename.c_str()));
						SOIL_free_image_data(data);
					}
					else {
						printf("nao pode criar textura (%s),inserindo textura padrao\n", filename.c_str());
						unsigned char tdefault[] = { 0, 0, 0, 255 };
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdefault);
						printf("%s\n", SOIL_last_result());
						Meshs[i].texID = 0;
					}
				}
			}
		}
	}
}

GLuint ObjLoader::GetTotalFaces()
{
	GLuint faces = 0;
	for (size_t i = 0; i < Meshs.size(); i++)
	{
		faces += Meshs[i].faces.size();
	}
	return (GLuint)faces;
}

GLuint ObjLoader::GetTotalVertexCoord()
{
	GLuint vertexCoord = 0;
	for (size_t i = 0; i < Meshs.size(); i++)
	{
		vertexCoord += Meshs[i].vCoords.size();
	}
	return (GLuint)vertexCoord;
}
GLuint ObjLoader::GetTotalTextCoord()
{
	GLuint textCoords = 0;
	for (size_t i = 0; i < Meshs.size(); i++)
	{
		textCoords += Meshs[i].tCoords.size();
	}
	return (GLuint)textCoords;
}
GLuint ObjLoader::GetTotalNormCoord()
{
	GLuint nCoords = 0;
	for (size_t i = 0; i < Meshs.size(); i++)
	{
		nCoords += Meshs[i].nCoords.size();
	}
	return (GLuint)nCoords;
}

void ObjLoader::ParseFile()
{
	char line[2048];
	char name[256];
	int count = 0;
	int TotalFaces = 0, TotalVert = 0, TotalText = 0, TotalNorm = 0;

	vec3f temp(0.0f);
	vec3i tempi;
	Face tFace; //faces;
	int res = 0;
	int verticeT = 0, texT = 0, normalT = 0;
	char* aux;

	int nline = 0;

	while (fgets(line, 2048, m_arq) != NULL)
	{
		nline++;
		if (sscanf(line, "o %s", name) != NULL)
		{
			if (name[0] != 0)
			{
				Mesh mesh(name);
				Meshs.push_back(mesh);
				printf("object: %s\n", name);
				name[0] = 0;
			}
			count++;
		}
		else if(sscanf(line,"usemtl %[^\n]s", name) == 1) {
			if (name[0] != 0)
			{
				if (Meshs.size() == 0)
				{
					Mesh mesh("default");
					Meshs.push_back(mesh);
					printf("default name\n");
				}
				printf("usa materia %s\n", name);
				Meshs.back().materialName = name;
			}
		}
		else if (sscanf(line, "mtllib %[^\n]s", name))
		{
			if (name[0] != 0)
			{
				
				materialFileName = name;
				ParseMaterial();
			}
		}
		else if (sscanf(line, "v %f %f %f", &temp.x, &temp.y , &temp.z)) {
			//printf("v %f %f %f\n", temp.x, temp.y, temp.z);
			if (Meshs.size() == 0)
			{
				Mesh mesh("default");
				Meshs.push_back(mesh);
				printf("default name\n");
			}
			Meshs.back().vCoords.push_back(temp);
			TotalVert++;
		}
		else if (sscanf(line, "vt %f %f", &temp.x, &temp.y)) {
			temp.z = 0.0f;
			//printf("vt %f %f %f\n", temp.x, temp.y, temp.z);
			Meshs.back().tCoords.push_back(temp);
			TotalText++;
		}
		else if (sscanf(line, "vn %f %f %f", &temp.x, &temp.y, &temp.z)) {
			//printf("vn %f %f %f\n", temp.x, temp.y, temp.z);
			Meshs.back().nCoords.push_back(temp);
			TotalNorm++;
		}
		else if (strstr(line, "f ")) {
			
			aux = strtok(line, " ");
			aux = strtok(NULL, " ");
			TotalFaces++;
			tFace.index.clear();
			verticeT = texT = normalT = 0;
			for (unsigned int i = 0; i < Meshs.size() - 1; i++)
			{
				verticeT += Meshs[i].vCoords.size();
				texT += Meshs[i].tCoords.size();
				normalT += Meshs[i].nCoords.size();
			}
			while (aux)
			{
				tempi.clear();
				if (sscanf(aux, "%d//%d", &tempi.x, &tempi.z) == 2);
				else if (sscanf(aux, "%d/%d/%d", &tempi.x, &tempi.y, &tempi.z) == 3);
				else if (sscanf(aux, "%d/%d", &tempi.x, &tempi.y) == 2);
				else {
					printf("erro em %s %d\n", aux, nline);
					//exit(0);
				}
				tempi.x -= (verticeT);
				tempi.y -= (texT);
				tempi.z -= (normalT);
				tFace.index.push_back(tempi);
				aux = strtok(NULL, " ");
			}
			//printf("\n");
			if(tFace.index.size() > 0)
				Meshs.back().faces.push_back(tFace);
		}
	}

	printf("objeto carregado\nMeshs: %d\nFaces: %d\nVertices: %d\nTextureV: %d\nnVert: %d\n",
				Meshs.size(), TotalFaces, TotalVert, TotalText, TotalNorm);
	/*for(int i = 0; i < Meshs.size(); i++)
	{
		printf("%s\n", Meshs[i].objname.c_str());
		for (int j = 0; j < Meshs[i].faces.size(); j++)
		{
			for (int k = 0; k < Meshs[i].faces[j].index.size(); k++)
			{
				printf("%d %d %d ", Meshs[i].faces[j].index[k].x, Meshs[i].faces[j].index[k].y, Meshs[i].faces[j].index[k].z);
			}
			printf("\n");
		}
	}*/
}

void ObjLoader::ParseMaterial()
{
	std::string MaterialPath = m_filename.substr(0, m_filename.rfind("/")+1) + materialFileName.c_str();
	FILE* arq = fopen(MaterialPath.c_str(), "r");
	char line[256];
	char aux[256];
	int res = 0;
	
	printf("Material Parsing: %s\n", MaterialPath.c_str());
	if (arq) {
		while (fgets(line, 256, arq) != NULL)
		{
			printf("%s", line);
			if (sscanf(line, "newmtl %[^\n]s", aux))
			{
				if (aux[0] != 0) {
					printf("adciona %s\n", aux);
					materials.push_back(Material());
					materials.back().name = aux;
				}
			}
			else if (strstr(line, "map_Kd ")) {
				res = sscanf(line, "map_Kd %[^\n]s", aux);
				if (aux[0] != 0) {
					materials.back().mapDifuse = aux;
				}
			}
			else if (strstr(line, "Ns ")) {
				res = sscanf(line, "Ns %f", &materials.back().SpecularDensity);
				printf("spec %.2f\n", materials.back().SpecularDensity);
			}
			else if (strstr(line, "Ka ")) {
				res = sscanf(line, "Ka %f %f %f", &materials.back().ambient.x, &materials.back().ambient.y, &materials.back().ambient.z);
			}
			else if (strstr(line, "Kd ")) {
				res = sscanf(line, "Kd %f %f %f", &materials.back().difuse.x, &materials.back().difuse.y, &materials.back().difuse.z);
			}
			else if (strstr(line, "Ks ")) {
				res = sscanf(line, "Ks %f %f %f", &materials.back().specular.x, &materials.back().specular.y, &materials.back().specular.z);
			}
			else if (strstr(line, "Ke ")) {
				res = sscanf(line, "Ke %f %f %f", &materials.back().emissive.x, &materials.back().emissive.y, &materials.back().emissive.z);
			}
		}
	}
	else {
		printf("%s não encontrado\n", MaterialPath.c_str());
	}

}
