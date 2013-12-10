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
	"	if(color.w > 0.9)"
	"		gl_FragColor = color * vColor;\n"
	"	else discard;"
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
		needsRedraw(true), m_bias(0.0f), m_renderSize(1024,1024), m_viewportSize(0,0) 

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
	LOGI("Loading plant %d :",plant.id);
	LOGI("	axiom: %s",plant.axiom.c_str());
	LOGI("	iterations: %d",plant.iterCount);
	
	Plant _plant(plant.angle, plant.scale, plant.angleInc, plant.scaleInc, plant.axiom, plant.iterCount);
	for(uint i = 0; i < plant.rules.size(); i++){
		_plant.addRule(Rule(plant.rules[i].lhs,plant.rules[i].rhs,plant.rules[i].prob));
		LOGI("	rule %d: %c -> %s", i, plant.rules[i].lhs,plant.rules[i].rhs.c_str());

	}

	for(uint i = 0; i < plant.drawData.size(); i++){		
		png::image<png::rgba_pixel> image;
		LOGI("	texture %d: %s", i, plant.drawData[i].texture.c_str());
		loadImage(image, plant.drawData[i].texture.c_str());

		DrawableObject obj(plant.drawData[i].letter, Colorf(plant.drawData[i].clr.c_str()),	image, 
				Vector2f(plant.drawData[i].size.width, plant.drawData[i].size.height), m_programId, plant.drawData[i].vertOffset, plant.drawData[i].stepSize);
		
		for(uint j = 0; j < plant.drawData[i].verts.size();j++){
			obj.setWdith(plant.drawData[i].verts[j].height, plant.drawData[i].verts[j].width);
		}
		_plant.addObject(obj);
	}

	m_plants[index] = _plant;
	m_resultPlant = Plant(_plant);
}
Plant App::createTomato()
{
	png::image<png::rgba_pixel> leaf, fruit, cact, thorn, dusty, flower;

	png::image<png::rgba_pixel> tom_stalk, tom_leaf, tom_fruit, root;
	loadImage(tom_leaf,"textures/l_tomato_1.png");
	loadImage(tom_fruit, "textures/f_tomato_2.png");
	loadImage(tom_stalk, "textures/s_tomato.png");
	loadImage(root, "textures/root.png");

	Plant tomato(2, 0.018f,1.5f, 1.09f, "[<R][P]", 6);

	DrawableObject t_fruit('f',Colorf("#FFFFFFFF"), tom_fruit, Vector2f(1.0f,1.0f), m_programId, 0);//1.1f);
	DrawableObject t_root('r',Colorf("#DB877CFF"), root, Vector2f(1.0f, 1.5f), m_programId, 0.0f, -0.9);
	DrawableObject t_stalk('s',Colorf("#FFFFFFFF"), tom_stalk, Vector2f(1.0f, 3.0f), m_programId, 0.5f, 0.8f);
	DrawableObject t_leaf('l',Colorf("#FFFFFFFF"), tom_leaf, Vector2f(1.0f,1.0f), m_programId, 0);


	tomato.addObject(t_root);
	tomato.addObject(t_stalk);
	tomato.addObject(t_leaf);
	tomato.addObject(t_fruit);

	tomato.addRule(Rule('R',"r-#r+#r[))))++++++####R]-#r+r[))))------####R]<#R"));
	tomato.addRule(Rule('F',"[f]"));
	tomato.addRule(Rule('L',"[l]"));
	tomato.addRule(Rule('P',"@P#S[%)))--B))]#S[)))B]"));
	tomato.addRule(Rule('B',"S#[>>)))))++@@@@@@@@@@@@@@@>L][>)))))--@@@@@@@@@@@@@@>F]S#[>>)))-@@@@@@@@@@@@@@@>L](B"));
	tomato.addRule(Rule('S',"Ss+s"));

	return tomato;
}
Plant App::createDustyMiller()
{
	png::image<png::rgba_pixel> dus_stalk, dus_leaf, dus_flower, root;
	loadImage(dus_leaf,"textures/l_dusty_2.png");
	loadImage(dus_flower, "textures/f_dusty.png");
	loadImage(dus_stalk, "textures/s_dusty.png");
	loadImage(root, "textures/root.png");

	Plant dustyMiller(0.6, 0.025f, 2.0, 1.1f, "[<R][P]", 6);

	DrawableObject d_root('r',Colorf("#FFFFFFFF"), root, Vector2f(0.5f,1.0f), m_programId, 0.4f, -0.5f);
	DrawableObject d_stalk('s',Colorf("#FFFFFFFF"), dus_stalk, Vector2f(0.8f,1.0f), m_programId, 0.3f, 0.6f);
	DrawableObject d_leaf('l',Colorf("#FFFFFFFF"), dus_leaf, Vector2f(1.0f,1.0f), m_programId, 0.0f);
	DrawableObject d_flower('f',Colorf("#FFFFFFFF"), dus_flower, Vector2f(1, 1), m_programId, 0.5f);
	
	dustyMiller.addObject(d_stalk);
	dustyMiller.addObject(d_root);
	dustyMiller.addObject(d_leaf);
	dustyMiller.addObject(d_flower);

	dustyMiller.addRule(Rule('R',"R)[%-K][-K]"));
	dustyMiller.addRule(Rule('K',"r+r+r+r#%K"));
	dustyMiller.addRule(Rule('F',"[f]"));
	dustyMiller.addRule(Rule('L',"[)))---<l][)))+++l]"));
	dustyMiller.addRule(Rule('P',"[>))----#B<[@@@@@@@@@@@@l]]S#[%)))++>A[@@@@@@@@@@@@@@@@@>F]][)))++>A[@@@@@@@@@@@@@@@@@>F]]S#[%>))----#B<[@@@@@@@@@@@@l]]S#P"));
	dustyMiller.addRule(Rule('B',"S<[@@@@@@@@@@@@L]#S<[@@@@@@@@@@@@L]#S<[@@@@@@@@@@@@L]#B"));
	dustyMiller.addRule(Rule('A',"(AS#S#S"));
	dustyMiller.addRule(Rule('S',"S-s"));

	return dustyMiller;
}

