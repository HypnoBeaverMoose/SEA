#pragma once
//
//class IDrawableObject
//{
//public: 
//	virtual bool draw(const PaintState&) = 0;
//};
typedef png::image<png::rgba_pixel> rgbaImage;
class DrawableObject// : public IDrawableObject
{
public:
	friend DrawableObject CombineObjects( const DrawableObject& lhs, const DrawableObject& rhs, float bias);
	
	static void CombineTextures(const rgbaImage& lhs, const rgbaImage& rhs, rgbaImage& result, float  bias);

	DrawableObject();
	
	DrawableObject(char letter, const Colorf& baseColor = Colorf(1.0f,1.0f, 1.0f), float width = 0.0f, uint shader = 0, float offset = 0.0f, int dir  = 1);

	DrawableObject(char letter, const Colorf& baseColor, const png::image<png::rgba_pixel>& texture, float width, uint shader, float offset = 0, int dir  = 1);

	DrawableObject(char letter, const Colorf& baseColor, const png::image<png::rgba_pixel>& texture, float width, float offset, int dir  = 1);

	virtual bool draw(PaintState& state) const;

	void setWdith(float pos, float width);

	void setShader(uint shader);

	float  getWidth(float pos) const;

	void setLetter(char letter) { m_letter = letter; }

	char getLetter() const { return m_letter; }

	~DrawableObject();

private:
	void initVerticies(float width);
	void prepareDefaultTexture();
private:
	static rgbaImage	s_defaultTexture;
	static uint			s_defaultTexId;
private:
	rgbaImage				m_texture;
	std::vector<Vector4f>	m_vertices;
	std::vector<Vector2f>	m_textureCoords;
	uint					m_colorHandle;
	uint					m_positionHandle;	
	uint					m_textureCoordsHandle;
	uint					m_modelViewHandle;
	uint					m_shaderProgram;		
	Colorf					m_baseColor;
	char					m_letter;
	float					m_width;
	float					m_height;
	float					m_verticalOffset;
	uint					m_textureId;
	int						m_direction;
};

