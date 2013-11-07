#include "Definitions.h"
#include"Color.h"
#include"Vector.h"
#include "Matrix.h"
#include "PaintState.h"
#include "DrawableObject.h"

DrawableObject::DrawableObject() : m_letter(' '), m_baseColor(0,0,0,1) 
{
}


DrawableObject::DrawableObject(char letter, const Colorf& baseColor, float width, uint shader, float offset) 
	: m_letter(letter), m_baseColor(baseColor),m_width(width), m_verticalOffset(offset)
{
	m_vertices.push_back(Vector4f(-0.5f * width,0.0f, 0.0f, 1.0f));
	m_vertices.push_back(Vector4f(0.5f * width,0.0f, 0.0f, 1.0f));
	m_vertices.push_back(Vector4f(-0.5f * width, 1.0f, 0.0f, 1.0f));
	m_vertices.push_back(Vector4f(0.5f * width, 1.0f, 0.0f, 1.0f));
	setShader(shader);
}
void DrawableObject::setShader(uint shader)
{
	m_shaderProgram = shader;
	m_positionHandle = glGetAttribLocation(m_shaderProgram, "vPosition");
	checkGlError("glGetAttribLocation");
	m_modelViewHandle = glGetUniformLocation(m_shaderProgram,"mModelView");
	checkGlError("glGetUniformLocation");
	m_colorHandle = glGetUniformLocation(m_shaderProgram,"vColor");
	checkGlError("glGetUniformLocation");
    glEnableVertexAttribArray(m_positionHandle);
    checkGlError("glEnableVertexAttribArray");

}
bool DrawableObject::draw(PaintState& state) const
{
	glUseProgram(m_shaderProgram);
    checkGlError("glUseProgram");

	float length = state.LineLength.getValue();
	Matrix4f mat = state.ModelView * Matrix4f::Scale(state.LineWidth.getValue(), length, 1.0f);
	mat *=  Matrix4f::Translation(0,-m_verticalOffset,0).Transposed();
	glUniformMatrix4fv(m_modelViewHandle, 1, GL_FALSE,  mat.getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glUniform4fv(m_colorHandle, 1, m_baseColor.getValuePtr());
    checkGlError("glUniform1fv");

	glVertexAttribPointer(m_positionHandle, 4, GL_FLOAT, GL_FALSE, m_vertices[0].stride(), m_vertices.data());
    checkGlError("glVertexAttribPointer");
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertices.size());
    checkGlError("glDrawArrays");

	state.ModelView *= Matrix4f::Translation(0,length,0).Transposed();
	
	return true;
}

void DrawableObject::setWdith(float pos, float width)
{
	if(pos < 0.0f || pos > 1.0f || width < 0.0f) return;
	bool insert = false;
	std::vector<Vector4f>::iterator it = m_vertices.begin();
	for(; it!=m_vertices.end(); ++it)
	{
		if((*it)[1] == pos)
		{
			(*it)[0] = m_width * (-width / 2.0f);
			(*(++it))[0] = m_width * (width / 2.0f); 
			break;
		}
		else if((*it)[1] > pos)  { insert = true; break; }
	}
	if(insert)
	{
		it = m_vertices.insert(it,Vector4f (m_width*(width / 2.0f), pos, 0.0f,1.0f));
		m_vertices.insert(it,Vector4f(m_width * (-width / 2.0f), pos, 0.0f,1.0f));
	}
}

DrawableObject::~DrawableObject()
{
}

float DrawableObject::getWidth(float pos) const
{
	if(pos < 0.0f || pos > 1.0f) return -1;

	uint i  = 0; 
	for(; i < m_vertices.size(); i+=2){
		if( pos < m_vertices[i][1]) 
			break;
	}

	if(i == (m_vertices.size()))
		i-= 2;

	float len = (m_vertices[i + 1][1] - m_vertices[i -1][1]);
	float t = (pos - m_vertices[i - 1][1]) / len;

	return (1 - t)  * m_vertices[i - 1][0] + t * m_vertices[i + 1][0];
}

DrawableObject CombineObjects( const DrawableObject& lhs, const DrawableObject& rhs, float bias)
{
	Colorf color(	lhs.m_baseColor[0] * (1.0f - bias) + rhs.m_baseColor[0] * bias,
						lhs.m_baseColor[1] * (1.0f - bias) + rhs.m_baseColor[1] * bias,
							lhs.m_baseColor[2] * (1.0f - bias) + rhs.m_baseColor[2] * bias,
								1.0f);

	///we assume that the letters of the objects are unique
	DrawableObject obj(	lhs.getLetter(), color,
						lhs.m_width * (1.0f - bias) + rhs.m_width * bias,
						lhs.m_shaderProgram,lhs.m_verticalOffset * ( 1.0f - bias) + rhs.m_verticalOffset * bias );

	const DrawableObject& first = lhs.m_vertices.size() > rhs.m_vertices.size() ? lhs : rhs;
	const DrawableObject& second = lhs.m_vertices.size() < rhs.m_vertices.size() ? lhs : rhs;
	for(uint i = 0; i < first.m_vertices.size(); i+=2)
	{
		float pos = first.m_vertices[i][1];
		float width1 = first.m_vertices[i+1][0] / first.m_width;
		float width2 = second.getWidth(pos) / second.m_width;
		obj.setWdith(pos, 2 * ( width1 * (1.0f - bias) + width2 * bias));
	}
	return obj;
}