Plant App::createCactus()
{
	png::image<png::rgba_pixel> cac_stalk, cac_leaf, cac_flower, root;
	loadImage(cac_stalk,"textures/s_cactus.png");
	loadImage(cac_leaf, "textures/l_cactus.png");
	loadImage(cac_flower, "textures/f_cactus.png");
	loadImage(root, "textures/root.png");

	Plant cactus(3.3, 0.2f, 3, 1.05f, "[<R][P]", 6);
	DrawableObject c_root('r',Colorf("#FFFFFFFF"), root, Vector2f(0.2f,0.3f), m_programId, 0.1f, -0.2f);
	DrawableObject c_stalk('s', Colorf("#FFFFFFFF"), cac_stalk, Vector2f(1.2f,1.2f), m_programId, 0.4, 0.45f);
	DrawableObject c_leaf('l', Colorf("#FFFFFFFF"), cac_leaf, Vector2f(0.7f,0.7f), m_programId, 0.1f );
	DrawableObject c_flower('f',Colorf("#FFFFFFFF"), cac_flower, Vector2f(1.0f,1.0f), m_programId, 0.0f);
	//
	cactus.addObject(c_stalk);
	cactus.addObject(c_root);
	cactus.addObject(c_leaf);
	cactus.addObject(c_flower);

	cactus.addRule(Rule('R',"))[K][%K][@rK][%@rK]"));
	cactus.addRule(Rule('K',"#r+#r+#r(K"));
	cactus.addRule(Rule('F',"f"));
	cactus.addRule(Rule('L',"[>l]"));
	
	cactus.addRule(Rule('P',"B>F"));
	cactus.addRule(Rule('B',"S#[)))+L][)))-L]B"));
	cactus.addRule(Rule('S',"s"));
	return cactus;
}
Plant App::createBamboo()
{	
	png::image<png::rgba_pixel> stalk, leaf, root, fruit;
	loadImage(stalk,"textures/s_bamboo.png");
	loadImage(leaf, "textures/l_bamboo_2.png");
	loadImage(root, "textures/root.png");
	loadImage(fruit,"textures/empty.png");

	DrawableObject b_stalk('s', Colorf("#FFFFFFFF"), stalk, Vector2f(1.0f,1.3f), m_programId, -0.2f, 0.7f);
	DrawableObject b_leaf('l', Colorf("#FFFFFFFF"), leaf, Vector2f(1.0f,1.0f), m_programId, -0.4f );
	DrawableObject b_root('r',Colorf("#FFFFFFFF"), root, Vector2f(1.0f,1.0f), m_programId, 0.5, -0.5f);
	DrawableObject b_fruit('f',Colorf("#FFFFFFFF"), fruit, Vector2f(1.0f,1.0f), m_programId, 0);


	Plant bamboo(3.3f, 0.06f, 3.0f, 1.1f, "[<R][P]", 6);
	bamboo.addObject(b_stalk);
	bamboo.addObject(b_leaf);
	bamboo.addObject(b_root);
	bamboo.addObject(b_fruit);

	bamboo.addRule(Rule('R',"r+++r+++#R"));
	bamboo.addRule(Rule('L',"[+>l]"));
	bamboo.addRule(Rule('L',"[->l]"));
	bamboo.addRule(Rule('P',"S[>)))@@@L]P"));
	bamboo.addRule(Rule('S',"sS"));

	bamboo.getLSystem().normalizeProbs();
	return bamboo;
}
Plant App::createPineapple()
{
	png::image<png::rgba_pixel> stalk, leaf, fruit, root;
	loadImage(stalk,"textures/s_pineapple.png");
	loadImage(leaf, "textures/empty.png");
	loadImage(fruit, "textures/f_pineapple.png");
	loadImage(root, "textures/root.png");

	DrawableObject p_fruit('f',Colorf("#FFFFFFFF"), fruit, Vector2f(1.0f,1.0f), m_programId, 0);
	DrawableObject p_root('r', Colorf("#965E0EFF"), root, Vector2f(0.5f,0.5f), m_programId, 0.0f, -0.3f);
	DrawableObject p_stalk('s', Colorf("#FFFFFFFF"), stalk, Vector2f(0.7f,1.6f), m_programId, 0.1f, 0.8f);
	DrawableObject p_leaf('l', Colorf("#FFFFFFFF"), leaf, Vector2f(1.0f,1.0f), m_programId, 0.4f );
	
	Plant pineapple(5, 0.05f, 1.5f, 1.4f, "[<R][P]", 6);
	pineapple.addObject(p_stalk);
	pineapple.addObject(p_fruit);
	pineapple.addObject(p_root);
	pineapple.addObject(p_leaf);


	pineapple.addRule(Rule('F',"[f]"));
	pineapple.addRule(Rule('R',"@R-#r#+r"));
	pineapple.addRule(Rule('P',"[B]%[B][<[@@@@@@F]"));
	pineapple.addRule(Rule('B',"++++[))S]B"));
	pineapple.addRule(Rule('S',"@S+#s+#s"));
	
	return pineapple;
}
void App::setUpPlant()
{	
	m_plants.push_back(createDustyMiller());
	m_plants.push_back(createCactus());
	m_plants.push_back(createTomato());

	m_resultPlant = createTomato();
	CombinePlants(m_resultPlant,createPineapple(),createBamboo(),m_bias,Stalk);
	m_resultPlant.setPosition(Vector3f(0.4f,0.2f,0));
}

