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
    "attribute vec4 vPosition;\n"
	"attribute vec2 vTexCoord;\n"
	"uniform mat4 mModelView;\n"
	"uniform mat4 mProjection;\n"
	"varying vec2 varTexCoord;\n"
    "void main() {\n"
	"	gl_Position = mProjection * mModelView * vPosition;\n"
	"	varTexCoord = vTexCoord;//(mProjection * mModelView * vPosition).xy; \n"
    "}\n";


char* App::s_FragmentShader = 
    "precision mediump float;\n"
	"varying vec2 varTexCoord;\n"
	"uniform sampler2D tDiffuse;"
	"uniform vec4 vColor;\n"	
	"void main() {\n"
	"	vec4 color = texture2D(tDiffuse, varTexCoord);\n"
    "	gl_FragColor = color * vColor;\n"
    "}\n";

App* App::s_instance = 0;

App* const App::getInstance()
{
//	LOGI("App::getInstance()");
	if(s_instance == NULL)
		s_instance = new App;
	return s_instance;
}

App::App() 
	:	m_projectionMatrix(Matrix4f::Identity()), m_painter(25, 30, 10.0f, 10.0f, 10.0f, 10.0f), 
		needsRedraw(true), m_bias(0.0f)

{
}

void App::SetUpPlant()
{
	png::image<png::rgba_pixel> img;
	loadImage(img,"checker.png");
	m_plants.push_back(Plant(0,100,0,0,"f",2));
	m_plants[0].addObject(DrawableObject('f',Colorf(1,1,0,1),img,1.0f, m_programId));
	m_plants[0].addRule(Rule('f',"f"));
	m_plants[0].regeneratePlant();
	m_plants[0].setPosition(Vector3f(0,-0.5f,0));
}
void App::OnCreate()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glActiveTexture(GL_TEXTURE0);
	checkGlError("glActiveTexture");
	m_programId = createProgram(s_VertexShader, s_FragmentShader);	
	SetUpPlant();
	needsRedraw = true;
}

void App::OnRender()
{
	//if(needsRedraw)
	{
		glClearColor(1.0f,1.0f,1.0f,0);
		glClear(GL_COLOR_BUFFER_BIT);		
		m_painter.drawPlant(m_plants[0]);
		needsRedraw = false;		
	}
}

void App::OnResize(int width, int height)
{
	glViewport(0,0,width,std::max(1, height));
	float aspect = width /(float) std::max(1, height);
	m_projectionMatrix = Matrix4f::Orthographic(1.0f, 30.0f,-50.0f, 50.0f ,-50.0f * aspect,50.0f * aspect);
	//m_projectionMatrix = Matrix4f::Identity();
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


bool App::loadImageFromFile(png::image<png::rgba_pixel>& image, const char* filename)
{
	return getInstance()->loadImage(image,filename);
}

bool App::loadImage(png::image<png::rgba_pixel>& image, const char* filename)
{
	std::ifstream fileInput(filename, std::ios::binary);
	
	if(!fileInput.is_open()) 
	{
		LOGE("IMAGE LOAD:CANNOT OPEN FILE");
		return false;
	}
	image.read_stream(fileInput);
	return true;
}

void App::OnTouch(int posx, int posy)
{
	//m_plants[1].setIterations(m_plants[1].getIterations() + 1);
	m_bias+=0.1f;
	m_bias = std::min(m_bias,1.0f);
	needsRedraw = true;
}

void App::OnDestroy()
{
}

App::~App()
{
}


