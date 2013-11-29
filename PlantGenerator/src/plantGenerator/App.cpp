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
	m_biases[0] = m_biases[1] = m_biases[2] = 0;
	m_renderUV.push_back(Vector2f(0, 0));
	m_renderUV.push_back(Vector2f(1, 0));
	m_renderUV.push_back(Vector2f(0, 1));
	m_renderUV.push_back(Vector2f(1, 1));

	m_renderQuad.push_back(Vector4f(-1.0f, -1.0f, 0, 1.0f));
	m_renderQuad.push_back(Vector4f(1.0f, -1.0f, 0, 1.0f));
	m_renderQuad.push_back(Vector4f(-1.0f, 1.0f, 0, 1.0f));
	m_renderQuad.push_back(Vector4f(1.0f, 1.0f, 0, 1.0f));
}
void App::loadPlant(PlantDatabase::PlantData plant, int index)
{
	Plant _plant(plant.angle, plant.scale, plant.angleInc, plant.scaleInc, plant.axiom, plant.iterCount);
	for(uint i = 0; i < plant.rules.size(); i++){
		_plant.addRule(Rule(plant.rules[i].lhs,plant.rules[i].rhs,plant.rules[i].prob));
	}

	for(uint i = 0; i < plant.drawData.size(); i++){		
		png::image<png::rgba_pixel> image;
		getInstance()->loadImage(image, plant.drawData[i].texture.c_str());

		DrawableObject obj(plant.drawData[i].letter, Colorf(plant.drawData[i].clr.c_str()),	image,Vector2f(1.0f,1.0f), plant.drawData[i].vertOffset);
		
		for(uint j = 0; j < plant.drawData[i].verts.size();j++)
			obj.setWdith(plant.drawData[i].verts[j].height, plant.drawData[i].verts[j].width);

		_plant.addObject(obj);
	}

	m_plants[index] = _plant;
}