void App::combinePlants(int l_index, int r_index, PlantPart part)
{
	LOGI("plant combination set: left: %d, right: %d, part: %d",l_index, r_index, (uint)part);
	CombinePlants(m_resultPlant, m_plants[l_index], m_plants[r_index],m_biases[part], part);
	m_resultPlant.setPosition(Vector3f(0.5f, 0.2f, 0.0f));
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
	
	///Enable blending and depth and set active texture
	glEnable(GL_DEPTH_TEST);
	checkGlError("glEnable");
	glEnable(GL_BLEND);
	checkGlError("glEnable");
	glBlendEquation(GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	checkGlError("glActiveTexture");
	///create shader and get all nessesary attributes for it
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


	//create and bind the framebuffer
	glGenFramebuffers(1,&m_framebufferHandle);
	checkGlError("glGenFramebuffers");	

	//create, bind and set nessasary attributes to the framebuffer texture - we need to acess the color later
	glGenTextures(1, &m_targetTexHandle);
	checkGlError("glGenTextures");	
	glBindTexture(GL_TEXTURE_2D, m_targetTexHandle);
	checkGlError("glBindTexture");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkGlError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	checkGlError("glTexParameteri");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE,0);
	checkGlError("glTexImage2D");
	glBindTexture(GL_TEXTURE_2D, 0);

	//create the depth renderbuffer - we don't need to output the depth
	glGenRenderbuffers(1,&m_renderBufferId);
	checkGlError("glGenRenderbuffers");
	glBindRenderbuffer(GL_RENDERBUFFER,m_renderBufferId);
	checkGlError("glBindRenderbuffer");
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,512,512);
	checkGlError("glRenderbufferStorage");	
	

	//bind the texture to the color attachment and renderbuffer to the depth
	glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferHandle);	
	checkGlError("glBindFramebuffer");		
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_targetTexHandle, 0);
	checkGlError("glFramebufferTexture2D");
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_renderBufferId);
	checkGlError("glFramebufferTexture2D");

	
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glGenTextures(1, &m_previewTexHandle);
	//loadImage(m_defaultTexture,"default.png");
	m_plants.resize(3);
	needsRedraw = true;
	//setUpPlant();
}
void App::RenderPlant()
{

	LOGI("render plant!");
	//setBias(0.5f, Stalk);
	//combinePlants(0, 2, Stalk);
	m_resultPlant.regeneratePlant();
	glBindTexture(GL_TEXTURE_2D, 0);
	checkGlError("glBindTexture");
	glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferHandle);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.9f);
	checkGlError("glBindFramebuffer");
	glViewport(0,0,(int)m_renderSize[0],(int)m_renderSize[1]);
	float aspect = m_renderSize[0] / m_renderSize[1];
	
	glUseProgram(m_programId);
	checkGlError("glUseProgram");

	glUniformMatrix4fv(glGetUniformLocation(m_programId, "mProjection"), 1, GL_FALSE, 
		Matrix4f::Orthographic(-100.0f, 100.0f, 0, 1, 0, aspect).Transposed().getValuePtr());
	checkGlError("glUniformMatrix4fv");
	
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_painter.drawPlant(m_resultPlant);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	//glDisable(GL_ALPHA_TEST);
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
	//m_resultPlant.regeneratePlant();
	//glViewport(0,0,(int)m_viewportSize[0], (int)m_viewportSize[1]);
	//float aspect = m_renderSize[0] / m_renderSize[1];
	//
	//glUseProgram(m_programId);
	//checkGlError("glUseProgram");

	//glUniformMatrix4fv(glGetUniformLocation(m_programId, "mProjection"), 1, GL_FALSE, 
	//	Matrix4f::Orthographic(-10.0f, 10.0f, 0, 1, 0, aspect).Transposed().getValuePtr());
	//checkGlError("glUniformMatrix4fv");
	//
	//glClearColor(1.0f,1.0f,1.0f,0.0f);
	//glClearDepthf(1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_painter.drawPlant(m_resultPlant);

	glViewport(0,0,(int)m_viewportSize[0], (int)m_viewportSize[1]);
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	///glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);		
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

	std::ifstream fileInput(std::string("./" + std::string(filename)).c_str(), std::ios::binary);
	
	if(!fileInput.is_open()) 
	{
		LOGE("IMAGE LOAD:CANNOT OPEN FILE %s\n",filename);
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


