#include "Includes.h"
#include "Definitions.h"
#include "TurtleGraphics.h"
#include "App.h"
float TurtleGraphics::s_initialLineLength = 1.0f;

char* TurtleGraphics::s_VertexShader = 
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "}\n";

char* TurtleGraphics::s_FragmentShader = 
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
    "}\n";


TurtleGraphics::TurtleGraphics(float angle, float length) : m_angle(angle), m_length(length)
{
	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(0.0f);

	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(s_initialLineLength);
	m_linePositions.push_back(0.0f);
	m_program = App::createProgram(s_VertexShader, s_FragmentShader);
	m_positionHandle = glGetAttribLocation(m_program, "vPosition");

}
bool TurtleGraphics::drawLine()
{
	glUseProgram(m_program);
    checkGlError("glUseProgram");

	glVertexAttribPointer(m_positionHandle, 3, GL_FLOAT, GL_FALSE, 0, m_linePositions.data());
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(m_positionHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_LINES, 0, 2);
    checkGlError("glDrawArrays");
}

bool TurtleGraphics::drawLSystem(const char* system, int size)
{
	return true;
}


TurtleGraphics::~TurtleGraphics(void)
{
}

float TurtleGraphics::initialLineLength()
{
	return s_initialLineLength;
}