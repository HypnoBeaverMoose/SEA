#pragma once

class IDrawableObject
{
public: 
	virtual bool draw(const PaintState&) = 0;
};

class DrawableObject : public IDrawableObject
{
public:
	DrawableObject(char letter, const Colorf& baseColor);

	virtual bool draw(const PaintState& state);

	void setShader(uint shader) { m_shaderProgram = shader; }

	~DrawableObject();
private:
	uint					m_colorHandle;
	uint					m_positionHandle;	
	uint					m_modelViewHandle;
	uint					m_shaderProgram;
	char					m_letter;
//	std::vector<Vector4f>	m_vertices;
	std::vector<float>	m_vertices;
	Colorf					m_baseColor;
	
};

