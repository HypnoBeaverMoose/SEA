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
		needsRedraw(true), m_bias(0.0f)

{
}

void App::SetUpPlant()
{
	png::image<png::rgba_pixel> img;
	loadImageFromFile(this, img,"test.png");
	DrawableObject cactusTrunk('f',Colorf(56 / 256.0f,133 / 256.0f, 0), 10.0f, m_programId);
	DrawableObject top('t',Colorf(56 / 256.0f,133 / 256.0f, 0), 10.0f, m_programId);
	top.setWdith(1.0f,0.0f);
	top.setWdith(0.5f,0.8f);
	top.setWdith(0.7f,0.6f);
	top.setWdith(0.9f,0.4f);
	DrawableObject bottom('b',Colorf(56 / 256.0f,133 / 256.0f, 0), 10.0f, m_programId);
	bottom.setWdith(0.0f,0.6f);
	bottom.setWdith(0.5f,0.8f);

	DrawableObject thorn('l',Colorf(0,0,0, 0), 0.1f, m_programId,0.0f);
	Plant cactus(22.5f,1.0f,0.3f,0.3f,">>>>>b<<<<<F>>>>>t",5);
	cactus.addObject(cactusTrunk);
	cactus.addObject(thorn);
	cactus.addObject(top);
	cactus.addObject(bottom);
	cactus.addRule(Rule('F',"[L][R]FFf"));
	cactus.addRule(Rule('L',"&[----&&&&&l]&[----&&&&&l]&[----&&&&&l]L"));
	cactus.addRule(Rule('R',"&[++++&&&&&l]&[++++&&&&&l]&[++++&&&&&l]R"));

	DrawableObject leaf('l',Colorf(56 / 256.0f,133 / 256.0f, 0), 0.3f, m_programId);
	leaf.setWdith(0.0f,0.0f);
	leaf.setWdith(0.2f,0.6f);	
	leaf.setWdith(0.6f,0.8f);
	leaf.setWdith(0.8f,0.7f);
	leaf.setWdith(0.85f,0.5f);
	leaf.setWdith(1.0f,0.0f);
	DrawableObject obj('f',Colorf(134 / 256.0f, 91 / 256.0f, 74 / 256.0f), 0.3f, m_programId);


	m_plants.push_back(Plant(5.0f, 5.2f, 5.0f, 0.7f, "-f-f-f-S", 1));
	m_plants.push_back(cactus);
	m_plants[1].setPosition(Vector3f(0,-45,0));
	m_plants[0].setPosition(Vector3f(0,-45,0));
	m_plants[0].addObject(leaf);
	m_plants[0].addObject(obj);
	m_plants[0].setPosition(Vector3f(0,-45,0));
	m_plants[0].addRule(Rule('L',"[>>>>\\\\))))--l]"));
	m_plants[0].addRule(Rule('R',"[>>>>\\\\))))++l]"));
	m_plants[0].addRule(Rule('S',"[<<\\\\\\\\f]/+++[[+R]+f+fR+fR+S+\\\\\\[)))))---[+l][+++l]]]---[L-f-fL-fL+S+\\\\\\[)))))+++[-l][--l][---l]]]"));
	m_plants[0].addRule(Rule('S',"[<<\\\\\\\\f]/+++[[+R]+f+f+fR+S+\\\\\\[))))---[+l][++l][+++l]]]---[L-f-f-fL+S+\\\\\\[))))+++[-l][---l]]]"));
	m_plants[0].addRule(Rule('S',"/[-f[-L]-f-f[-L]-S+\\\\[+l][-l]]",0.2f));
	m_plants[0].addRule(Rule('S',"/[+f[+R]+f[+R]+f+S+\\\\[+l][-l][-l]]",0.2f));	

	m_plants[0].setIterations(4);
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
		//DrawableObject cactusTrunk('f',Colorf(56 / 256.0f,133 / 256.0f, 0), 2.0f, m_programId);

		//DrawableObject obj('f',Colorf(134 / 256.0f, 91 / 256.0f, 74 / 256.0f), 0.3f, m_programId);
		//DrawableObject dro = CombineObjects(cactusTrunk,obj,m_bias);
		Plant plant = CombinePlants(m_plants[0],m_plants[1],m_bias);
		plant.regeneratePlant();
		plant.setPosition(Vector3f(0,-40,0));


		//Plant plant1(0,10,0,0, "f",1);
		//Plant plant2(0,10,0,0, "f",1);
		//plant.addObject(cactusTrunk);
		//plant1.addObject(dro);
		//plant2.addObject(obj);

		//plant1.regeneratePlant();
		//plant2.regeneratePlant();
		//plant1.setPosition(Vector3f(10,-40,0));
		//plant2.setPosition(Vector3f(10,-40,0));

		
		//m_plants[0].regeneratePlant();
		glClearColor(1.0f,1.0f,1.0f,0);
		glClear(GL_COLOR_BUFFER_BIT);		
		m_painter.drawPlant(plant);
		//m_painter.drawPlant(plant1);
		//m_painter.drawPlant(plant2);
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


bool App::loadImageFromFile(App* app, png::image<png::rgba_pixel>& image, const char* filename)
{
	return app->loadImage(image,filename);
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


