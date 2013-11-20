#include "Definitions.h"
#include "PlantDatabase.h"
#include "PlantGenerator.h"
#include "App.h"
#include "PlantGeneratorApp.h"
#include<EGL\egl.h>
#include<EGL\eglplatform.h>
#include "android\asset_manager.h"

//extern "C" 
//{	
//	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr);
//	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_TestContext(JNIEnv * env, jobject obj);
//}
//
//
//JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_TestContext(JNIEnv * env, jobject obj)
//{
//	PlantGenerator::InitGenerator();
//}

//JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr)
//{
//	PlantGeneratorApp* l_instance = PlantGenerator::getInstance();
//	l_instance->m_assetManager = AAssetManager_fromJava(env,mgr);
//	if(l_instance->m_assetManager == NULL){
//		LOGE("FAILED TO LOAD ASSET MANAGER");	
//		return;
//	}
//	AAsset* asset = AAssetManager_open(l_instance->m_assetManager,"test.txt", AASSET_MODE_STREAMING);
//
//	if(asset == NULL){
//		LOGE("ASSET MANAGER FAIL: FAILED TO LOAD INITAL ASSET");	
//		return;
//	}
//
//	off_t  start, length;
//	int ret = AAsset_openFileDescriptor(asset,&start, &length);
//	if(ret < 0){
//		LOGE("ASSET MANAGER FAIL: FILE DESCRIPTOR IS NOT OPEN!");	
//		AAsset_close(asset);
//		return;
//	}
//
//	char* string  = new char[length + 1];	
//	ret = AAsset_read(asset,string,length);
//	if(ret !=length){
//		if(ret == 0 ) 
//			LOGE("ASSET MANAGER FAIL: EOF ENCOUNTERED");	
//		else  if(ret < 0)
//			LOGE("ASSET MANAGER FAIL: READ ERROR");		
//		AAsset_close(asset);
//		return;
//	}
//	string[length] = '\0';
//	
//	LOGI("%s",string);
//	AAsset_close(asset);	
//	delete string;
//}

PlantGeneratorApp* PlantGenerator::instance = NULL;
void PlantGenerator::setAssetManager(JNIEnv * env, jobject mgr)
{
	PlantGeneratorApp* l_instance = PlantGenerator::getInstance();
	l_instance->m_assetManager = AAssetManager_fromJava(env,mgr);
	if(l_instance->m_assetManager == NULL){
		LOGE("FAILED TO LOAD ASSET MANAGER");	
		return;
	}
	AAsset* asset = AAssetManager_open(l_instance->m_assetManager,"test.txt", AASSET_MODE_STREAMING);

	if(asset == NULL){
		LOGE("ASSET MANAGER FAIL: FAILED TO LOAD INITAL ASSET");	
		return;
	}

	off_t  start, length;
	int ret = AAsset_openFileDescriptor(asset,&start, &length);
	if(ret < 0){
		LOGE("ASSET MANAGER FAIL: FILE DESCRIPTOR IS NOT OPEN!");	
		AAsset_close(asset);
		return;
	}

	char* string  = new char[length + 1];	
	ret = AAsset_read(asset,string,length);
	if(ret !=length){
		if(ret == 0 ) 
			LOGE("ASSET MANAGER FAIL: EOF ENCOUNTERED");	
		else  if(ret < 0)
			LOGE("ASSET MANAGER FAIL: READ ERROR");		
		AAsset_close(asset);
		return;
	}
	string[length] = '\0';
	
	LOGI("%s",string);
	AAsset_close(asset);	
	delete string;
}

