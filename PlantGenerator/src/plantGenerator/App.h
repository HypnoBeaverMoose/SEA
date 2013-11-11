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
	static bool loadImageFromFile(png::image<png::rgba_pixel>& image, const char* filename);
	static App* const getInstance(); 

protected:	
	App();

	std::vector<Plant> m_plants;
	TurtleGraphics	m_painter;
	Matrix4f m_projectionMatrix;
	virtual bool loadImage(png::image<png::rgba_pixel>& image, const char* filename);
	static App* s_instance;
private:
	void SetUpPlant();
	bool  needsRedraw;
	static char* s_VertexShader;
	static char* s_FragmentShader;
	float m_bias;
	uint m_programId;
};