void App::setUpPlant()
{	
	//tomato plant
	png::image<png::rgba_pixel> leaf, fruit, cact, thorn, dusty, flower;
	loadImage(leaf,"leaf.png");
	loadImage(fruit, "tomato.png");
	Plant tomato(3, 0.025f,1.5f, 1.3f, "[--R][-R][R][+R][++R][PF]", 5);
	
	DrawableObject t_root('r',Colorf("#DB877CFF"), m_defaultTexture, Vector2f(1.0f, 1.2f), m_programId, 0.0f, -0.6);
	DrawableObject t_stalk('s',Colorf("#3D6E11FF"), m_defaultTexture, Vector2f(1.2f, 1.3f), m_programId, 0.0f, 0.8f);
	DrawableObject t_leaf('l',Colorf("#FFFFFFFF"), leaf, Vector2f(5.0f,5.0f), m_programId, 0.0f);
	DrawableObject t_fruit('f',Colorf("#FFFFFFFF"), fruit, Vector2f(3.0f,3.0f), m_programId, -0.1f);

	tomato.addObject(t_root);
	tomato.addObject(t_stalk);
	tomato.addObject(t_leaf);
	tomato.addObject(t_fruit);

	tomato.addRule(Rule('R',"r-r+r#R"));
	tomato.addRule(Rule('F',"f"));
	tomato.addRule(Rule('L',"l"));

	tomato.addRule(Rule('P',"[)))--A))---F][))))))++L]S[)))++BF][))))))--L]S+#P"));
	tomato.addRule(Rule('B',"#S[))))))))+++L]S[))))))))+++F]B"));
	tomato.addRule(Rule('A',"#V[))))))))---L]V[))))))))--F]V"));
	tomato.addRule(Rule('S',"Ss+s"));
	tomato.addRule(Rule('V',"Vs-s"));
	tomato.addRule(Rule('K',"Kss"));

	tomato.setPosition(Vector3f(0.5f,0.2f,1));

	loadImage(cact, "cactus.png");
	loadImage(thorn, "thorns.png");

	Plant cactus(2.0f, 0.03f, 1.5f, 1.1f, "[)))R][P]", 5);
	DrawableObject c_root('r',Colorf("#DB877CFF"), m_defaultTexture, Vector2f(3.0f,3.0f), m_programId, 1.0f, -0.9f);
	DrawableObject c_stalk('s',Colorf(0,1,0,1), cact, Vector2f(5.0f,5.0f), m_programId, 0.0f, 0.7f);
	DrawableObject c_leaf('l',Colorf(1,1,1,1), thorn, Vector2f(1.5f,1.5f), m_programId, 0.0f);
	DrawableObject c_fruit('f',Colorf(1,0,0,1), m_defaultTexture, Vector2f(5.0f,5.0f), m_programId, 0.0f);
	
	cactus.addObject(c_root);
	cactus.addObject(c_stalk);
	cactus.addObject(c_leaf);
	cactus.addObject(c_fruit);

	cactus.addRule(Rule('F',"f"));
	cactus.addRule(Rule('L',"l"));
	cactus.addRule(Rule('R',"r+#r-#r#R"));
	cactus.addRule(Rule('P',"SL"));
	cactus.addRule(Rule('S',"s@S#s"));
	cactus.addRule(Rule('V',"s@V#s"));
	cactus.addRule(Rule('K',"sKs"));

	cactus.addRule(Rule('A',"A"));
	cactus.addRule(Rule('B',"B"));
	cactus.setPosition(Vector3f(0.5f,0.2f,1));

	Plant dustyMiller(1.4, 0.025f, 1.5f, 1.2f, "[>>>R]P[((((Sf][)))))+++++Vf][))))))----Sf]", 5);

	loadImage(dusty, "dusty.png");
	loadImage(flower, "flower.png");
	DrawableObject d_root('r',Colorf("#DB877CFF"), m_defaultTexture, Vector2f(1.0f,1.0f), m_programId, 0.5f, -0.5f);
	DrawableObject d_stalk('s',Colorf("#9CC7A1FF"), m_defaultTexture, Vector2f(1.0f,1.0f), m_programId, 0.0f, 0.7f);
	DrawableObject d_leaf('l',Colorf(1,1,1,1), dusty, Vector2f(3.0f,3.0f), m_programId, 0.0f);
	DrawableObject d_fruit('f',Colorf(1,1,1,1), flower, Vector2f(3.0f, 3.0f), m_programId, 0.5f);
	
	dustyMiller.addObject(d_root);
	dustyMiller.addObject(d_stalk);
	dustyMiller.addObject(d_leaf);
	dustyMiller.addObject(d_fruit);

	dustyMiller.addRule(Rule('F',"f"));
	dustyMiller.addRule(Rule('L',"[)))--l][)))++l]"));
	dustyMiller.addRule(Rule('R',"Rr[#-r][#+r]"));
	dustyMiller.addRule(Rule('r',"#rr"));

	dustyMiller.addRule(Rule('P',"[))))------B]K[))))+++++++A]P"));

	dustyMiller.addRule(Rule('B',"-BS[L]S[L]"));
	dustyMiller.addRule(Rule('A',"+AV[L]V[L]"));
	dustyMiller.addRule(Rule('S',"Ss+s"));
	dustyMiller.addRule(Rule('V',"Vs-s"));
	dustyMiller.addRule(Rule('K',"Kss"));

	dustyMiller.setPosition(Vector3f(0.5f,0.2f,1));
	
	m_plants.push_back(dustyMiller);
	m_plants.push_back(cactus);
	m_plants.push_back(tomato);	

	m_resultPlant = Plant(tomato);
}

void App::combinePlants(int l_index, int r_index, PlantPart part)
{

	LOGI("plant combination set: left: %d, right: %d, part: %d",l_index, r_index, (uint)part);
	CombinePlants(m_resultPlant, m_plants[l_index], m_plants[r_index],m_biases[part], part);
}

void App::setBias( float bias, PlantPart part)
{
	m_biases[part] = bias;
}

