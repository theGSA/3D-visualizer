#pragma once
#include <vector>
#include <string>
#include "vec3.h"

class Material
{
public:
	Material();
	~Material();
	std::string name;
	std::string mapDifuse;
	vec3f ambient;
	vec3f difuse;
	vec3f specular;
	float SpecularDensity;
	vec3f emissive;
private:
};

