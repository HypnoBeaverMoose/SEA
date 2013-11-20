#include "Definitions.h"
#include "AppAndroid.h"
#include<png.hpp>
#include<sstream>
#include"EGL\egl.h"
#include "EGL\eglplatform.h"
extern "C" 
{	
	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr);
    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnResize(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnRender(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnDestroy(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnCreate(JNIEnv * env, jobject obj);	
};


JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnResize(JNIEnv * env, jobject obj,  jint width, jint height)
{
	AppAndroid::getInstance()->OnResize(width, height);
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnRender(JNIEnv * env, jobject obj)
{
	AppAndroid::getInstance()->OnRender();
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnDestroy(JNIEnv * env, jobject obj)
{	
	LOGI("DestroyCallback");
	AppAndroid::getInstance()->OnDestroy();
}

JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_OnCreate(JNIEnv * env, jobject obj)
{	
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
		return ;
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
		return ;
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
		return ;

	LOGI("EGL ERROR: STEP_FOUR");
	}

	EGLContext l_context = eglCreateContext(l_display, l_configs[i], EGL_NO_CONTEXT, l_contextAttribs);
	if(l_context == EGL_NO_CONTEXT){
		LOGE("EGL ERROR: FAILED TO CREATE CONTEXT, %d",eglGetError());
		return ;	
	}

	LOGI("EGL ERROR: STEP_FIVE");
	if(eglMakeCurrent(l_display,l_surface, l_surface, l_context) == EGL_FALSE){
		LOGE("EGL ERROR: FAILED TO SET CURRENT CONTEXT");
		return ;
	}	

	LOGI("EGL ERROR: EGL CONTEXT SET UP!");
	AppAndroid::getInstance()->OnCreate();

}
JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr)
{
	AppAndroid* l_instance = AppAndroid::getInstance();
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
	
	//png::image<png::rgba_pixel> img;
	//if(AppAndroid::loadImageFromFile(AppAndroid::getInstance(), img,"img.png"))
	//	LOGI("image loaded %d %d",img.get_width(), img.get_height());
}

//AppAndroid* AppAndroid::s_instance = NULL;
void AppAndroid::CrateCallback()
{
	App::getInstance()->OnCreate();
}
void AppAndroid::RenderCallback()
{
	App::getInstance()->OnRender();
}

void AppAndroid::ResizeCallback(int width, int height)
{
	App::getInstance()->OnResize(width, height);
}

void AppAndroid::DestroyCallback()
{
	App::getInstance()->OnDestroy();
}

AppAndroid* AppAndroid::getInstance()
{
///	LOGI("AppAndroid::getInstance()");
	if(s_instance == NULL)
		s_instance = new AppAndroid();
	return (AppAndroid*)s_instance;
}

bool AppAndroid::loadImage(png::image<png::rgba_pixel>& image, const char* filename)
{
	AAsset* asset = AAssetManager_open(m_assetManager,filename,AASSET_MODE_STREAMING);

	if(asset == NULL){
		LOGE("IMAGE LOADING FAIL: FAILED TO LOAD INITAL ASSET");	
		return false;
	}

	off_t start, length;
	int ret = AAsset_openFileDescriptor(asset, &start, &length);
	if(ret < 0){
		LOGE("IMAGE LOADING FAIL: FILE DESCRIPTOR IS NOT OPEN!");	
		AAsset_close(asset);
		return false;
	}
	std::vector<char> buff(length,0);
	ret = AAsset_read(asset,buff.data(),length);
	if(ret != length)
	{
		LOGE("IMAGE LOADING FAIL: READING FAIL!");	
		AAsset_close(asset);
		return false;
	}
	try{
		std::istringstream stream;
		stream.rdbuf()->pubsetbuf(buff.data(),buff.size());
		image.read_stream(stream);
	}
	catch(...)
	{
		LOGE("IMAGE LOADING FAIL: LOADING ERROR!");	
	}
	AAsset_close(asset);
	return true;
}

AppAndroid::AppAndroid(void)
{
}


AppAndroid::~AppAndroid(void)
{

}
