#pragma once
#include "LSystem.h"
#include "Matrix.h"
#include "PaintState.h"
#include "DrawableObject.h"
#include "Plant.h"
#include "TurtleGraphics.h"
#include "PlantDatabase.h"

class App
{
public:
	virtual void OnCreate();

	virtual void OnRender();

	virtual void OnResize(int width, int height);

	virtual void OnTouch(int posx, int posy);

	virtual void OnDestroy();

	void setRenderSize(int width, int height);

	byte* getPlantImage(uint& width, uint& height);

	void SetDefaultBiases(float leaves, float stalk, float flowers);

	void loadPlant(PlantDatabase::PlantData plant, int index);

	virtual ~App();

public:
	static uint createProgram(const char* pVertexSource, const char* pFragmentSource);
	static uint loadShader(uint shaderType, const char* pSource);
	static bool loadImageFromFile(png::image<png::rgba_pixel>& image, const char* filename);
	static App* const getInstance(); 
	virtual void RenderPlant();
protected:	
	App();
	
	virtual bool loadImage(png::image<png::rgba_pixel>& image, const char* filename);

public:
	Vector2f				m_renderSize;
	float					m_leafBias;
	float					m_flowerBias;
	float					m_stalkBias;
	std::vector<Plant>		m_plants;

protected:	
	std::auto_ptr<Plant>	m_resultPlant;
	TurtleGraphics			m_painter;
	Matrix4f				m_projectionMatrix;
	static App*				s_instance;


private:
	void SetUpPlant();

private:	
	bool					needsRedraw;
	static char*			s_VertexShader;
	static char*			s_FragmentShader;
	uint					m_programId;
	float					m_bias;
	uint					m_framebufferHandle;
	uint					m_targetTexHandle;
	uint					m_previewTexHandle;
	Vector2f				m_viewportSize;
	std::vector<Vector4f>	m_renderQuad;
	std::vector<Vector2f>	m_renderUV;
	rgbaImage				m_defaultTexture;
	uint					m_colorHandle;
	uint					m_positionHandle;	
	uint					m_textureCoordsHandle;
	uint					m_modelViewHandle;

};