bool PlantGenerator::InitGenerator()
{
	EGLContext prevContext = eglGetCurrentContext();
	EGLDisplay prevDisplay = eglGetCurrentDisplay();
	EGLSurface prevSurfaceRead = eglGetCurrentSurface(EGL_READ);
	EGLSurface prevSurfaceDraw = eglGetCurrentSurface(EGL_DRAW);

	LOGI("CrateCallback");	
	LOGI("EGL ERROR: STEP_ONE");
	const EGLint l_attribs[] = 
	{
		EGL_RENDERABLE_TYPE,		EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE,				8,
		EGL_GREEN_SIZE,				8,
		EGL_BLUE_SIZE,				8,
		EGL_ALPHA_SIZE,				8,
		EGL_SURFACE_TYPE,			EGL_PBUFFER_BIT,
		EGL_NONE
	};

	EGLDisplay l_display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	EGLint l_major, l_minor;
	
	eglInitialize(l_display, &l_major, &l_minor);
	
	int l_nConfig;
	eglChooseConfig(l_display,l_attribs, 0,  1, &l_nConfig);
	const int num_configs = l_nConfig;
	
	if(l_nConfig <=0) {
		LOGE("EGL ERROR: NO CONFIGS MATCH THE CONFIG SPEC");
		return false;
	}
	std::auto_ptr<EGLConfig> l_pConfigs(new EGLConfig[num_configs]);
	
	eglChooseConfig(l_display, l_attribs, l_pConfigs.get(),num_configs,&l_nConfig);
	EGLConfig* l_configs = l_pConfigs.get();
	int i = 0; bool  found = false;
	LOGI("EGL ERROR: STEP_TWO, CONFIG COUNT: %d", l_nConfig);
	for(; i < l_nConfig; i++)
	{
		int r,g,b,a;
		eglGetConfigAttrib(l_display, l_configs[i], EGL_RED_SIZE, &r);
        eglGetConfigAttrib(l_display, l_configs[i], EGL_GREEN_SIZE, &g);
        eglGetConfigAttrib(l_display, l_configs[i], EGL_BLUE_SIZE, &b);
        eglGetConfigAttrib(l_display, l_configs[i], EGL_ALPHA_SIZE, &a);
		
		if(r ==8 && g == 8 && b == a && a == 8) {
			found = true; break; 
		}
	}
	if(!found){
		LOGE("EGL ERROR: FAILED TO FIND SUITABLE CONTEXT CONFIG");
		return false;
	}

	int l_contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	const EGLint srfPbufferAttr[] =
	{
		EGL_WIDTH, 512,
		EGL_HEIGHT, 512,
		//EGL_COLORSPACE, GL_RGBA,
		//EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGB,
		//EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
		EGL_LARGEST_PBUFFER, EGL_TRUE,
		EGL_NONE
	};
	
	LOGI("EGL ERROR: STEP_THREE");
	EGLSurface l_surface = 	eglCreatePbufferSurface(l_display, l_configs[i], srfPbufferAttr);	
	if(l_surface == EGL_NO_SURFACE) {		
		LOGE("EGL ERROR: FAILED TO CREATE RENDERING SURFACE	%d",eglGetError());
		return false;

	LOGI("EGL ERROR: STEP_FOUR");
	}

	EGLContext l_context = eglCreateContext(l_display, l_configs[i], EGL_NO_CONTEXT, l_contextAttribs);
	if(l_context == EGL_NO_CONTEXT){
		LOGE("EGL ERROR: FAILED TO CREATE CONTEXT, %d",eglGetError());
		return false;
	}

	LOGI("EGL ERROR: STEP_FIVE");
	if(eglMakeCurrent(l_display,l_surface, l_surface, l_context) == EGL_FALSE){
		LOGE("EGL ERROR: FAILED TO SET CURRENT CONTEXT");
		return false;
	}	
	
	LOGI("EGL ERROR: EGL CONTEXT SET UP!");
	//getInstance()->m_renderSize[0] = getInstance()->m_renderSize[1] = 512;
	//getInstance()->OnCreate();

	//if(prevDisplay != EGL_NO_DISPLAY && prevSurfaceDraw != EGL_NO_SURFACE 
	//				&& prevSurfaceRead != EGL_NO_SURFACE && prevContext !=EGL_NO_CONTEXT)
	//{
	//	if(eglMakeCurrent(prevDisplay,prevSurfaceDraw, prevSurfaceRead, prevContext) == EGL_FALSE){
	//		LOGE("EGL ERROR: FAILED TO SET PREVIOUS CONTEXT");
	//		return false;
	//	}	
	//}
}

void PlantGenerator::setDefaulBias(float leaves, float stalk, float flowers)
{
	getInstance()->SetDefaultBiases(leaves, stalk, flowers);
}

PlantGeneratorApp* PlantGenerator::getInstance()
{
	if ( instance ==NULL)
		instance = new	PlantGeneratorApp();
	return instance;
}

void PlantGenerator::loadPlants(PlantDatabase::PlantData plantOne, PlantDatabase::PlantData plantTwo, PlantDatabase::PlantData plantThree)
{
	Plant plant_one(plantOne.angle, plantOne.scale, plantOne.angleInc, plantOne.scaleInc, plantOne.axiom, plantOne.iterCount);
	Plant plant_two(plantTwo.angle, plantTwo.scale, plantTwo.angleInc, plantTwo.scaleInc, plantTwo.axiom, plantTwo.iterCount);	
	Plant plant_three(plantThree.angle, plantThree.scale, plantThree.angleInc, plantThree.scaleInc, plantThree.axiom, plantThree.iterCount);	

	getInstance()->m_plants.clear();
	getInstance()->m_plants.push_back(plant_one);
	getInstance()->m_plants.push_back(plant_two);
	getInstance()->m_plants.push_back(plant_three);
}

void PlantGenerator::setCombination(PlantGenerator::PlantPart plantPart, int lhs, int rhs, float bias)
{
	switch(plantPart)
	{
		case Stalk:
			getInstance()->m_stalkBias = bias;break;
		case Flowers:
			getInstance()->m_flowerBias = bias;break;
		case Leaves:
			getInstance()->m_leafBias = bias;break;
	}
}

bool PlantGenerator::RenderPlant(int width, int height)
{
	getInstance()->m_renderSize[0] = width;
	getInstance()->m_renderSize[1] = height;
	getInstance()->RenderPlant();
	return true;
}

const unsigned char* PlantGenerator::getRenderedImage(unsigned int& width, unsigned int& height)
{
	return getInstance()->getPlantImage(width, height);
}
