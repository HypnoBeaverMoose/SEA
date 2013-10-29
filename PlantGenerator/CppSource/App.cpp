#include "Matrix.h"
#include "Definitions.h"
#include "LSystem.h"
#include "TurtleGraphics.h"
#include "App.h"

App::App() 
	: m_lSystem("F"), m_projectionMatrix(Matrix4f::Identity()), m_painter(20.0f, 25.0f, 0.1f, 0.2f), needsRedraw(true)
{	
	m_lSystem.addRule(Rule('F',"F[+F]F[-F]F",0.33f));
	m_lSystem.addRule(Rule('F',"F[+F]F",0.33f));
	m_lSystem.addRule(Rule('F',"F[-F]F",0.34f));
}

void App::OnCreate()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	m_painter.init();
	needsRedraw = true;
}

void App::OnRender()
{
	if(needsRedraw)
	{
		m_lSystem.reset();
		m_lSystem.Iterate(6);
		glClearColor(1.0f,1.0f,1.0f,0);
		glClear(GL_COLOR_BUFFER_BIT);
		std::string str = m_lSystem.getSystemString();
		m_painter.drawLSystem(str.c_str(),str.size());
		needsRedraw = false;
	}
}

void App::OnResize(int width, int height)
{
	glViewport(0,0,width,std::max(1, height));
	float aspect = width /(float) std::max(1, height);
	m_projectionMatrix = Matrix4f::Orthographic(0.1f,10.0f,-20.0f,20.0f,-20.0f,20.0f);
	//m_projectionMatrix = Matrix4f::Perspective(-0.1f,10.0f,50,aspect);
	glUseProgram(m_painter.getShaderHandle());	
	checkGlError("glUseProgram");
	glUniformMatrix4fv(glGetUniformLocation(m_painter.getShaderHandle(), "mProjection"), 1, GL_FALSE, m_projectionMatrix.getValuePtr());
	checkGlError("glUniformMatrix4fv");
	glUseProgram(0);
	checkGlError("glUseProgram");
	needsRedraw = true;	
}

uint App::loadShader(GLenum shaderType, const char* pSource) 
{
    GLuint shader = glCreateShader(shaderType);
    if (shader) 
	{
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
    
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) 
		{
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) 
			{
				std::auto_ptr<char> ptr(new char[infoLen]);
				glGetShaderInfoLog(shader, infoLen, NULL, ptr.get());
                LOGE("Could not compile shader %d:\n%s\n", shaderType, ptr.get());

                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

uint App::createProgram(const char* pVertexSource, const char* pFragmentSource)
{	
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader)	return 0;

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader)	return 0;

    GLuint program = glCreateProgram();
    if (program) 
	{
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        
		glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) 
		{
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) 
			{
                std::auto_ptr<char> buf(new char[bufLength]);
				glGetProgramInfoLog(program, bufLength, NULL, buf.get());
				LOGE("Could not link program:\n%s\n", buf.get());
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void App::OnTouch(int posx, int posy)
{
	needsRedraw = true;
}

void App::OnDestroy()
{

}

App::~App()
{
}


