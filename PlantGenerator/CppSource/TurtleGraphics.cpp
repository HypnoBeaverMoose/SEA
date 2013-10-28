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
	m_angle(angle - 10,angle + 10), m_length(length - 0.02f, length + 0.02f), m_modelviewMatrix(Matrix4f(Matrix4f::Identity()))
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

bool TurtleGraphics::drawLine(float length)
{
	glUseProgram(m_program);
    checkGlError("glUseProgram");

	glUniformMatrix4fv(m_modelviewHandle, 1, GL_FALSE,( m_modelviewMatrix * Matrix4f::Scale(length, length, length)) .getValuePtr());
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
	m_modelviewMatrix = Matrix4f::Translation(0,-10.0,0).Transposed();
	float length = m_length.getValue();

	for(int i = 0; i < size; i++)
	{
		switch (system[i])
		{
		case 'F':						
			drawLine(length);
			m_modelviewMatrix =m_modelviewMatrix *  Matrix4f::Translation(0,length,0).Transposed();
			break;
		case '+':
			m_modelviewMatrix = m_modelviewMatrix * Matrix4f::Rotation(m_angle.getValue(), Vector3f(0, 0, 1.0f));
			break;
		case '-':
			m_modelviewMatrix = m_modelviewMatrix * Matrix4f::Rotation(-m_angle.getValue(), Vector3f(0, 0, 1.0f));
			break;
		case '[':
			m_MatrixStack.push(m_modelviewMatrix); 
			break;
		case ']':
			m_modelviewMatrix = m_MatrixStack.top();
			m_MatrixStack.pop();
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
///////////RandomValue Class////////////////

//RandomValue::RandomValue(float mean, float variance, Distribution dist) 
//	: m_mean(mean), m_variance(variance), m_distribution(dist)
//{}
	
RandomValue::RandomValue(float min, float max, Distribution dist) : m_distribution(dist)
{
	m_mean = (max + min) / 2.0f;
	m_variance = m_mean	- min;
}

float RandomValue::getValue(Distribution override)
{
	Distribution dist = override == None ? m_distribution : override;
	float rand = rand01();

	if(dist == Uniform)
		return randomUniform(rand);
	else 
		return randomGaussian(rand);
}

inline float RandomValue::randomUniform(float rand)
{
	return m_mean + m_variance * ( 2 * rand - 1.0f);
}

float RandomValue::randomGaussian(float rand)
{
	float r = (1.0f / (0.5f * std::sqrtf(2 * PI)) * std::expf( - (rand  - 0.5f) / 2 * 0.25f));
	return randomUniform(r);
}