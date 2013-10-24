#include "Includes.h"
#include "Definitions.h"
#include "TurtleGraphics.h"
#include "LSystem.h"
#include "App.h"


float TurtleGraphics::s_initialLineLength = 1.0f;

char* TurtleGraphics::s_VertexShader = 
    "attribute vec4 vPosition;\n"
	"uniform mat4 mModelView;"
	"uniform mat4 mProjection;"
    "void main() {\n"
	"  gl_Position = mProjection * mModelView * vPosition;\n"
    "}\n";

char* TurtleGraphics::s_FragmentShader = 
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
    "}\n";

TurtleGraphics::TurtleGraphics(float angle, float length) : 
	m_angle(angle), m_length(length), m_modelviewMatrix(Matrix4f(Matrix4f::Identity()))
{
	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(0.0f);

	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(s_initialLineLength);
	m_linePositions.push_back(0.0f);
}
void TurtleGraphics::init()
{
	m_program = App::createProgram(s_VertexShader, s_FragmentShader);
	m_positionHandle = glGetAttribLocation(m_program, "vPosition");
	m_modelviewHandle = glGetUniformLocation(m_program,"mModelView");
}

bool TurtleGraphics::drawLine()
{
	glUseProgram(m_program);
    checkGlError("glUseProgram");

	glUniformMatrix4fv(m_modelviewHandle, 1, GL_FALSE,( m_modelviewMatrix * Matrix4f::Scale(m_length, m_length, m_length)) .getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glVertexAttribPointer(m_positionHandle, 3, GL_FLOAT, GL_FALSE, 0, m_linePositions.data());
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(m_positionHandle);
    checkGlError("glEnableVertexAttribArray");

	glDrawArrays(GL_LINE_STRIP, 0, 2);
    checkGlError("glDrawArrays");
	
	return true;
}

bool TurtleGraphics::drawLSystem(const char* system, int size)
{
	m_modelviewMatrix = Matrix4f::Identity();
	char* syst = "F-F-F-F";
	for(int i = 0; i < size; i++)
	{
		switch (system[i])
		{
		case 'F':			
			drawLine();
			m_modelviewMatrix =m_modelviewMatrix *  Matrix4f::Translation(0,m_length,0).Transposed();
			break;

		case '+':
			m_modelviewMatrix = m_modelviewMatrix * Matrix4f::Rotation(m_angle, Vector3f(0, 0, 1.0f));
			break;

		case '-':
			m_modelviewMatrix = m_modelviewMatrix * Matrix4f::Rotation(-m_angle, Vector3f(0, 0, 1.0f));
			break;
		}
	}
	
	return true;
}


TurtleGraphics::~TurtleGraphics(void)
{
}

float TurtleGraphics::initialLineLength()
{
	return s_initialLineLength;
}