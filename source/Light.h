
#pragma once

class Light
{
public:
	Light();
	~Light();

	void Draw();
	float m_lightDifuse[4];
	float m_lightAmbient[4];
	float m_lightSpecular[4];
	float m_lightPosition[4];
	float m_lightDir[4];
	float m_lightSpot;
	bool m_bLighting, m_bLanternMode;
private:
};