void App::OnCreate()
{
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	checkGlError("glPixelStorei");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	checkGlError("glPixelStorei");
	LOGI("OnCreate::ONE");
	glEnable(GL_BLEND);
	checkGlError("glEnable");
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glActiveTexture(GL_TEXTURE0);
	checkGlError("glActiveTexture");
	m_programId = createProgram(s_VertexShader, s_FragmentShader);	
	LOGI("OnCreate::TWO");

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
	LOGI("OnCreate::Three");
	glGenFramebuffers(1,&m_framebufferHandle);
	checkGlError("glGenFramebuffers");	
	glGenTextures(1, &m_targetTexHandle);
	checkGlError("glGenTextures");	
	glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferHandle);
	checkGlError("glBindFramebuffer");	
	LOGI("OnCreate::FOUR");
	glBindTexture(GL_TEXTURE_2D, m_targetTexHandle);
	checkGlError("glBindTexture");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	checkGlError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	checkGlError("glTexParameteri");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE,0);
	checkGlError("glTexImage2D");
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_targetTexHandle, 0);
	checkGlError("glFramebufferTexture2D");
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	LOGI("OnCreate::FIVE");
	checkGlError("glBindFramebuffer");
	glGenTextures(1, &m_previewTexHandle);

	loadImage(m_defaultTexture,"default.png");
	LOGI("OnCreate::SIX");
	needsRedraw = true;
}
void App::RenderPlant()
{
	//setBias(0.0f, PlantPart::Stalk);
	//combinePlants(0, 2, PlantPart::Stalk);
	m_resultPlant.regeneratePlant();
	glBindTexture(GL_TEXTURE_2D, 0);
	checkGlError("glBindTexture");
	glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferHandle);
	checkGlError("glBindFramebuffer");
	glViewport(0,0,(int)m_renderSize[0],(int)m_renderSize[1]);
	float aspect = m_renderSize[0] / m_renderSize[1];
	
	glUseProgram(m_programId);
	checkGlError("glUseProgram");

	glUniformMatrix4fv(glGetUniformLocation(m_programId, "mProjection"), 1, GL_FALSE, 
		Matrix4f::Orthographic(0.0f, 1.0f, 0, 1, 0, aspect).Transposed().getValuePtr());
	checkGlError("glUniformMatrix4fv");
	
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_painter.drawPlant(m_resultPlant);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

byte* App::getPlantImage(uint& width, uint& height)
{
	width = (uint)m_renderSize[0]; height = (uint)m_renderSize[1];
	std::auto_ptr<byte> tex(new byte[4 * width * height ]);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferHandle);
	glReadPixels(0, 0, width,  height, GL_RGBA, GL_UNSIGNED_BYTE, tex.get());
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
		uint width, height;
		const byte* img = getPlantImage(width, height);
		glBindTexture(GL_TEXTURE_2D, m_previewTexHandle);		
		checkGlError("glBindTexture");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		checkGlError("glTexParameteri");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		checkGlError("glTexParameteri");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		delete img;
	}


	glViewport(0,0,(int)m_viewportSize[0], (int)m_viewportSize[1]);
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);		
	float aspect = m_viewportSize[0] /(float) std::max(1.0f, m_viewportSize[1]);
	m_projectionMatrix = Matrix4f::Orthographic(0.0f, 1.0f, -1.0f, 1.0f, -aspect, aspect).Transposed();
	glUseProgram(m_programId);
	uint loc = glGetUniformLocation(m_programId, "mProjection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, Matrix4f::Identity().getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(m_modelViewHandle, 1, GL_FALSE, Matrix4f::Identity().getValuePtr());
	checkGlError("glUniformMatrix4fv");

	glUniform4fv(m_colorHandle, 1, Colorf(1, 1, 1, 1).getValuePtr());
    checkGlError("glUniform1fv");

	glVertexAttribPointer(m_textureCoordsHandle, 2, GL_FLOAT, GL_FALSE, m_renderUV[0].stride(), m_renderUV.data()->getValuePtr());
    checkGlError("glVertexAttribPointer");

	glVertexAttribPointer(m_positionHandle, 4, GL_FLOAT, GL_FALSE, m_renderQuad[0].stride(), m_renderQuad.data()->getValuePtr());
    checkGlError("glVertexAttribPointer");
	
	glBindTexture(GL_TEXTURE_2D, m_previewTexHandle);		
	glUniform1i(glGetUniformLocation(m_programId,"tDiffuse"),0);	

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_renderQuad.size());
    checkGlError("glDrawArrays");
}

void App::OnResize(int width, int height)
{
	m_viewportSize[0] = (float)width; m_viewportSize[1] = (float)height;
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
//	m_plants[0].setIterations(m_plants[0].getIterations() + 1);
	m_bias+=0.1f;
	m_plants.clear();
	setUpPlant();
	m_bias = std::min(m_bias,1.0f);
	needsRedraw = true;
}

void App::SetDefaultBiases(float leaves, float stalk, float flowers)
{
	m_biases[Flowers] = flowers;
	m_biases[Leaves] = leaves;
	m_biases[Stalk] = stalk;
}

void App::setRenderSize(int width, int height)
{
	m_renderSize[0] = (float)width;
	m_renderSize[1] = (float)height;
}

void App::OnDestroy()
{
}

App::~App()
{
}


