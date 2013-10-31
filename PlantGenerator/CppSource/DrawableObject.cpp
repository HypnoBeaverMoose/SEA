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
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGlError("glDrawArrays");

	state.ModelView *= Matrix4f::Translation(0,length,0).Transposed();
	
	return true;
}
DrawableObject::~DrawableObject()
{
}
