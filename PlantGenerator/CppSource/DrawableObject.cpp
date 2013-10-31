#include "Definitions.h"
#include"Color.h"
#include"Vector.h"
#include "Matrix.h"
#include "PaintState.h"
#include "DrawableObject.h"

DrawableObject::DrawableObject() : m_letter(' '), m_baseColor(0,0,0,1) 
{
}


DrawableObject::DrawableObject(char letter, const Colorf& baseColor, float width, uint shader) : m_letter(letter), m_baseColor(baseColor)
{
	m_vertices.push_back(Vector4f(-0.5f,0.0f, 0.0f, 1.0f));
	m_vertices.push_back(Vector4f(0.5f,0.0f, 0.0f, 1.0f));
	m_vertices.push_back(Vector4f(-0.5f, 1.0f, 0.0f, 1.0f));
	m_vertices.push_back(Vector4f(0.5f, 1.0f, 0.0f, 1.0f));
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
bool DrawableObject::draw(PaintState& state)
{
	glUseProgram(m_shaderProgram);
    checkGlError("glUseProgram");

	float length = state.LineLength.getValue();
	glUniformMatrix4fv(m_modelViewHandle, 1, GL_FALSE, 
		( state.ModelView * Matrix4f::Scale(state.LineWidth.getValue(), length, 1.0f)).getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glUniform4fv(m_colorHandle,1.0f,m_baseColor.getValuePtr());
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
			(*it)[0] = -width / 2.0f;
			(*(++it))[0] = width / 2.0f; 
			break;
		}
		else if((*it)[1] > pos)  { insert = true; break; }
	}
	if(insert)
	{
		it = m_vertices.insert(it,Vector4f(width / 2.0f, pos, 0.0f,1.0f));
		m_vertices.insert(it,Vector4f(-width / 2.0f, pos, 0.0f,1.0f));
	}
}

DrawableObject::~DrawableObject()
{
}
