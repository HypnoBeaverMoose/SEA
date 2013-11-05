#include "Definitions.h"
#include "Matrix.h"
#include "LSystem.h"
#include "Color.h"
#include "PaintState.h"
#include "DrawableObject.h"
#include "Plant.h"
#include "TurtleGraphics.h"
#include "App.h"

char* App::s_VertexShader = 
    "attribute vec3 vPosition;\n"
	"uniform mat4 mModelView;\n"
	"uniform mat4 mProjection;\n"
    "void main() {\n"
	"  gl_Position = mProjection * mModelView * vec4(vPosition, 1.0);\n"
    "}\n";

char* App::s_FragmentShader = 
    "precision mediump float;\n"
	"uniform vec4 vColor;\n"
    "void main() {\n"
    "  gl_FragColor = vColor;//vec4(0.0, 0.0, 0.0, 1.0);\n"
    "}\n";


App::App() 
	:	m_projectionMatrix(Matrix4f::Identity()), m_painter(25, 30, 10.0f, 10.0f, 10.0f, 10.0f), 
		needsRedraw(true), m_plant(5.0f, 5.2f, 5.0f, 0.7f, "-f-f-f-S", 1)

{
}

void App::SetUpPlant()
{

	DrawableObject leaf('l',Colorf(56 / 256.0f,133 / 256.0f, 0), 0.3f, m_programId);
	leaf.setWdith(0.0f,0.0f);
	//leaf.setWdith(0.1f,0.0f);	
	leaf.setWdith(0.2f,0.6f);	
	leaf.setWdith(0.6f,0.8f);
	leaf.setWdith(0.8f,0.7f);
	leaf.setWdith(0.85f,0.5f);
	leaf.setWdith(1.0f,0.0f);
	DrawableObject obj('f',Colorf(134 / 256.0f, 91 / 256.0f, 74 / 256.0f), 0.3f, m_programId);
	m_plant.addObject(leaf);
	m_plant.addObject(obj);
	m_plant.setPosition(Vector3f(0,-45,0));
	m_plant.addRule(Rule('L',"[>>>>\\\\))))--l]"));
	m_plant.addRule(Rule('R',"[>>>>\\\\))))++l]"));
	m_plant.addRule(Rule('S',"[<<\\\\\\\\f]/+++[[+R]+f+fR+fR+S+\\\\\\[)))))---[+l][+++l]]]---[L-f-fL-fL+S+\\\\\\[)))))+++[-l][--l][---l]]]"));
	m_plant.addRule(Rule('S',"[<<\\\\\\\\f]/+++[[+R]+f+f+fR+S+\\\\\\[))))---[+l][++l][+++l]]]---[L-f-f-fL+S+\\\\\\[))))+++[-l][---l]]]"));
	m_plant.addRule(Rule('S',"/[-f[-L]-f-f[-L]-S+\\\\[+l][-l]]",0.2f));
	m_plant.addRule(Rule('S',"/[+f[+R]+f[+R]+f+S+\\\\[+l][-l][-l]]",0.2f));	

	m_plant.setIterations(0);


}
void App::OnCreate()
{
	LOGI("OnCreate()");
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_painter.init();
	m_programId = createProgram(s_VertexShader, s_FragmentShader);

	SetUpPlant();
	needsRedraw = true;
}

void App::OnRender()
{
	if(needsRedraw)
	{
		m_plant.regeneratePlant();
		glClearColor(1.0f,1.0f,1.0f,0);
		glClear(GL_COLOR_BUFFER_BIT);
		m_painter.drawPlant(m_plant);
		needsRedraw = false;		
	}
}

void App::OnResize(int width, int height)
{
	LOGI("OnResize()");
	glViewport(0,0,width,std::max(1, height));
	float aspect = width /(float) std::max(1, height);
	m_projectionMatrix = Matrix4f::Orthographic(0.1f,10.0f,-50.0f,50.0f,-50.0f,50.0f);
	glUseProgram(m_programId);
	checkGlError("glUseProgram");
	glUniformMatrix4fv(glGetUniformLocation(m_programId, "mProjection"), 1, GL_FALSE, m_projectionMatrix.getValuePtr());
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
	m_plant.setIterations(m_plant.getIterations() + 1);
	needsRedraw = true;
}

void App::OnDestroy()

{

}

App::~App()
{
}


