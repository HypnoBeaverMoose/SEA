#include "Matrix.h"
#include"Includes.h"
#include "Definitions.h"
#include "LSystem.h"
#include "TurtleGraphics.h"
#include "App.h"

//char* App::s_VertexShader = 
//    "attribute vec4 vPosition;\n"
//	"uniform mat4 mModelView;"
//	"uniform mat4 mProjection;"
//    "void main() {\n"
//	"  gl_Position = mProjection * mModelView * vPosition;\n"
//    "}\n";
//
//char* App::s_FragmentShader = 
//    "precision mediump float;\n"
//    "void main() {\n"
//    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
//    "}\n";
//

App::App() : m_lSystem("F-F-F-F"), m_projectionMatrix(Matrix4f::Identity()), m_painter(90,0.15f)//, m_modelViewMatrix(Matrix4f::Identity())
{	
/*	float triangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };

	m_triangle.assign(triangleVertices,triangleVertices + 6);*/	

	m_lSystem.addRule(Rule('F',"F+F-F-FF+F+F-F"));
	m_lSystem.Iterate(3);
}

void App::OnCreate()
{
	//m_programId = createProgram(s_VertexShader, s_FragmentShader);
	//m_positionHandle = glGetAttribLocation(m_programId, "vPosition");

	//m_modelViewHandle = glGetUniformLocation(m_programId, "mModelView");
	//checkGlError("glGetUniformLocation");
	//m_projectionHandle = glGetUniformLocation(m_programId, "mProjection");
	//checkGlError("glGetUniformLocation");
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	m_painter.init();
}

void App::OnRender()
{
	glClearColor(1.0f,1.0f,1.0f,0);
	glClear(GL_COLOR_BUFFER_BIT);
	std::string str = m_lSystem.getSystemString();
	m_painter.drawLSystem(str.c_str(),str.size());


	//int handle = glGetUniformLocation(m_programId, "mProjection");
	//glUseProgram(m_Painter.getShaderHandle());
	//checkGlError("glUseProgram");
	
	//std::string str = m_lSystem.getSystemString();
	//m_painter.drawLSystem(str.c_str(),str.size());
    

	//glUniformMatrix4fv(glGetUniformLocation(m_programId, "mProjection"), 1, GL_FALSE, m_projectionMatrix.getValuePtr());
	//checkGlError("glUniformMatrix4fv");

	//glVertexAttribPointer(m_positionHandle, 2, GL_FLOAT, GL_FALSE, 0, m_triangle.data());
 //   checkGlError("glVertexAttribPointer");

 //   glEnableVertexAttribArray(m_positionHandle);
 //   checkGlError("glEnableVertexAttribArray");

	//glDrawArrays(GL_TRIANGLES, 0, 3);
 //   checkGlError("glDrawArrays");
}

void App::OnResize(int width, int height)
{
	glViewport(0,0,width,max(1, height));
	m_projectionMatrix =  Matrix4f::Orthographic(0.1f,10.0f,10.0f,-10.0f,-10.0f,10.0f);
	glUseProgram(m_painter.getShaderHandle());	
	checkGlError("glUseProgram");
	glUniformMatrix4fv(glGetUniformLocation(m_painter.getShaderHandle(), "mProjection"), 1, GL_FALSE, m_projectionMatrix.getValuePtr());
	checkGlError("glUniformMatrix4fv");
	glUseProgram(0);
	checkGlError("glUseProgram");
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

}

void App::OnDestroy()
{

}

App::~App()
{
}


