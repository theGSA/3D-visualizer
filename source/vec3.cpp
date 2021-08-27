#include "vec3.h"
#include <math.h>

vec3f::vec3f(float value):
	x(value),
	y(value),
	z(value)
{
}

vec3f::vec3f(float x, float y, float z):
	x(x),
	y(y),
	z(z)
{
}

vec3f::~vec3f()
{
}

vec3f vec3f::normalize()
{
	float norm = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
	return vec3f(x/norm, y/norm, z/norm);
}

vec3i::vec3i() :
	x(0),
	y(0),
	z(0)
{
}

vec3i::~vec3i()
{
}

vec3i::vec3i(int x, int y, int z) :
	x(x),
	y(y),
	z(z)
{

}
