#include "Definitions.h"
#include "TurtleGraphics.h"
#include "DrawableObject.h"
#include "LSystem.h"
#include "App.h"

float TurtleGraphics::s_initialLineLength = 1.0f;	
float TurtleGraphics::s_lengthModifier = 0.1f;
float TurtleGraphics::s_widthModifier = 1.0f;
float TurtleGraphics::s_angleModifier = 5.0f;


char* TurtleGraphics::s_VertexShader = 
    "attribute vec4 vPosition;\n"
	"uniform mat4 mModelView;\n"
	"uniform mat4 mProjection;\n"
    "void main() {\n"
	"  gl_Position = mProjection * mModelView * vec4(vPosition, 1.0f);\n"
    "}\n";

char* TurtleGraphics::s_FragmentShader = 
    "precision mediump float;\n"
	"uniform vec4 vColor;\n"
    "void main() {\n"
    "  gl_FragColor = vColor;//vec4(0.0, 0.0, 0.0, 1.0);\n"
    "}\n";


TurtleGraphics::TurtleGraphics(float minAngle, float maxAngle, float minLength, float maxLength)// : 
	//m_angle(angle - 10,angle + 10), m_length(length - 0.02f, length + 0.02f)
{

	DrawableObject dr('a',Colorf(0,0,0,1));

	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(0.0f);


	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(s_initialLineLength);
	m_linePositions.push_back(0.0f);

	m_paintState.ModelView = Matrix4f::Identity();
	m_paintState.Color = Colorf(0,0,0,1.0f);
	m_paintState.Angle = RandomValue(minAngle,maxAngle);
	m_paintState.LineLength = RandomValue(minLength, maxLength);
	m_paintState.LineWidth = RandomValue(10.0f, 10.0f);
}
void TurtleGraphics::init()
{
	m_program = App::createProgram(s_VertexShader, s_FragmentShader);
	m_positionHandle = glGetAttribLocation(m_program, "vPosition");
	m_modelviewHandle = glGetUniformLocation(m_program,"mModelView");
}

bool TurtleGraphics::drawLine(const PaintState& state)
{
	float length = state.LineLength.getValue();
	glLineWidth(state.LineWidth.getValue());
	glUseProgram(m_program);
    checkGlError("glUseProgram");

	glUniformMatrix4fv(m_modelviewHandle, 1, GL_FALSE,( m_paintState.ModelView * Matrix4f::Scale(length, length, length)).getValuePtr());
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
	m_paintState.ModelView = Matrix4f::Translation(0,-20.0,0).Transposed();
	float length = m_paintState.LineLength.getValue();
	
	for(int i = 0; i < size; i++)
	{
		switch (system[i])
		{
		case 'F':						
			drawLine(m_paintState);
			m_paintState.ModelView *= Matrix4f::Translation(0,length,0).Transposed();
			break;
		case '+':
			m_paintState.ModelView *= Matrix4f::Rotation(m_paintState.Angle.getValue(), Vector3f(0, 0, 1.0f));
			break;
		case '-':
			m_paintState.ModelView *=Matrix4f::Rotation(-m_paintState.Angle.getValue(), Vector3f(0, 0, 1.0f));
			break;
		case '[':
			m_StateStack.push(m_paintState);
			break;
		case ']':
			m_paintState = m_StateStack.top();
			m_StateStack.pop();
			break;
		case '(':
			m_paintState.Angle.setMean(std::max(0.0f,m_paintState.Angle.getMean() - s_angleModifier)); 
			break;
		case ')':
			m_paintState.Angle.setMean(std::min(180.0f,m_paintState.Angle.getMean() + s_angleModifier)); 
			break;
		case '<':
			m_paintState.LineLength.setMean(std::max(0.0f,m_paintState.LineLength.getMean() - s_lengthModifier)); 
			break;
		case '>':
			m_paintState.LineLength.setMean(m_paintState.LineLength.getMean() + s_lengthModifier); 
			break;
		case '\\':
			m_paintState.LineWidth.setMean(m_paintState.LineWidth.getMean() + s_widthModifier); 
			break;
		case '/':
			m_paintState.LineWidth.setMean(std::max(0.0f,m_paintState.LineWidth.getMean() - s_widthModifier)); 
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
	setMinMax(min,max);
}

float RandomValue::getValue(Distribution override) const
{
	Distribution dist = override == None ? m_distribution : override;
	float rand = rand01();

	if(dist == Uniform)
		return randomUniform(rand);
	else 
		return randomGaussian(rand);
}

inline float RandomValue::randomUniform(float rand) const
{
	return m_mean + m_variance * ( 2 * rand - 1.0f);
}

float RandomValue::randomGaussian(float rand) const
{
	float r = (1.0f / (0.5f * std::sqrtf(2 * (float)PI)) * std::expf( - (rand  - 0.5f) / 2 * 0.25f));
	return randomUniform(r);
}