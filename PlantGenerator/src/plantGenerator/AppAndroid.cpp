#include "Definitions.h"
#include "AppAndroid.h"
#include<png.hpp>
#include<sstream>

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
	AppAndroid::getInstance()->OnCreate();

}
JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_SetAssetManager(JNIEnv * env, jobject obj, jobject mgr)
{
	AppAndroid::getInstance()->m_assetManager = AAssetManager_fromJava(env,mgr);

	if(AppAndroid::getInstance()->m_assetManager == NULL){
		LOGE("FAILED TO LOAD ASSET MANAGER");	
		return;
	}
	AAssetDir* dir = AAssetManager_openDir(AppAndroid::getInstance()->m_assetManager,"");
	const char* filename;
	AAssetDir_close(dir);

	AAsset* asset = AAssetManager_open(AppAndroid::getInstance()->m_assetManager,"test.txt", AASSET_MODE_STREAMING);

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
	
	png::image<png::rgba_pixel> img;
	if(AppAndroid::loadImageFromFile(AppAndroid::getInstance(), img,"img.png"))
		LOGI("image loaded %d %d",img.get_width(), img.get_height());
}

AppAndroid* AppAndroid::s_instance = NULL;
void AppAndroid::CrateCallback()
{
	getInstance()->OnCreate();
}
void AppAndroid::RenderCallback()
{
	getInstance()->OnRender();
}

void AppAndroid::ResizeCallback(int width, int height)
{
	getInstance()->OnResize(width, height);
}

void AppAndroid::DestroyCallback()
{
	getInstance()->OnDestroy();
}

AppAndroid* AppAndroid::getInstance()
{
	if(s_instance == NULL)
		s_instance = new AppAndroid();
	return s_instance;
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
	return true;
}

AppAndroid::AppAndroid(void)
{
}


AppAndroid::~AppAndroid(void)
{

}
