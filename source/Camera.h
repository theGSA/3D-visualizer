
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
	static void WheelEvent(int b, int action, int x, int y);
	static void SetRotate(bool _rotate) {		rotate = _rotate; }
	static void SetRotateVelocity(float rv) { RotateVelocity = rv; }
	static bool rotate;
	static float RotateVelocity;
	static float tx, ty, tz;
	static float rx, ry, rz;
	static float upx, upy, upz;
private:

};
