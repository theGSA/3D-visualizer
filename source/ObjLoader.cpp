#include <Windows.h>
#include <GL/freeglut.h>
#include <SOIL2.h>
#include "stb_image.h"
#include "ObjLoader.h"
#include "vec3.h"


ObjLoader::ObjLoader(std::string filename):
m_filename(filename),
nVertices(0),
nTex(0),
nNormals(0)
{
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
	char path[256] = "objects\\texturas\\textures.png";

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	/* if (!data)
		 data = stbi_dds_load(temp, &width, &height, &nrChannels, 0);*/

	if (data)
	{
		GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;
		printf("criando textura(%s)(texID: %d)\n", path, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	else {
		printf("nao pode criar textura (%s),inserindo textura padrao\n", path);
		unsigned char tdefault[] = { 0, 254, 254 };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, tdefault);
		printf("%s\n", SOIL_last_result());
	}

	
	//glGenerateMipmap(GL_TEXTURE_2D);
	//m_tbo.push_back(texbo);

	SOIL_free_image_data(data);
}

void ObjLoader::ParseFile()
{
	char line[256];
	char name[256];
	int count = 0;
	
	vec3f temp(0.0f);
	vec3i tempi;
	Face tFace; //faces;
	int res = 0;
	int verticeT, texT, normalT;
	char* aux;

	while (fgets(line, 256, m_arq) != NULL)
	{
		
		if (sscanf(line, "o %s", name) != NULL)
		{
			if (name[0] != 0)
			{
				Mesh mesh(name);
				Meshs.push_back(mesh);
				printf("object: %s\n", name);
			}
			count++;
		}
		else if (sscanf(line, "mtllib %s", &materialFileName[0]))
		{
			ParseMaterial();
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
		}
		else if (sscanf(line, "vt %f %f", &temp.x, &temp.y)) {
			temp.z = 0.0f;
			//printf("vt %f %f %f\n", temp.x, temp.y, temp.z);
			Meshs.back().tCoords.push_back(temp);
		}
		else if (sscanf(line, "vn %f %f %f", &temp.x, &temp.y, &temp.z)) {
			//printf("vn %f %f %f\n", temp.x, temp.y, temp.z);
			Meshs.back().nCoords.push_back(temp);
		}
		else if (strstr(line, "f ")) {
			
			aux = strtok(line, " ");
			aux = strtok(NULL, " ");
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
				if (strstr(aux,"//")) {
					char r = sscanf(aux, "%d//%d", &tempi.x, &tempi.z);
				}
				else {
					char r = sscanf(aux, "%d/%d/%d", &tempi.x, &tempi.y, &tempi.z);
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
	printf("objeto carregado\n");
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
	std::string MaterialPath = m_filename.substr(0, m_filename.rfind("/")+1) + materialFileName;
	FILE* arq = fopen(MaterialPath.c_str(), "r");
	char line[256];
	char aux[256];
	int res = 0;
	printf("parssing %s\n", MaterialPath.c_str());
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
			else if (strstr(line, "Ns ")) {
				res = sscanf(line, "Ns %f", &materials.back().SpecularDensity);
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
			else if (strstr(line, "map_Kd ")) {
				res = sscanf(line, "map_Kd %s", &materials.back().mapDifuse[0]);
			}
		}

		
	}
	else {
		printf("%s não encontrado\n", MaterialPath.c_str());
	}

}
