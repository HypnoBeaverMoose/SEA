#pragma once
//
//class IDrawableObject
//{
//public: 
//	virtual bool draw(const PaintState&) = 0;
//};

class DrawableObject// : public IDrawableObject
{
public:

	friend DrawableObject CombineObjects( const DrawableObject& lhs, const DrawableObject& rhs, float bias);

	DrawableObject();
	
	DrawableObject(char letter, const Colorf& baseColor, float width, uint shader, float offset = 0);

	virtual bool draw(PaintState& state) const;

	void setWdith(float pos, float width);

	void setShader(uint shader);

	float  getWidth(float pos) const;

	void setLetter(char letter) { m_letter = letter; }

	char getLetter() const { return m_letter; }

	~DrawableObject();
private:
	uint					m_colorHandle;
	uint					m_positionHandle;	
	uint					m_modelViewHandle;
	uint					m_shaderProgram;		
	Colorf					m_baseColor;
	std::vector<Vector4f>	m_vertices;
	char					m_letter;
	float					m_width;
	float					m_height;
	float					m_verticalOffset;
};

