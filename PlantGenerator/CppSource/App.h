#pragma once
#include "LSystem.h"
#include "Matrix.h"
#include "TurtleGraphics.h"

class App
{
public:
	App();
	
	virtual void OnCreate();

	virtual void OnRender();

	virtual void OnResize(int width, int height);

	virtual void OnTouch(int posx, int posy);

	virtual void OnDestroy();

	virtual ~App();

public:
	static uint createProgram(const char* pVertexSource, const char* pFragmentSource);
	static uint loadShader(uint shaderType, const char* pSource);

protected:
	LSystem			m_lSystem;
	TurtleGraphics	m_painter;
	Matrix4f	m_projectionMatrix;
	//Matrix4f m_modelViewMatrix;
	//std::vector<float> m_triangle;

private:

	//static char* s_VertexShader;
	//static char* s_FragmentShader;

	//uint m_programId;
	//uint m_positionHandle;
	//uint m_modelViewHandle;
	//uint m_projectionHandle;

};