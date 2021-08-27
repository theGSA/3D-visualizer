#pragma once
#include <string>
#include <vector>
#include <stdlib.h>
#include "vec3.h"
#include "Material.h"

class Face
{
public:
	Face() {};
	~Face() {};
	std::vector<vec3i> index;
};

class Mesh
{
public:
	Mesh() {};
	Mesh(std::string strName) { name = strName; };
	~Mesh() {};
	std::vector<vec3f> vCoords, tCoords, nCoords;
	std::vector<Face> faces;
	std::string name;
};

class ObjLoader
{
public:
	ObjLoader(std::string filename);
	~ObjLoader();

	std::vector<Mesh> Meshs;
	unsigned texID;
private:
	void CreateTexture();
	void ParseFile();
	void ParseMaterial();
	std::string m_filename;
	std::string materialFileName;
	std::vector<Material> materials;

	FILE* m_arq;
	int nVertices, nNormals, nTex;
};
