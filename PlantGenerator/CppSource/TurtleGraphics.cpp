#include "Definitions.h"
#include "PaintState.h"
#include "DrawableObject.h"
#include "TurtleGraphics.h"
#include "LSystem.h"
#include "App.h"

float TurtleGraphics::s_initialLineLength = 1.0f;	
float TurtleGraphics::s_lengthModifier = 1.0f;
float TurtleGraphics::s_widthModifier = 1.0f;
float TurtleGraphics::s_angleModifier = 5.0f;


char* TurtleGraphics::s_VertexShader = 
    "attribute vec3 vPosition;\n"
	"uniform mat4 mModelView;\n"
	"uniform mat4 mProjection;\n"
    "void main() {\n"
	"  gl_Position = mProjection * mModelView * vec4(vPosition, 1.0);\n"
    "}\n";

char* TurtleGraphics::s_FragmentShader = 
    "precision mediump float;\n"
	"uniform vec4 vColor;\n"
    "void main() {\n"
    "  gl_FragColor = vColor;//vec4(0.0, 0.0, 0.0, 1.0);\n"
    "}\n";


TurtleGraphics::TurtleGraphics(float minAngle, float maxAngle, float minLength, float maxLength, float minWidth, float maxWidth) 
{
	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(0.0f);

	m_linePositions.push_back(0.0f);
	m_linePositions.push_back(s_initialLineLength);
	m_linePositions.push_back(0.0f);

	m_paintState.ModelView = Matrix4f::Identity();
	m_paintState.Color = Colorf(0.1f,0.1f,0.1f,1.0f);
	m_paintState.Angle = RandomValue(minAngle,maxAngle);
	m_paintState.LineLength = RandomValue(minLength, maxLength);
	m_paintState.LineWidth = RandomValue(minWidth, maxWidth);
}
void TurtleGraphics::init()
{
	m_program = App::createProgram(s_VertexShader, s_FragmentShader);
	m_positionHandle = glGetAttribLocation(m_program, "vPosition");
	checkGlError("glGetAttribLocation");
	m_modelviewHandle = glGetUniformLocation(m_program,"mModelView");
	checkGlError("glGetUniformLocation");
	m_colorHandle = glGetUniformLocation(m_program,"vColor");
	checkGlError("glGetUniformLocation");

	m_drawableObjects.insert(std::pair<char, DrawableObject>('f',DrawableObject('f',Colorf(82 / 256.0f,4 / 256.0f,4 / 256.0f,1),0.2f, m_program)));
//	m_drawableObjects['f'].setShader(m_program);
}

bool TurtleGraphics::drawLSystem(const char* system, int size, const Vector3f& pos)
{
	m_paintState.ModelView =  Matrix4f::Translation(pos[0], pos[1], pos[2]).Transposed();
		
	for(int i = 0; i < size; i++)
	{
		if(system[i] > 'a' )
		{
			m_drawableObjects[system[i]].draw(m_paintState);			
		}

		switch (system[i])
		{
		case '+':
			m_paintState.Color = Colorf(m_paintState.Color * 2.0f);
			m_paintState.ModelView *= Matrix4f::Rotation(m_paintState.Angle.getValue(), Vector3f(0, 0, 1.0f));
			break;
		case '-':
			m_paintState.Color = Colorf(m_paintState.Color *2.0f);
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


bool TurtleGraphics::drawLine(const PaintState& state)
{
	
	glLineWidth(state.LineWidth.getValue());
	glUseProgram(m_program);
    checkGlError("glUseProgram");

	glUniformMatrix4fv(m_modelviewHandle, 1, GL_FALSE,
		( state.ModelView * Matrix4f::Scale(state.LineLength.getValue(), state.LineLength.getValue(), state.LineLength.getValue())).getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glUniform1fv(m_colorHandle,1.0f,state.Color.getValuePtr());
	glVertexAttribPointer(m_positionHandle, 3, GL_FLOAT, GL_FALSE, 0, m_linePositions.data());
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(m_positionHandle);
    checkGlError("glEnableVertexAttribArray");

	glDrawArrays(GL_LINE_STRIP, 0, 2);
    checkGlError("glDrawArrays");
	
	return true;
}
