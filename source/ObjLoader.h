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
	Mesh():texID(0), materialID(-1) {};
	Mesh(std::string strName) { name = strName; };
	~Mesh() {};
	std::vector<vec3f> vCoords, tCoords, nCoords;
	std::vector<Face> faces;
	std::string name;
	std::string materialName;
	unsigned texID;
	int materialID;
};

class ObjLoader
{
public:
	ObjLoader(std::string filename);
	~ObjLoader();

	std::vector<Mesh> Meshs;
	unsigned texID;
	GLuint GetTotalMeshs() { return Meshs.size(); };
	GLuint GetTotalFaces();
	GLuint GetTotalVertexCoord();
	GLuint GetTotalTextCoord();
	GLuint GetTotalNormCoord();
	bool m_bRotate;
	float m_fRotateVel;
	float m_fAngleRotate;
	
	std::vector<Material> materials;
private:
	void CreateTexture();
	void ParseFile();
	void ParseMaterial();
	std::string m_filename;
	std::string materialFileName;

	FILE* m_arq;
	int nVertices, nNormals, nTex;
};
