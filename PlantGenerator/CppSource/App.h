#pragma once
#include "LSystem.h"
#include "Matrix.h"
#include "PaintState.h"
#include "DrawableObject.h"
#include "Plant.h"
#include "TurtleGraphics.h"

class App
{
public:
	App();
	
	virtual void OnCreate();

	virtual void OnRender();

	virtual void OnResize(int width, int height);

	virtual void OnTouch(int posx, int posy);

	//virtual void OnKeybaord(uint key);

	virtual void OnDestroy();

	virtual ~App();

public:
	static uint createProgram(const char* pVertexSource, const char* pFragmentSource);
	static uint loadShader(uint shaderType, const char* pSource);

protected:	
	std::vector<Plant> m_plants;
	TurtleGraphics	m_painter;
	Matrix4f m_projectionMatrix;
	//std::vector<float> m_triangle;
	//LSystem		m_lSystem;
	//Plant			m_plant;

private:
	void SetUpPlant();
	bool  needsRedraw;
	static char* s_VertexShader;
	static char* s_FragmentShader;
	float m_bias;
	uint m_programId;
	//uint m_positionHandle;
	//uint m_modelViewHandle;
	//uint m_projectionHandle;
};