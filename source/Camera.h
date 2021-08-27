
#pragma once

class Camera
{
public:
	Camera();
	~Camera();
	static void Update();
	static void KeyBoardEvent(unsigned char c, int x, int y);
	static void MouseEvent(int b, int action, int x, int y);
	static void Motion(int x, int y);
private:
	static float tx, ty, tz;
	static float rx, ry, rz;

};
