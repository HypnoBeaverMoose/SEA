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
	if(s_instance == NULL)
		s_instance = new App;
	return s_instance;
}

App::App() 
	:	m_projectionMatrix(Matrix4f::Identity()), m_painter(25, 30, 10.0f, 10.0f, 10.0f, 10.0f), 
		needsRedraw(true), m_bias(0.0f), m_renderSize(512,512), m_viewportSize(0,0) 

{
	m_renderUV.push_back(Vector2f(0, 0));
	m_renderUV.push_back(Vector2f(1, 0));
	m_renderUV.push_back(Vector2f(0, 1));
	m_renderUV.push_back(Vector2f(1, 1));

	m_renderQuad.push_back(Vector4f(-1.0f, -1.0f, 0, 1.0f));
	m_renderQuad.push_back(Vector4f(1.0f, -1.0f, 0, 1.0f));
	m_renderQuad.push_back(Vector4f(-1.0f, 1.0f, 0, 1.0f));
	m_renderQuad.push_back(Vector4f(1.0f, 1.0f, 0, 1.0f));
}

void App::SetUpPlant()
{
	png::image<png::rgba_pixel> img;
	loadImage(img,"checker.png");
	m_plants[0].regeneratePlant();
	m_plants.push_back(Plant(0,1.0f,0,0,"f",2));
	m_plants[0].addObject(DrawableObject('f',Colorf(1,1,1,1),img, 1.0f, m_programId));
	m_plants[0].addRule(Rule('f',"f"));
	m_plants[0].setPosition(Vector3f(0.5f,0,0));
}

void App::OnCreate()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glActiveTexture(GL_TEXTURE0);
	checkGlError("glActiveTexture");
	m_programId = createProgram(s_VertexShader, s_FragmentShader);	

	m_textureCoordsHandle = glGetAttribLocation(m_programId, "vTexCoord");
	checkGlError("glGetAttribLocation");
	m_positionHandle = glGetAttribLocation(m_programId, "vPosition");
	checkGlError("glGetAttribLocation");

	m_modelViewHandle = glGetUniformLocation(m_programId,"mModelView");
	checkGlError("glGetUniformLocation");
	m_colorHandle = glGetUniformLocation(m_programId,"vColor");
	checkGlError("glGetUniformLocation");
    glEnableVertexAttribArray(m_positionHandle);
    checkGlError("glEnableVertexAttribArray");	
    glEnableVertexAttribArray(m_textureCoordsHandle);
    checkGlError("glEnableVertexAttribArray");	

	glGenFramebuffers(1,&m_framebufferHandle);
	checkGlError("glGenFramebuffers");	
	glGenTextures(1, &m_targetTexHandle);
	checkGlError("glGenTextures");	
	glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferHandle);
	checkGlError("glBindFramebuffer");	
	glBindTexture(GL_TEXTURE_2D, m_targetTexHandle);
	checkGlError("glBindTexture");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	checkGlError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	checkGlError("glTexParameteri");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_renderSize[0], m_renderSize[1], 0, GL_RGBA, GL_UNSIGNED_BYTE,0);
	checkGlError("glTexImage2D");
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_targetTexHandle, 0);
	checkGlError("glFramebufferTexture2D");
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	checkGlError("glBindFramebuffer");
	
	//SetUpPlant();
	needsRedraw = true;
}
void App::RenderPlant()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	checkGlError("glBindTexture");
	glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferHandle);
	checkGlError("glBindFramebuffer");

	glViewport(0,0,(int)m_renderSize[0],std::max(1, (int)m_renderSize[1]));
	float aspect = m_renderSize[0] /(float) std::max(1.0f, m_renderSize[1]);

	glUseProgram(m_programId);
	checkGlError("glUseProgram");

	glUniformMatrix4fv(glGetUniformLocation(m_programId, "mProjection"), 1, GL_TRUE, 
		Matrix4f::Orthographic(0.0f, 1.0f, 0, 1, 0, aspect).getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_painter.drawPlant(m_plants[0]);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

byte* App::getPlantImage(uint& width, uint& height)
{
	width = m_renderSize[0]; height = m_renderSize[1];
	std::auto_ptr<byte> tex(new byte(width * height * 4));
	glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferHandle);
	glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,tex.get());
	glBindFramebuffer(GL_FRAMEBUFFER,0);	
	return tex.release();
}
void App::OnRender()
{
	//redraw the plant to a frame-bufffer
	if(needsRedraw)
	{
		RenderPlant();
		needsRedraw = false;	
	}

	glViewport(0,0,m_viewportSize[0],m_viewportSize[1]);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);		
	float aspect = m_viewportSize[0] /(float) std::max(1.0f, m_viewportSize[1]);
	m_projectionMatrix = Matrix4f::Orthographic(0.0f, 1.0f, -1.0f, 1.0f, -aspect, aspect);
	
	///draw full screen quad with the texture
	glUseProgram(m_programId);
	glUniformMatrix4fv(glGetUniformLocation(m_programId, "mProjection"), 1, GL_TRUE, m_projectionMatrix.getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(m_modelViewHandle, 1, GL_FALSE, Matrix4f::Identity().getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glUniform4fv(m_colorHandle, 1, Colorf(1, 1, 1, 1).getValuePtr());
    checkGlError("glUniform1fv");

	glVertexAttribPointer(m_textureCoordsHandle, 2, GL_FLOAT, GL_FALSE, m_renderUV[0].stride(), m_renderUV.data()->getValuePtr());
    checkGlError("glVertexAttribPointer");

	glVertexAttribPointer(m_positionHandle, 4, GL_FLOAT, GL_FALSE, m_renderQuad[0].stride(), m_renderQuad.data()->getValuePtr());
    checkGlError("glVertexAttribPointer");
	
	glBindTexture(GL_TEXTURE_2D, m_targetTexHandle);		
	glUniform1i(glGetUniformLocation(m_programId,"tDiffuse"),0);	

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_renderQuad.size());
    checkGlError("glDrawArrays");
}

void App::OnResize(int width, int height)
{
	m_viewportSize[0] = width; m_viewportSize[1] = height;
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

void App::SetDefaultBiases(float leaves, float stalk, float flowers)
{
	m_flowerBias = flowers;
	m_leafBias = leaves;
	m_stalkBias = stalk;
}

void App::setRenderSize(int width, int height)
{
	m_renderSize[0] = width;
	m_renderSize[1] = height;
}

void App::OnDestroy()
{
}

App::~App()
{
}


