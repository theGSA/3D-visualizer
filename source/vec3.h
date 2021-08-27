#pragma once
#ifndef _VEC3_H
#define _VEC3_h

class vec3f
{
public:
	vec3f() { x = y = z = 0.0f;  }
	vec3f(float value);
	vec3f(float x, float y, float z);
	~vec3f();
	vec3f normalize();
	float x, y, z;
private:

};

class vec3i
{
public:
	vec3i();
	~vec3i();
	vec3i(int x, int y, int z);
	void clear() { x = y = z = 0; }
	int x, y, z;
private:

};
#endif // !_VEC3_H

