#pragma once
#include "LSystem.h"
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
	LSystem m_lSystem;

};