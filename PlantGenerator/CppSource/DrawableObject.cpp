#include "Definitions.h"
#include "TurtleGraphics.h"
#include "DrawableObject.h"


DrawableObject::DrawableObject(char letter, const Colorf& baseColor) : m_letter(letter), m_baseColor(baseColor)
{
	m_vertices.push_back(Vector4f(1.0f,2.0f,3.0f,4.0f));
	m_vertices.push_back(Vector4f(5.0f,6.0f,7.0f,8.0f));
	float* vert  = m_vertices.data()->getValuePtr();

	for(int i = 0; i < 8; i++)
	{
		vert++;
	}
}

bool DrawableObject::draw(const PaintState& state)
{
	float length = state.LineLength.getValue();
	glLineWidth(state.LineWidth.getValue());

	glUseProgram(m_shaderProgram);
    checkGlError("glUseProgram");

	glUniformMatrix4fv(m_modelViewHandle, 1, GL_FALSE,( state.ModelView * Matrix4f::Scale(length, length, length)).getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glVertexAttribPointer(m_positionHandle, 4, GL_FLOAT, GL_FALSE, 0, m_vertices.data());
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(m_positionHandle);
    checkGlError("glEnableVertexAttribArray");

	glDrawArrays(GL_LINE_STRIP, 0, 2);
    checkGlError("glDrawArrays");
	return true;
}
DrawableObject::~DrawableObject()
{
}
